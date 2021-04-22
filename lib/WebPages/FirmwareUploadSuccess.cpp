#include "WebPages.h"

extern bool firmwareUploadFailed;
extern PGM_P firmwareUploadErrorMessage;

void handleFirmwareUploadSuccess()
{
  sendAuthentication();
  sendHeader(APP_NAME " - Firmware upload", true);
  sendPrint_P("<form class='pure-form'>");

  if (firmwareUploadFailed)
  {
    sendLegend_P(PSTR("Firmware upload FAILED!"));
    sendPrint_P(PSTR("<h4>ERROR: "));
    sendPrint_P(firmwareUploadErrorMessage);
    sendPrint_P(PSTR(".</h4>"));
    sendFooter();
  }
  else
  {
    sendLegend_P(PSTR("Firmware successfully uploaded."));
    sendPrint_P(PSTR("<h4 style='color: red'>Restarting System ... takes about 30s</h4></form>"));
    sendFooter();
    delay(1000);
    ESP.restart();
  }
}
