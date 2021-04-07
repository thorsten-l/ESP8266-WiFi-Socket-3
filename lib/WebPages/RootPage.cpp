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

  sendPrint("<form class='pure-form pure-form-aligned'><fieldset>");

  sendLegend("Current Status");
  sendPrintf("<div id='idpwr' class='pure-button' "
                   "style='background-color: #%s'>Power is %s</div>",
                   webPowerState ? "80ff80" : "ff8080",
                   webPowerState ? "ON" : "OFF");

  sendLegend("Actions");
  sendPrint(
      "<a href=\"/?power=ON\" class=\"pure-button button-on\">ON</a>"
      "<a href=\"/?power=OFF\" class=\"pure-button button-off\">OFF</a>");

#if defined(HAVE_ENERGY_SENSOR) && defined(HAVE_HLW8012)
  prLegend(response, "Energy Sensor");
  int rid=0;
  char valueBuffer[32];
  sprintf( valueBuffer, "%0.1fV", hlw8012Handler.getVoltage() );
  prTextGroupReadOnly( response, rid++, "Voltage", valueBuffer );
  sprintf( valueBuffer, "%0.2fA", hlw8012Handler.getCurrent() );
  prTextGroupReadOnly( response, rid++, "Current", valueBuffer );
  sprintf( valueBuffer, "%0.1fW", hlw8012Handler.getPower() );
  prTextGroupReadOnly( response, rid++, "Power", valueBuffer );
#endif

  sendPrint("</fieldset></form>\n");

#ifdef HAVE_ENERGY_SENSOR
  response->print( "<script>function getPowerState(){"
    "var v=document.getElementById('pgid0'),"
        "c=document.getElementById('pgid1'),"
        "p=document.getElementById('pgid2');"
        "s=document.getElementById('idpwr');"
    "fetch('/state').then(resp=>resp.json()).then(function(o){"
      "v.value=o.voltage.toFixed(1)+'V',"
      "c.value=o.current.toFixed(2)+'A',"
      "p.value=o.power.toFixed(1)+'W';"
      "if(o.state===1){"
         "s.textContent=\"Power is ON\";"
         "s.style=\"background-color: #80ff80\";"
      "} else { "
         "s.textContent=\"Power is OFF\";"
         "s.style=\"background-color: #ff8080\";"
      "}"
    "})}setInterval(getPowerState,5e3);</script>" );
#else
  sendPrint(
      "<script>function getPowerState(){var e = "
      "document.getElementById(\"idpwr\");fetch(\"/state\").then((resp) => "
      "resp.json()).then(function(data){if(data.state===\"ON\"){e.textContent="
      "\"Power is ON\";e.style=\"background-color: #80ff80\";} else "
      "{e.textContent=\"Power is OFF\";e.style=\"background-color: "
      "#ff8080\";}});} setInterval(getPowerState,10000);</script>");
#endif
  sendFooter();
}
