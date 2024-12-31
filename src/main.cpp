#define SerialUSB Serial
#define MYDEBUG
//#define USE_LV_LOG

#include <Arduino.h>
#include <lvgl.h>

// for battery access
#include <SparkFunBQ27441.h>

// wifi and mqtt stuff
#include "rpcWiFi.h"
#include "PubSubClient.h"

// Update these with values suitable for your network.
// need to figure out a way to auto discover mqtt server
//const char* ssid = "Indulgence"; // WiFi Name
//const char* password = "XWsE52HZ";  // WiFi Password
//const char* mqtt_server = "10.10.10.1";  // MQTT Broker URL
const char* ssid = "BELL838"; // WiFi Name
const char* password = "C1E34F964543";  // WiFi Password

bool sw_dbnc = 0; // used to debounce navigation switch
bool refresh = 0; // used to for refresh of static items on all screens

#define MAXPAGES 6 // number of pages
int curPage = 0; // index of current page
#define MAXSUBSCREEN 4 // how many subscreens on a single page

WiFiClient wioClient;
PubSubClient client(wioClient);

float RssI;

// battery stuff
bool lipoPresent = 0; // flag for if battery is present
unsigned int soc;  // state-of-charge (%)
unsigned int volts; // battery voltage (mV)
int current; // average current (mA)
unsigned int fullCapacity; // full capacity (mAh)
unsigned int capacity; // remaining capacity (mAh)
int power; // average power draw (mW)
int health; // state-of-health (%)


// psuedo rtos - manage fast and slow tick 
#define FASTTICK 1000           // # ms in fast tick
#define SLOWTICK 10           // # fast ticks in slow tick
unsigned long currentMillis;    // storage for timer tick
unsigned long previousMillis;   // storage for timer tick
unsigned char previousSlowTick;   // storage for the slow tick count

// forward declarations
void updateBatteryStats(void);
void fastTick(void);
void slowTick(void);

// common data used for display
double locEngRPM = 0;
double locEngOilPres = 0;
double locEngOilTemp = 0;
double locEngCoolTemp = 0; 
double locEngAltVolt = 0; 
double locEngFuelRate = 0; 
double locEngHours = 0; 

// init for display
extern void do_lvgl_init();
extern void do_mqtt_init();

#ifdef USE_LV_LOG
void my_print(lv_log_level_t level, const char *file, uint32_t line, const char *dsc)
{
    Serial.printf("%s@%d->%s\r\n", file, line, dsc);
    delay(100);
}
#endif

// routine to setup the wifi connection
void setup_wifi() {

  // add small delay
  delay(200);
  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password); // Connecting WiFi

  // loop until connected
  while (WiFi.status() != WL_CONNECTED) {
    vTaskDelay(200); // use rtos friendly delay
    Serial.print(".");
  } // end while

  Serial.println("");
  Serial.println("WiFi connected");

  Serial.println("IP address: ");
  Serial.println(WiFi.localIP()); // Display Local IP Address
  Serial.println(rpc_system_version());
}


// right switch handler
void switchRight(){
  if (sw_dbnc == 0) {
    curPage++;
    sw_dbnc = 1;
    refresh = 1;
  } // debounce the switch

  if (curPage > (MAXPAGES-1)) 
    curPage = (MAXPAGES-1);
} // end switchRight

// left switch handler
void switchLeft(){
  if (sw_dbnc == 0) {
    curPage--;
    sw_dbnc = 1;
    refresh = 1;
  } // debounce the switch
  
  if (curPage < 0)
    curPage =0;
} // end switchLeft

// main setup code
void setup()
{
  // serial logging  
  Serial.begin(115200); /* prepare for possible serial debug */

  // check for battery
  if (lipo.begin()) {
    lipoPresent = 1;
    lipo.setCapacity(650);
    // get battery stats
    updateBatteryStats();
  } // end if
  
  // navigation using left and right of the 5 way switch
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(WIO_5S_LEFT),    switchLeft, FALLING);
  attachInterrupt(digitalPinToInterrupt(WIO_5S_RIGHT),  switchRight, FALLING);

  // setup wifi
  setup_wifi();

  // call display init
  do_lvgl_init();

#ifdef USE_LV_LOG
    lv_log_register_print(my_print); /* register print function for debugging */
#endif

  // start mqtt 
  do_mqtt_init();

  // Init timer ticks
  currentMillis = millis();
  previousMillis = currentMillis;
  previousSlowTick = 0;

} // end setup

void loop()
{

  // lvgl timer tick
  lv_timer_handler();

  // fetch current milli tick
  currentMillis = millis();

  // timer ticks here, have fast and slow tick
  if (currentMillis - previousMillis >= FASTTICK) {
    previousMillis = currentMillis;

    // cheesy debounce code
    if (sw_dbnc == 1) {
      sw_dbnc = 0;
    }

    // call the fast tick code
    fastTick();

    // do slow tick stuff
    //previousSlowTick = previousSlowTick+1;
    if (previousSlowTick++ > SLOWTICK) {
      previousSlowTick = 0;
      slowTick();
    }// end if
  } // end if

  // small delay
  vTaskDelay(10);
} // end loop

// do fast tick processing
void fastTick(void)
{
    #ifdef MYDEBUG
    Serial.println("Fast tick hit");
    #endif

    SerialUSB.print("Mqtt client state=");
    SerialUSB.println(client.state());


    #if 0
    // get RssI value every tick
    RssI = WiFi.RSSI();
    RssI = isnan(RssI) ? -100.0 : RssI;
    RssI = min(max(2 * (RssI + 100.0), 0.0), 100.0);
    #endif
    
    // poll battery stats if we have one
    if (lipoPresent){
      // general poll of battery health
      updateBatteryStats();
    }// end if
    
} // end fastTick

// do slow tick processing
void slowTick(void)
{
    #ifdef MYDEBUG
    Serial.println("Slow tick hit");
    #endif    
} // end slow tick


void updateBatteryStats()
{
  // Read battery stats from the BQ27441-G1A
  soc = lipo.soc();  // Read state-of-charge (%)
  volts = lipo.voltage(); // Read battery voltage (mV)
  current = lipo.current(AVG); // Read average current (mA)
  fullCapacity = lipo.capacity(FULL); // Read full capacity (mAh)
  capacity = lipo.capacity(REMAIN); // Read remaining capacity (mAh)
  power = lipo.power(); // Read average power draw (mW)
  health = lipo.soh(); // Read state-of-health (%)
  
 #ifdef MYDEBUG
  // Now print out those values:
  String toPrint = String(soc) + "% | ";
  toPrint += String(volts) + " mV | ";
  toPrint += String(current) + " mA | ";
  toPrint += String(capacity) + " / ";
  toPrint += String(fullCapacity) + " mAh | ";
  toPrint += String(power) + " mW | ";
  toPrint += String(health) + "%";
  SerialUSB.println(toPrint);
 #endif
} // end updatebatterystats
