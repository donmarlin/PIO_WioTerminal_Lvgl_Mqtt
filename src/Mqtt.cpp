#define SerialUSB Serial

// wifi and mqtt stuff
#include "rpcWiFi.h"
#include "PubSubClient.h"
#include "Globals.h"

// array of mqtt topics to subscribe to
#define MAXTOPICS 10
#define HDRSPACE 6

const char* mqtt_server = "192.168.2.135";  // MQTT Broker URL
long lastMsg = 0;
char msg[256];
int value = 0;
int mqttMsgRecieved=0;

static TaskHandle_t mqtt_task_handle = NULL;

// topics to subscribe to
char* subTopics[MAXTOPICS] = {"vessels/self/propulsion/port/temperature",
                             "vessels/self/propulsion/port/oilPressure",
                             "vessels/self/propulsion/port/alternatorVoltage",
                             "vessels/self/propulsion/port/revolutions",
                             "vessels/self/environment/inside/engineRoom/temperature",
                             "","","","",""};
                             
// place to store the values and labels
float subTopicValues[MAXTOPICS] = {0,0,0,0,0,0,0,0,0,0};
float subTopicValuesOld[MAXTOPICS] = {0,0,0,0,0,0,0,0,0,0};
bool subTopicValuesChanged[MAXTOPICS] = {1,1,1,1,1,1,1,1,1,1};

// forward declarations
void mqttCallBack(char* , byte* , unsigned int );
void reconnect(void);

// from loop
void mqttTask(void *pvParameters) {

Serial.println( "Mqtt Task Start" );

  // loop untill mqtt connectd  
  while (!client.connected()) {
        reconnect();
  } // end while

  // loop forever
  for(;;){

    // if we are not connected to mqtt broker, then reconnect
    if (!client.connected()) {
        reconnect();
    } // end if

    // ensure we service mqtt messages
    client.loop();
    //Serial.print("Mqtt state ");
    //Serial.println(client.state());

    // send a message every so often
    //long now = millis();
    //if (now - lastMsg > 2000) {
    //  lastMsg = now;
    //  ++value;
    //  snprintf (msg, 50, "Wio Terminal #%ld", value);
    //  SerialUSB.print("Publish message: ");
    //  SerialUSB.println(msg);
    //  client.publish("WTout", msg);
    //} // end if

    // release for a bit
    vTaskDelay(50);
  } // end for

} // end mqttTask

// end from loop
void do_mqtt_init() {

  Serial.println( "Mqtt Init Start" );
  
  // delay for 1 second for network
  delay(1000);

  // some parameters for MQTT
  client.setKeepAlive(5);
  // connect
  client.setServer(mqtt_server, 1883); // Connect the MQTT Server
  // setup mqtt callback
  client.setCallback(mqttCallBack);

  Serial.println( "Mqtt Start Task" );

  // start the mqtt task
  xTaskCreate(
    &mqttTask,            // Pointer to the task entry function.
    "mqttTask",           // A descriptive name for the task for debugging.
    512,                  // size of the task stack in bytes.
    NULL,                 // Optional pointer to pvParameters
    10, // priority at which the task should run
    &mqtt_task_handle      // Optional pass back task handle
  );
  
  Serial.println( "Mqtt Init Done" );

} // end mqtt_init

// end from mqtt setup

// all incoming messages arrive here, pull out the value and stuff into values array
void mqttCallBack(char* topic, byte* payload, unsigned int length) {
  char buff_p[length+1];  

  // message count rollover
  if (mqttMsgRecieved++ > 8)
    mqttMsgRecieved=0;
        
  // copy value into array of chars
  buff_p[length+1] = 0; // null terminate
  for (int i = 0; i < length; i++) {
    buff_p[i] = (char)payload[i];
  } // end for
  
  // try and match the topic
  for (int j=0; j<MAXTOPICS; j++){
     if (!strcmp(topic,subTopics[j])) {
        // match so populate value and print message
        String tmp = String(buff_p);
        // only update if value changed
        if (tmp.toFloat() != subTopicValues[j]) {
          // save old value
          subTopicValuesOld[j]=subTopicValues[j];
          // convert to degC fromkelvin if required...hokey test check if > 273
          //if (tmp.toFloat() >= 273) {
          //  subTopicValues[j] = tmp.toFloat()-273;
          //} else {
          subTopicValues[j] = tmp.toFloat();  
          //} // end if
          // note new value
          subTopicValuesChanged[j] = 1;
          // engine coolant temp
          if (j == 0){
            locEngCoolTemp = (double)subTopicValues[j];
          } // end if
          // engine oil pressure in kpa
          if (j == 1){
            locEngOilPres = (double)subTopicValues[j];
          } // end if
          // alternator voltage
          if (j == 2){
            locEngAltVolt = (double)subTopicValues[j];
          } // end if
          // engine rpm          
          if (j == 3) {
            // come in as Hz...so mult by 60cycles/sec
            locEngRPM = (double)subTopicValues[j]*60.0;
          } // end if
        } // end if
        //#ifdef MYDEBUG
          SerialUSB.print("Message arrived, Topic Match = " + String(j) + " [");
          SerialUSB.print(topic);
          SerialUSB.print("] ");
          SerialUSB.print(String(subTopicValues[j]));
          SerialUSB.println();
        //#endif
        break; // exit for loop
     } // end if
  } // end for
} // end callback

// this reconnects to mqtt broker in the case of a disconnect
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    SerialUSB.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "WioTerm";
    clientId += String(random(0xffff), HEX);
    
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      SerialUSB.println("connected");
      // Once connected, publish an announcement...
      client.publish("WTout", "hello world");
      // ... and resubscribe, topics are in table
      for (int j=0; j<MAXTOPICS; j++){
        if (strlen(subTopics[j]) != 0) {
          client.subscribe(subTopics[j]);
        } // end if
      } // end for
    } else {
      SerialUSB.print("failed, rc=");
      SerialUSB.print(client.state());
      SerialUSB.println(" try again in 5 seconds");
      // Wait a bit before retrying
      vTaskDelay(500);
    } // end if
  } // end while
} // end reconnect
