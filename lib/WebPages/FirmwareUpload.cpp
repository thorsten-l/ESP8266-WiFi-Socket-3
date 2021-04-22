#include <App.hpp>
#include <ESP8266mDNS.h>
#include "WebPages.h"

bool firmwareUploadFailed;
PGM_P firmwareUploadErrorMessage;

PGM_P ERROR_UNDEFINED PROGMEM = "Undefined";
PGM_P ERROR_FILE_SIZE_ZERO PROGMEM = "Firmware file size is zero";
PGM_P ERROR_INVALID_FILENAME PROGMEM = "Invalid firmware filename";
PGM_P ERROR_NOT_ENOUGH_MEMORY PROGMEM = "Not enough memory";
PGM_P ERROR_WRONG_FILE_FORMAT PROGMEM = "Wrong file format";

static bool uploadError;

void handleFirmwareUpload()
{
  HTTPUpload &upload = server.upload();

  if (upload.status == UPLOAD_FILE_START)
  {
    uploadError = false;
    firmwareUploadErrorMessage = ERROR_UNDEFINED;
    firmwareUploadFailed = true;

    Serial.setDebugOutput(true);
    WiFiUDP::stopAll();
    Serial.printf("Update: %s\n", upload.filename.c_str());

    if (upload.filename.endsWith(String("." PIOENV_NAME ".bin")) == true)
    {
      uint32_t maxSketchSpace = (ESP.getFreeSketchSpace() - 0x1000) & 0xFFFFF000;

      if (!Update.begin(maxSketchSpace))
      {
        Update.printError(Serial);
        firmwareUploadErrorMessage = ERROR_NOT_ENOUGH_MEMORY;
        uploadError = true;
      }
    }
    else
    {
      Serial.println(ERROR_INVALID_FILENAME);
      firmwareUploadErrorMessage = ERROR_INVALID_FILENAME;
      uploadError = true;
    }

    Serial.println();
  }
  else if (upload.status == UPLOAD_FILE_WRITE)
  {
    size_t p = upload.totalSize * 100 / upload.contentLength;
    Serial.printf("\rupload progress = %u%%", p);
    Serial.flush();

    if (uploadError == false && Update.write(upload.buf, upload.currentSize) != upload.currentSize)
    {
      int errorCode = Update.getError();

      if (errorCode == 4)
      {
        firmwareUploadErrorMessage = ERROR_NOT_ENOUGH_MEMORY;
      }
      if (errorCode == 10)
      {
        firmwareUploadErrorMessage = ERROR_WRONG_FILE_FORMAT;
      }

      Update.printError(Serial);
    }
  }
  else if (upload.status == UPLOAD_FILE_END)
  {
    if (Update.getError() == 10)
    {
      firmwareUploadErrorMessage = ERROR_WRONG_FILE_FORMAT;
    }

    if (Update.end(true))
    {
      if (uploadError == false && upload.totalSize > 0)
      {
        firmwareUploadFailed = false;
        Serial.printf("\n\nUpdate Success: %u\nRebooting...\n\n", upload.totalSize);
      }
      else
      {
        Serial.println(ERROR_FILE_SIZE_ZERO);
        firmwareUploadErrorMessage = ERROR_FILE_SIZE_ZERO;
      }
    }
    else
    {
      Update.printError(Serial);
    }
    Serial.setDebugOutput(false);
  }
  yield();
}
