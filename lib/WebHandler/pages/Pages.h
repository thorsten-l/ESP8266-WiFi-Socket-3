#ifndef __PAGES_H__
#define __PAGES_H__

#include <App.hpp>
#include <DefaultAppConfig.h>
#include <ESP8266WebServer.h>

#define JSON_RELAY_ON    1
#define JSON_RELAY_OFF   2
#define JSON_RELAY_STATE 3

extern ESP8266WebServer server;

extern const char header_html[];
extern const char header2_html[];
extern const char footer_html[];

extern void handleRootPage();
extern void handleInfoPage();
extern void handleSetupPage();
extern void handleMaintenancePage();
extern void handleSaveConfigPage();
extern void handleBackupConfiguration();
extern void handleRestoreConfiguration();
extern void handleConfigFileUpload();
extern void handleFirmwareUpload();
extern void handleFirmwareUploadSuccess();
extern void handleResetFirmware();
extern void handleSystemRestart();
extern void handleJsonStatusOn();
extern void handleJsonStatusOff();
extern void handleJsonStatusState();

extern void sendAuthentication();
extern void sendHeaderNoCache();
extern void sendHeader(const char *title);
extern void sendHeader(const char *title, bool sendMetaRefresh);
extern void sendHeader(const char *title, bool sendMetaRefresh, const char *style);
extern void sendFooter();
extern void sendPrintf(const char *format, ...);
extern void sendPrint(const char *message);
extern void sendLegend(const char *name);

#endif
