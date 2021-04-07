#include <App.hpp>
#include <RelayHandler.hpp>
#include <Hlw8012Handler.hpp>
#include <WifiHandler.hpp>
#include <ESP8266WebServer.h>
#include "WebPages.h"

extern ESP8266WebServer server;

void handlePlainStatus(int json_state)
{
  bool powerIsOn;

  String message;

  switch (json_state)
  {
  case JSON_RELAY_ON:
    message = "ON";
    powerIsOn = true;
    break;

  case JSON_RELAY_OFF:
    message = "OFF";
    powerIsOn = false;
    break;

  default:
    powerIsOn = relayHandler.isPowerOn();
    message = (powerIsOn) ? "ON" : "OFF";
  };

  server.sendHeader("Access-Control-Allow-Origin", "*");
  sendHeaderNoCache();
  server.send(200, "text/plain", message);
}

void handlePlainStatusOn()
{
  relayHandler.delayedOn();
  handlePlainStatus(JSON_RELAY_ON);
}

void handlePlainStatusOff()
{
  relayHandler.delayedOff();
  handlePlainStatus(JSON_RELAY_OFF);
}

void handlePlainStatusState()
{
  handlePlainStatus(JSON_RELAY_STATE);
}

void handlePlainStatusPOST()
{
  LOG0("POST\n");
  LOG1( "args=%d\n", server.args());
  for( int i = 0; i<server.args(); i++ )
  {
    LOG1( "  - %s = %s\n", server.argName(i).c_str(), server.arg(i).c_str());
  }

  if ( server.arg(0) == "ON" )
  {
    handlePlainStatusOn();
  }
  else
  {
    handlePlainStatusOff();
  }
}
