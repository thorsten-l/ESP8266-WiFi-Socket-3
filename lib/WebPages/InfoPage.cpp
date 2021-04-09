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

  sendPrint("<p><b>Legacy API</b> - Returns in JSON format</p>");

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
      "<p><a href='http://%s/state'>http://%s/state</a></p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrint("<p>&nbsp;</p>");
  sendPrint("<p><b>JSON API</b> - Returns status in JSON format</p>");

  sendPrintf(
      "<p>http://%s/api/json - HTTP_POST socket (ON or OFF)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/api/json'>http://%s/api/json</a> - HTTP_GET</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p>http://%s/cmd - HTTP_POST (ON or OFF)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/cmd/ON'>http://%s/cmd/ON</a></p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/cmd/OFF'>http://%s/cmd/OFF</a></p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/state'>http://%s/state</a></p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrint("<p>&nbsp;</p>");
  sendPrint("<p><b>Plain text API</b> - Returns status in plain text format</p>");

  sendPrintf(
      "<p>http://%s/api/plain - HTTP_POST socket (ON or OFF)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/api/plain'>http://%s/api/plain</a> - HTTP_GET</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p>http://%s/plain/cmd - HTTP_POST (ON or OFF)</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/plain/cmd/ON'>http://%s/plain/cmd/ON</a> - Socket ON</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());
 
  sendPrintf(
      "<p><a href='http://%s/plain/cmd/OFF'>http://%s/plain/cmd/OFF</a> - Socket OFF</p>",
      WiFi.localIP().toString().c_str(), WiFi.localIP().toString().c_str());

  sendPrintf(
      "<p><a href='http://%s/plain/state'>http://%s/plain/state</a></p>",
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
//  sendPrintf("<p>Syslog Enabled: %s</p>",
//             (appcfg.syslog_enabled) ? "true" : "false");

  sendPrint("</form>");
  sendFooter();
}
