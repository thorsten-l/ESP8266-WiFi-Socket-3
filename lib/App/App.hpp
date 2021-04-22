#ifndef __APP_H__
#define __APP_H__

#include <Arduino.h>
#include <FS.h>
#include <DeviceConfig.hpp>
#include <ConfigAttributes.hpp>

#define LOG0( format ) Serial.printf( "(%lu) " format, millis())
#define LOG1( format, ... ) Serial.printf( "(%lu) " format, millis(), ##__VA_ARGS__ )

#define APP_NAME "WiFi Socket 3"
#define APP_AUTHOR "Dr. Thorsten Ludewig <t.ludewig@gmail.com>"
#define APP_CONFIG_FILE_JSON "/config.json"

// Network mode
#define NET_MODE_STATIC 1
#define NET_MODE_DHCP 2

// Power-Button Mode
#define POWER_BUTTON_MODE_SWITCH          1
#define POWER_BUTTON_MODE_TOGGLE          2
#define POWER_BUTTON_MODE_TOGGLE_SWITCH   3

// Internet Check Actions
#define INET_CHECK_ACTION_ON_DISCONNECT_SWITCH_OFF 1
#define INET_CHECK_ACTION_ON_DISCONNECT_SWITCH_ON 2
#define INET_CHECK_ACTION_ON_CONNECT_SWITCH_OFF 3
#define INET_CHECK_ACTION_ON_CONNECT_SWITCH_ON 4
#define INET_CHECK_ACTION_SHOW_CONNECTION_STATE 5
#define INET_CHECK_ACTION_SHOW_CONNECTION_STATE_INV 6

extern const char *appUptime();

#define BUFFER_LENGTH 4096
#define BUFFER2_LENGTH 256

extern char *buffer;
extern char *buffer2;
extern void appMemoryInit();
extern void appMemoryFree();

#define ATTRIBUTE_SIZE 40

typedef struct appconfig
{
  char wifi_ssid[ATTRIBUTE_SIZE];
  char wifi_password[ATTRIBUTE_SIZE];
  int  wifi_mode;

  int net_mode;
  char net_host[ATTRIBUTE_SIZE];
  char net_mask[ATTRIBUTE_SIZE];
  char net_gateway[ATTRIBUTE_SIZE];
  char net_dns[ATTRIBUTE_SIZE];

  bool ota_enabled;
  char ota_hostname[ATTRIBUTE_SIZE];
  char ota_password[ATTRIBUTE_SIZE];

  char admin_password[ATTRIBUTE_SIZE];

  bool ohab_enabled;
  int  ohab_version;
  char ohab_host[ATTRIBUTE_SIZE];
  int  ohab_port;
  bool ohab_useauth;
  char ohab_user[ATTRIBUTE_SIZE];
  char ohab_password[ATTRIBUTE_SIZE];
  char ohab_itemname[ATTRIBUTE_SIZE];
#ifdef HAVE_ENERGY_SENSOR
  char ohab_item_voltage[ATTRIBUTE_SIZE];
  char ohab_item_current[ATTRIBUTE_SIZE];
  char ohab_item_power[ATTRIBUTE_SIZE];
  unsigned long ohab_sending_interval;
#endif
  bool alexa_enabled;
  char alexa_devicename[ATTRIBUTE_SIZE];

  bool mqtt_enabled;
  char mqtt_clientid[ATTRIBUTE_SIZE];
  char mqtt_host[ATTRIBUTE_SIZE];
  int  mqtt_port;
  bool mqtt_useauth;
  char mqtt_user[ATTRIBUTE_SIZE];
  char mqtt_password[ATTRIBUTE_SIZE];
  char mqtt_intopic[ATTRIBUTE_SIZE];
  char mqtt_outtopic[ATTRIBUTE_SIZE];
#ifdef HAVE_ENERGY_SENSOR
  char mqtt_topic_voltage[ATTRIBUTE_SIZE];
  char mqtt_topic_current[ATTRIBUTE_SIZE];
  char mqtt_topic_power[ATTRIBUTE_SIZE];
  char mqtt_topic_json[ATTRIBUTE_SIZE];
#endif
  unsigned long mqtt_sending_interval;

/*
  bool syslog_enabled;
  char syslog_host[ATTRIBUTE_SIZE];
  int syslog_port;
  char syslog_app_name[ATTRIBUTE_SIZE];
*/

#ifdef POWER_BUTTON_IS_MULTIMODE
  int power_button_mode;
#endif

#ifdef WIFI_LED
  bool led_night_mode_enabled;
  int led_night_mode_timeout;  
#endif

  bool inet_check_enabled; // check captive.apple.de
  int  inet_check_period;  // in seconds
  int  inet_check_action;

} AppConfig;

class App
{
private:
  char initFilename[32];
  bool initialized = false;
  bool doSystemRestart;
  unsigned long systemRestartTimestamp;
  unsigned int systemRestartCounter;
  bool initSPIFFS = false;
  unsigned long ledStateTimestamp;
  int wifiLedState;
  int powerLedState;
  bool ledNightMode;
  bool ledActiveMode;
  bool internetIsConnected;

  void formatSPIFFS();
  void loadConfig();
  void restartSystem();
  void updateLedStates( unsigned long timestamp );

public:
  size_t fsTotalBytes;
  size_t fsUsedBytes;

  App();

  void setup();
  void firmwareReset();
  void defaultConfig();
  void writeConfig();
  bool loadJsonConfig( const char *filename );
  void printConfig(AppConfig ac);
  void delayedSystemRestart();
  void handle( unsigned long timestamp );
  //
  void showLeds();
  void wifiLedOn();
  void wifiLedOff();
  void wifiLedToggle();
  void powerLedOn();
  void powerLedOff();
  void powerLedToggle();
};

extern App app;
extern AppConfig appcfg;
extern AppConfig appcfgWR;
extern AppConfig appcfgRD;

#endif
