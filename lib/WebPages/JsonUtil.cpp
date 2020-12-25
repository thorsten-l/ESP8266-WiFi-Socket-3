#include <App.hpp>
#include <RelayHandler.hpp>
#include <Hlw8012Handler.hpp>
#include <WifiHandler.hpp>
#include <ESP8266WebServer.h>
#include "WebPages.h"

extern ESP8266WebServer server;

void handleJsonStatus(int json_state)
{
  bool powerIsOn;

  String message = F("{\"state\":");

  switch (json_state)
  {
  case JSON_RELAY_ON:
    message += "1";
    powerIsOn = true;
    break;

  case JSON_RELAY_OFF:
    message += "0";
    powerIsOn = false;
    break;

  default:
    powerIsOn = relayHandler.isPowerOn();
    message += (powerIsOn) ? "1" : "0";
  };

#if defined(HAVE_ENERGY_SENSOR) && defined(HAVE_HLW8012)
  char ebuffer[256];
  if ( powerIsOn )
  {
    sprintf( ebuffer, ",\"voltage\":%.2f,\"current\":%.2f,\"power\":%.2f", 
      hlw8012Handler.getVoltage(), hlw8012Handler.getCurrent(), 
      hlw8012Handler.getPower()
    );
  }
  else
  {
    sprintf( ebuffer, ",\"voltage\":%.2f,\"current\":0.0,\"power\":0.0", 
      hlw8012Handler.getVoltage()
    );
  }
  String m2(ebuffer);
  message += m2;
#endif

  message += "}\r\n";
  server.sendHeader("Access-Control-Allow-Origin", "*");
  sendHeaderNoCache();
  server.send(200, "application/json", message);
}

void handleJsonStatusOn()
{
  relayHandler.delayedOn();
  handleJsonStatus(JSON_RELAY_ON);
}

void handleJsonStatusOff()
{
  relayHandler.delayedOff();
  handleJsonStatus(JSON_RELAY_OFF);
}

void handleJsonStatusState()
{
  handleJsonStatus(JSON_RELAY_STATE);
}
