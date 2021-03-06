#include <App.hpp>
#include "WebPages.h"
#include <RelayHandler.hpp>
#ifdef HAVE_HLW8012
#include <Hlw8012Handler.hpp>
#endif

static bool webPowerState;
static char titleBuffer[100];

void handleRootPage()
{
  if ( server.args() == 1 && server.argName(0) == "power" )
  {
    if ( server.arg(0) == "ON")
    {
      relayHandler.delayedOn();
    }
    if ( server.arg(0) == "OFF" )
    {
      relayHandler.delayedOff();
    }
    server.sendHeader( "Location", "/", true );
    server.send ( 302, "text/plain", "");
    server.client().stop();
  }

  webPowerState = relayHandler.isDelayedPowerOn();
  
  sprintf(titleBuffer, APP_NAME " - %s", appcfg.ota_hostname);
  sendHeader(titleBuffer);

  sendPrint_P(PSTR("<form class='pure-form pure-form-aligned'><fieldset>"));

  sendLegend_P(PSTR("Current Status"));
  sendPrintf("<div id='idpwr' class='pure-button' "
                   "style='background-color: #%s'>Power is %s</div>",
                   webPowerState ? "80ff80" : "ff8080",
                   webPowerState ? "ON" : "OFF");

  sendLegend_P(PSTR("Actions"));
  sendPrint_P(PSTR(
      "<a href=\"/?power=ON\" class=\"pure-button button-on\">ON</a>"
      "<a href=\"/?power=OFF\" class=\"pure-button button-off\">OFF</a>"));

#if defined(HAVE_ENERGY_SENSOR) && defined(HAVE_HLW8012)
  sendLegend_P("Energy Sensor");
  int rid=0;
  char valueBuffer[32];

  sprintf( valueBuffer, "%0.1fV", hlw8012Handler.getVoltage() );
  sendTextGroupReadOnly( rid++, "Voltage", valueBuffer );
  sprintf( valueBuffer, "%0.2fA", hlw8012Handler.getCurrent() );
  sendTextGroupReadOnly( rid++, "Current", valueBuffer );
  sprintf( valueBuffer, "%0.1fW", hlw8012Handler.getPower() );
  sendTextGroupReadOnly( rid++, "Power", valueBuffer );
#endif

  sendLegend_P(PSTR("WiFi signal"));
  sendPrint_P(PSTR("<span id='wifi' style='vertical-align:middle;display:inline-block;width:48px;height:48px' class='wifi4'></span>" ));
  sendPrint_P(PSTR("<span id='wsig'></span>" ));

  sendPrint_P(PSTR("</fieldset></form>\n"));

  sendPrint_P( PSTR("<script>function getPowerState(){"
    "var wi = document.getElementById('wifi');"
    "var ws = document.getElementById('wsig');"
    "var s=document.getElementById('idpwr');"
#ifdef HAVE_ENERGY_SENSOR
    "var v=document.getElementById('pgid0'),"
        "c=document.getElementById('pgid1'),"
        "p=document.getElementById('pgid2');"
#endif
    "fetch('/info').then(resp=>resp.json()).then(function(o){"
#ifdef HAVE_ENERGY_SENSOR
      "v.value=o.voltage.toFixed(1)+'V',"
      "c.value=o.current.toFixed(2)+'A',"
      "p.value=o.power.toFixed(1)+'W';"
#endif
      "if(o.state===\"ON\"){"
         "s.textContent=\"Power is ON\";"
         "s.style=\"background-color: #80ff80\";"
      "} else { "
         "s.textContent=\"Power is OFF\";"
         "s.style=\"background-color: #ff8080\";"
      "}"
      "var i = Math.min(Math.trunc(o.wifi_signal/25)+1,4);"
      "ws.innerHTML= ' ' + o.wifi_signal + '%';"
      "wi.className='wifi' + i;"
    "})}"
    "getPowerState();"
    "setInterval(getPowerState,10000);"
  "</script>" ));

/*#else
  sendPrint_P(PSTR(
    "<script>"
      "function getPowerState(){"
        "var wi = document.getElementById(\"wifi\");"
        "var ws = document.getElementById(\"wsig\");"
        "var e = document.getElementById(\"idpwr\");"
        "fetch(\"/info\").then((resp) => resp.json()).then(function(data){"
          "if(data.state===\"ON\"){"
            "e.textContent=\"Power is ON\";"
            "e.style=\"background-color: #80ff80\";"
          "}"
          "else {"
            "e.textContent=\"Power is OFF\";"
            "e.style=\"background-color:#ff8080\";"
          "}"
          "var i = Math.min(Math.trunc(data.wifi_signal/25)+1,4);"
          "ws.innerHTML= ' ' + data.wifi_signal + '%';"
          "wi.className='wifi' + i;"
        "})"
      "}"
      "getPowerState();"
      "setInterval(getPowerState,10000);"
    "</script>"));
#endif
*/
/*
  sendPrint_P(PSTR(
    "<script>"
      "function getInfo(){"
        "var wi = document.getElementById(\"wifi\");"
        "var ws = document.getElementById(\"wsig\");"
        "fetch(\"/info\").then((resp) => resp.json()).then(function(data){"
          "var i = Math.min(Math.trunc(data.wifi_signal/25)+1,4);"
          "ws.innerHTML= ' ' + data.wifi_signal + '%';"
          "wi.className='wifi' + i;"
        "})"
      "}"
      "getInfo();"
      "setInterval(getInfo,61000);"
    "</script>"));
*/

  sendFooter();
}
