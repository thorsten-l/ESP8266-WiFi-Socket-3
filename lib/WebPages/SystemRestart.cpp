#include "WebPages.h"

void handleSystemRestart()
{
  sendAuthentication();
  sendHeader(APP_NAME " - System restart", true);
  sendPrint_P(PSTR("<form class='pure-form'>"));
  sendLegend_P(PSTR("System restart."));
  sendPrint_P(PSTR("<h4>Restart takes about 30sec.</h4>"));
  sendPrint_P(PSTR("</form>"));
  sendFooter();
  ESP.restart();
}
