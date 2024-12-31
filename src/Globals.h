#pragma once
#include "PubSubClient.h"

// from main
extern PubSubClient client;
extern char* ssid;

extern double locEngRPM;
extern double locEngOilPres;
extern double locEngOilTemp;
extern double locEngCoolTemp; 
extern double locEngAltVolt; 
extern double locEngFuelRate; 
extern double locEngHours; 

extern bool lipoPresent;
extern unsigned int soc;
extern int current;
