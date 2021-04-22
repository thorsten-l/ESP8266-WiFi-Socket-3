#include "WebPages.h"

void handleResetFirmware()
{
  sendAuthentication();
  sendHeader(APP_NAME " - Reset Firmware", true);
  sendPrint_P(PSTR("<form class='pure-form'>"));
  sendLegend_P(PSTR("Reset Firmware."));

  if (strcmp("true", server.arg(0).c_str()) == 0)
  {
    sendPrint_P(PSTR("<h4>Resetting firmware... restart takes about 30sec.</h4>"));
    app.firmwareReset();
  }
  else
  {
    sendPrint_P(PSTR("If you really want to reset to system defaults, you must select 'Yes' on the maintenance page."));
  }

  sendPrint_P(PSTR("</form>"));
  sendFooter();
}
