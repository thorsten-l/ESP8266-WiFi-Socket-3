#include "WebPages.h"

void handleInfoPage()
{
  sendHeader(APP_NAME " - Info");
  sendPrint("<form class='pure-form'>");
  sendLegend("Application");
  sendPrint(
      "<p>Name: " APP_NAME "</p>"
      "<p>Version: " APP_VERSION "</p>"
      "<p>PlatformIO Environment: " PIOENV "</p>"
      "<p>Author: Dr. Thorsten Ludewig &lt;t.ludewig@gmail.com></p>");

  sendLegend("Build");
  sendPrint("<p>Date: " __DATE__ "</p>"
            "<p>Time: " __TIME__ "</p>");

  sendLegend("RESTful API");

  sendPrintf(
      "<p><a href='http://%s/info'>http://%s/info</a> - ESP8266 Info</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/on'>http://%s/on</a> - Socket ON</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/off'>http://%s/off</a> - Socket OFF</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/state'>http://%s/state</a> - Socket JSON status (0 or 1)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendLegend("Services");

  sendPrintf("<p>OTA Enabled: %s</p>",
             (appcfg.ota_enabled) ? "true" : "false");
  sendPrintf("<p>OpenHAB Callback Enabled: %s</p>",
             (appcfg.ohab_enabled) ? "true" : "false");
//  sendPrintf("<p>Alexa Enabled: %s</p>",
//             (appcfg.alexa_enabled) ? "true" : "false");
  sendPrintf("<p>MQTT Enabled: %s</p>",
             (appcfg.mqtt_enabled) ? "true" : "false");
  sendPrintf("<p>Syslog Enabled: %s</p>",
             (appcfg.syslog_enabled) ? "true" : "false");

  sendPrint("</form>");
  sendFooter();
}
