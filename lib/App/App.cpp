#include "App.hpp"
#include "DefaultAppConfig.h"
#include <FS.h>
#include <LittleFS.h>
#include <MicroJson.hpp>
#include <Util.hpp>
#include <Development.h>

struct tm appTimeinfo;
char appUptimeBuffer[64];
char appLocalIpBuffer[32];
char appDateTimeBuffer[32];

App app;
AppConfig appcfg;
AppConfig appcfgWR;
AppConfig appcfgRD;

char buffer[BUFFER_LENGTH+1];
char buffer2[BUFFER2_LENGTH+1];

char *formatChipId(char *attribute)
{
  snprintf(buffer, (ATTRIBUTE_SIZE-1), attribute, ESP.getChipId());
  buffer[(ATTRIBUTE_SIZE-1)] = 0;
  strncpy(attribute, buffer, (ATTRIBUTE_SIZE-1));
  return attribute;
}

const char *appUptime()
{
  time_t uptime = millis() / 1000;
  int uptimeSeconds = uptime % 60;
  int uptimeMinutes = (uptime / 60) % 60;
  int uptimeHours = (uptime / 3600) % 24;
  time_t uptimeDays = (uptime / 86400);
  sprintf(appUptimeBuffer, "%ld days, %d hours, %d minutes, %d seconds", uptimeDays, uptimeHours, uptimeMinutes,
          uptimeSeconds);
  return appUptimeBuffer;
}

void appShowHeader(Stream &out)
{
  out.println(PSTR("\n\n" APP_NAME " - " APP_VERSION " - " APP_AUTHOR));
  out.println(PSTR("BUILD: " __DATE__ " " __TIME__));
  out.println(PSTR("PIOENV: " PIOENV));
  out.println(PSTR("PIOPLATFORM: " PIOPLATFORM));
  out.println(PSTR("PIOFRAMEWORK: " PIOFRAMEWORK));
  out.printf_P(PSTR("ESP SDK Version: %s\n"), ESP.getSdkVersion());
  out.printf_P(PSTR("ESP Core Version: %s\n\n"), ESP.getCoreVersion().c_str());
}

App::App()
{
  sprintf_P(initFilename, PSTR("/%08X.ini"), ESP.getChipId());
  defaultConfig();
  initSPIFFS = false;
  initialized = true;
  doSystemRestart = false;
  ledActiveMode = false;
  ledNightMode = false;
}

void App::defaultConfig()
{
  strncpy(appcfg.wifi_ssid, DEFAULT_WIFI_SSID, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.wifi_password, DEFAULT_WIFI_PASSWORD, (ATTRIBUTE_SIZE-1));
  appcfg.wifi_mode = DEFAULT_WIFI_MODE;

  appcfg.net_mode = DEFAULT_NET_MODE;
  strncpy(appcfg.net_host, DEFAULT_NET_HOST, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.net_mask, DEFAULT_NET_MASK, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.net_gateway, DEFAULT_NET_GATEWAY, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.net_dns, DEFAULT_NET_DNS, (ATTRIBUTE_SIZE-1));

  strncpy(appcfg.ota_hostname, DEFAULT_OTA_HOSTNAME, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.ota_password, DEFAULT_OTA_PASSWORD, (ATTRIBUTE_SIZE-1));

  strncpy(appcfg.admin_password, DEFAULT_ADMIN_PASSWORD, (ATTRIBUTE_SIZE-1));

  appcfg.ohab_enabled = DEFAULT_OHAB_ENABLED;
  appcfg.ohab_version = DEFAULT_OHAB_VERSION;
  strncpy(appcfg.ohab_host, DEFAULT_OHAB_HOST, (ATTRIBUTE_SIZE-1));
  appcfg.ohab_port = DEFAULT_OHAB_PORT;
  appcfg.ohab_useauth = DEFAULT_OHAB_USEAUTH;
  strncpy(appcfg.ohab_user, DEFAULT_OHAB_USER, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.ohab_password, DEFAULT_OHAB_PASSWORD, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.ohab_itemname, DEFAULT_OHAB_ITEMNAME, (ATTRIBUTE_SIZE-1));

  appcfg.alexa_enabled = DEFAULT_ALEXA_ENABLED;
  strncpy(appcfg.alexa_devicename, DEFAULT_ALEXA_DEVICENAME, (ATTRIBUTE_SIZE-1));

  appcfg.mqtt_enabled = DEFAULT_MQTT_ENABLED;
  strncpy(appcfg.mqtt_clientid, DEFAULT_MQTT_CLIENTID, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_host, DEFAULT_MQTT_HOST, (ATTRIBUTE_SIZE-1));
  appcfg.mqtt_port = DEFAULT_MQTT_PORT;
  appcfg.mqtt_useauth = DEFAULT_MQTT_USEAUTH;
  strncpy(appcfg.mqtt_user, DEFAULT_MQTT_USER, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_password, DEFAULT_MQTT_PASSWORD, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_intopic, DEFAULT_MQTT_INTOPIC, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_outtopic, DEFAULT_MQTT_OUTTOPIC, (ATTRIBUTE_SIZE-1));

/*
  appcfg.syslog_enabled = DEFAULT_SYSLOG_ENABLED;
  strncpy(appcfg.syslog_host, DEFAULT_SYSLOG_HOST, (ATTRIBUTE_SIZE-1));
  appcfg.syslog_port = DEFAULT_SYSLOG_PORT;
  strncpy(appcfg.syslog_app_name, DEFAULT_SYSLOG_APP_NAME, (ATTRIBUTE_SIZE-1));
*/

#ifdef HAVE_ENERGY_SENSOR
  strncpy(appcfg.ohab_item_voltage, DEFAULT_OHAB_ITEM_VOLTAGE, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.ohab_item_current, DEFAULT_OHAB_ITEM_CURRENT, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.ohab_item_power, DEFAULT_OHAB_ITEM_POWER, (ATTRIBUTE_SIZE-1));
  appcfg.ohab_sending_interval = DEFAULT_OHAB_SENDING_INTERVAL;

  strncpy(appcfg.mqtt_topic_voltage, DEFAULT_MQTT_TOPIC_VOLTAGE, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_topic_current, DEFAULT_MQTT_TOPIC_CURRENT, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_topic_power, DEFAULT_MQTT_TOPIC_POWER, (ATTRIBUTE_SIZE-1));
  strncpy(appcfg.mqtt_topic_json, DEFAULT_MQTT_TOPIC_JSON, (ATTRIBUTE_SIZE-1));
#endif
  appcfg.mqtt_sending_interval = DEFAULT_MQTT_SENDING_INTERVAL;

#ifdef POWER_BUTTON_IS_MULTIMODE
  appcfg.power_button_mode = DEFAULT_POWER_BUTTON_MODE;
#endif

#ifdef WIFI_LED
  appcfg.led_night_mode_enabled = DEFAULT_LED_NIGHT_MODE_ENABLED;
  appcfg.led_night_mode_timeout = DEFAULT_LED_NIGHT_MODE_TIMEOUT;
#endif

  appcfg.inet_check_enabled = DEFAULT_INET_CHECK_ENABLED;
  appcfg.inet_check_period = DEFAULT_INET_CHECK_PERIOD;
  appcfg.inet_check_action = DEFAULT_INET_CHECK_ACTION;

  memcpy(&appcfgWR, &appcfg, sizeof(appcfg));
  memcpy(&appcfgRD, &appcfg, sizeof(appcfg));
}

void App::firmwareReset()
{
  if (LittleFS.begin())
  {
    LOG0("Removing init file\n");
    LittleFS.remove(initFilename);
    LittleFS.end();
  }
  delayedSystemRestart();
}

void App::formatSPIFFS()
{
  wifiLedOn();
  ESP.eraseConfig();

  if (LittleFS.begin())
  {
    LOG0("File system format started...\n");
    LittleFS.format();
    LOG0("File system format finished.\n");
    LittleFS.end();
  }
  else
  {
    LOG0("\nERROR: format filesystem.\n");
  }
  wifiLedOff();
}

void App::restartSystem()
{
  // watchdogTicker.detach();
  ESP.eraseConfig();
  LOG0("*** restarting system ***\n");
  delay(2000);
  ESP.restart();
  delay(2000);
  ESP.reset();
}

void App::setup()
{
  Serial.begin(74880);

#ifdef BOARD_TYPE_SHELLY1
  pinMode(POWER_BUTTON, INPUT);
#else
  pinMode(POWER_BUTTON, INPUT_PULLUP);
#endif

#ifdef WIFI_LED
  pinMode(WIFI_LED, OUTPUT);
  wifiLedOff();
#endif

#if defined(BOARD_TYPE_OBI_V1)
  pinMode(RELAY_TRIGGER_OFF, OUTPUT);
  pinMode(RELAY_TRIGGER_ON, OUTPUT);
  digitalWrite(RELAY_TRIGGER_ON, 1);
  digitalWrite(RELAY_TRIGGER_OFF, 1);
  delay(50);
  digitalWrite(RELAY_TRIGGER_OFF, 0);
  delay(50);
  digitalWrite(RELAY_TRIGGER_OFF, 1);
#endif

#if defined(BOARD_TYPE_OBI_V2) || defined(BOARD_TYPE_DEV1) ||         \
    defined(BOARD_TYPE_BW_SHP6) || defined(BOARD_TYPE_BW_SHP6_V11) || \
    defined(BOARD_TYPE_SHELLY1) || defined(BOARD_TYPE_GEBA_01SWP) ||  \
    defined(BOARD_TYPE_SONOFF_BASIC)

#ifdef POWER_LED
  pinMode(POWER_LED, OUTPUT);
  powerLedOff();
#endif

  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW);
#endif

  for (int i = 0; i < 5; i++)
  {
    wifiLedOn();
    delay(500);
    wifiLedOff();
    delay(500);
    Serial.println();
  }

  appShowHeader(Serial);
  Serial.printf("appcfg size: %d bytes\n\n", sizeof(appcfg));
  showChipInfo();

  if (LittleFS.begin())
  {
    if (LittleFS.exists(initFilename))
    {
      LOG1("Init file %s found.\n", initFilename);
      initSPIFFS = false;
    }
    else
    {
      LOG1("WARNING: Init file %s does not exist.\n", initFilename);
      initSPIFFS = true;
    }

    LittleFS.end();
  }
  else
  {
    initSPIFFS = true;
  }

#ifndef POWER_BUTTON_IS_MULTIMODE
  if (digitalRead(POWER_BUTTON) == false)
  {
    Serial.println();
    LOG0("*** Firmware RESET ***\n");
    Serial.println();

    for (int i = 0; i < 15; i++)
    {
      wifiLedOn();
      delay(100);
      wifiLedOff();
      delay(100);
    }

    initSPIFFS = true;
  }
#endif

  if (initSPIFFS == true)
  {
    formatSPIFFS();

    if (LittleFS.begin())
    {
      LOG0("writing init file\n");
      File initFile = LittleFS.open(initFilename, "w");
      initFile.write("true");
      initFile.close();
      LittleFS.end();
    }

    restartSystem();
  }

  ESP.eraseConfig();
  memcpy(&appcfgRD, &appcfg, sizeof(appcfg));
  loadConfig();

///////////////////////////////////////////////////////////////
#ifdef OVERRIDE_WIFI_SETTINGS
  appcfg.wifi_mode = OVERRIDE_WIFI_MODE;
  strcpy(appcfg.wifi_ssid, OVERRIDE_WIFI_SSID);
  strcpy(appcfg.wifi_password, OVERRIDE_WIFI_PASSWORD);
  appcfg.ota_enabled = OVERRIDE_OTA_ENABLED;
#endif
  ///////////////////////////////////////////////////////////////

  formatChipId(appcfg.ota_hostname);
  formatChipId(appcfg.mqtt_clientid);
  formatChipId(appcfg.mqtt_intopic);
  formatChipId(appcfg.mqtt_outtopic);

  memcpy(&appcfgWR, &appcfg, sizeof(appcfg));
}

void App::loadConfig()
{
  if (!LittleFS.begin())
  {
    LOG0("ERROR: Failed to mount file system");
  }
  else
  {
    if (LittleFS.exists(APP_CONFIG_FILE_JSON))
    {
      if (loadJsonConfig(APP_CONFIG_FILE_JSON) == false)
      {
        memcpy(&appcfg, &appcfgRD, sizeof(appcfg));
      }
      else
      {
        LOG0("ERROR: loading config.json file. Using default config.\n");
      }
    }
    else
    {
      LOG0("WARNING: appcfg file " APP_CONFIG_FILE_JSON
           " does not exist. Using default appcfg.\n");
    }
    LittleFS.end();
  }
}

void App::writeConfig()
{
  if (!LittleFS.begin())
  {
    LOG0("ERROR: Failed to mount file system");
  }
  else
  {
    LOG0("Writing " APP_CONFIG_FILE_JSON " file.\n");
    File configJson = LittleFS.open(APP_CONFIG_FILE_JSON, "w");
    uJson j = uJson(configJson);

    j.writeHeader();
    j.writeEntry(A_wifi_ssid, appcfgWR.wifi_ssid);
    j.writeEntry(A_wifi_password, appcfgWR.wifi_password);
    j.writeEntry(A_wifi_mode, appcfgWR.wifi_mode);

    j.writeEntry(A_net_mode, appcfgWR.net_mode);
    j.writeEntry(A_net_host, appcfgWR.net_host);
    j.writeEntry(A_net_mask, appcfgWR.net_mask);
    j.writeEntry(A_net_gateway, appcfgWR.net_gateway);
    j.writeEntry(A_net_dns, appcfgWR.net_dns);

    j.writeEntry(A_ota_enabled, appcfgWR.ota_enabled);
    j.writeEntry(A_ota_hostname, appcfgWR.ota_hostname);
    j.writeEntry(A_ota_password, appcfgWR.ota_password);

    j.writeEntry(A_admin_password, appcfgWR.admin_password);

    j.writeEntry(A_ohab_enabled, appcfgWR.ohab_enabled);
    j.writeEntry(A_ohab_version, appcfgWR.ohab_version);
    j.writeEntry(A_ohab_host, appcfgWR.ohab_host);
    j.writeEntry(A_ohab_port, appcfgWR.ohab_port);
    j.writeEntry(A_ohab_itemname, appcfgWR.ohab_itemname);
    j.writeEntry(A_ohab_useauth, appcfgWR.ohab_useauth);
    j.writeEntry(A_ohab_user, appcfgWR.ohab_user);
    j.writeEntry(A_ohab_password, appcfgWR.ohab_password);
#ifdef HAVE_ENERGY_SENSOR
    j.writeEntry(A_ohab_item_voltage, appcfgWR.ohab_item_voltage);
    j.writeEntry(A_ohab_item_current, appcfgWR.ohab_item_current);
    j.writeEntry(A_ohab_item_power, appcfgWR.ohab_item_power);
    j.writeEntry(A_ohab_sending_interval, appcfgWR.ohab_sending_interval);
#endif

    j.writeEntry(A_alexa_enabled, appcfgWR.alexa_enabled);
    j.writeEntry(A_alexa_devicename, appcfgWR.alexa_devicename);

    j.writeEntry(A_mqtt_enabled, appcfgWR.mqtt_enabled);
    j.writeEntry(A_mqtt_clientid, appcfgWR.mqtt_clientid);
    j.writeEntry(A_mqtt_host, appcfgWR.mqtt_host);
    j.writeEntry(A_mqtt_port, appcfgWR.mqtt_port);
    j.writeEntry(A_mqtt_intopic, appcfgWR.mqtt_intopic);
    j.writeEntry(A_mqtt_outtopic, appcfgWR.mqtt_outtopic);
    j.writeEntry(A_mqtt_useauth, appcfgWR.mqtt_useauth);
    j.writeEntry(A_mqtt_user, appcfgWR.mqtt_user);
    j.writeEntry(A_mqtt_password, appcfgWR.mqtt_password);
#ifdef HAVE_ENERGY_SENSOR
    j.writeEntry(A_mqtt_topic_voltage, appcfgWR.mqtt_topic_voltage);
    j.writeEntry(A_mqtt_topic_current, appcfgWR.mqtt_topic_current);
    j.writeEntry(A_mqtt_topic_power, appcfgWR.mqtt_topic_power);
    j.writeEntry(A_mqtt_topic_json, appcfgWR.mqtt_topic_json);
#endif
    j.writeEntry(A_mqtt_sending_interval, appcfgWR.mqtt_sending_interval);

/*
    j.writeEntry(A_syslog_enabled, appcfgWR.syslog_enabled);
    j.writeEntry(A_syslog_host, appcfgWR.syslog_host);
    j.writeEntry(A_syslog_port, appcfgWR.syslog_port);
    j.writeEntry(A_syslog_app_name, appcfgWR.syslog_app_name);
*/
#ifdef POWER_BUTTON_IS_MULTIMODE
    j.writeEntry(A_power_button_mode, appcfgWR.power_button_mode);
#endif

#ifdef WIFI_LED
    j.writeEntry(A_led_night_mode_enabled, appcfgWR.led_night_mode_enabled);
    j.writeEntry(A_led_night_mode_timeout, appcfgWR.led_night_mode_timeout);
#endif

    j.writeEntry(A_inet_check_enabled, appcfgWR.inet_check_enabled);
    j.writeEntry(A_inet_check_period, appcfgWR.inet_check_period);
    j.writeEntry(A_inet_check_action, appcfgWR.inet_check_action);

    j.writeFooter();
    configJson.close();

    FSInfo fs_info;
    LittleFS.info(fs_info);

    fsTotalBytes = fs_info.totalBytes;
    fsUsedBytes = fs_info.usedBytes;

    Serial.printf("\n--- SPIFFS Info ---\ntotal bytes = %d\n",
                  fs_info.totalBytes);
    Serial.printf("used bytes = %d\n", fs_info.usedBytes);
    Serial.printf("block size = %d\n", fs_info.blockSize);
    Serial.printf("page size = %d\n", fs_info.pageSize);
    Serial.printf("max open files = %d\n", fs_info.maxOpenFiles);
    Serial.printf("max path length = %d\n", fs_info.maxPathLength);

    LittleFS.end();
  }
}

void App::printConfig(AppConfig ac)
{
  Serial.println();
  Serial.println("--- App appcfguration -----------------------------------");
  Serial.println("  Security:");
  Serial.printf("    Admin password: %s\n", ac.admin_password);
  Serial.println("\n  WiFi:");
  Serial.printf("    SSID: %s\n", ac.wifi_ssid);
  Serial.printf("    Password: %s\n", ac.wifi_password);
  Serial.printf("    Mode: %s\n",
                (ac.wifi_mode == 1) ? "Station" : "Access Point");
  Serial.println("\n  Network:");
  Serial.printf("    Mode: %s\n",
                (ac.net_mode == NET_MODE_DHCP) ? "DHCP" : "Static");
  Serial.printf("    host address: %s\n", ac.net_host);
  Serial.printf("    gateway: %s\n", ac.net_gateway);
  Serial.printf("    netmask: %s\n", ac.net_mask);
  Serial.printf("    dns server: %s\n", ac.net_dns);
  Serial.println("\n  OTA:");
  Serial.printf("    Enabled: %s\n", (ac.ota_enabled ? "true" : "false"));
  Serial.printf("    Hostname: %s\n", ac.ota_hostname);
  Serial.printf("    Password: %s\n", ac.ota_password);
  Serial.println("\n  OpenHAB:");
  Serial.printf("    Enabled: %s\n", (ac.ohab_enabled ? "true" : "false"));
  Serial.printf("    Version: %d\n", ac.ohab_version);
  Serial.printf("    Host: %s\n", ac.ohab_host);
  Serial.printf("    Port: %d\n", ac.ohab_port);
  Serial.printf("    Use Auth: %s\n", (ac.ohab_useauth ? "true" : "false"));
  Serial.printf("    User: %s\n", ac.ohab_user);
  Serial.printf("    Password: %s\n", ac.ohab_password);
  Serial.printf("    Itemname: %s\n", ac.ohab_itemname);
#ifdef HAVE_ENERGY_SENSOR
  Serial.printf("    Item Voltage: %s\n", ac.ohab_item_voltage);
  Serial.printf("    Item Current: %s\n", ac.ohab_item_current);
  Serial.printf("    Item Power: %s\n", ac.ohab_item_power);
  Serial.printf("    Sending Interval: %ld\n", ac.ohab_sending_interval);
#endif
  Serial.println("\n  Alexa:");
  Serial.printf("    Enabled: %s\n", (ac.alexa_enabled ? "true" : "false"));
  Serial.printf("    Deviename: %s\n", ac.alexa_devicename);
  Serial.println("\n  MQTT:");
  Serial.printf("    Enabled: %s\n", (ac.mqtt_enabled ? "true" : "false"));
  Serial.printf("    Client ID: %s\n", ac.mqtt_clientid);
  Serial.printf("    Host: %s\n", ac.mqtt_host);
  Serial.printf("    Port: %d\n", ac.mqtt_port);
  Serial.printf("    Use Auth: %s\n", (ac.mqtt_useauth ? "true" : "false"));
  Serial.printf("    User: %s\n", ac.mqtt_user);
  Serial.printf("    Password: %s\n", ac.mqtt_password);
  Serial.printf("    In Topic: %s\n", ac.mqtt_intopic);
  Serial.printf("    Out Topic: %s\n", ac.mqtt_outtopic);
#ifdef HAVE_ENERGY_SENSOR
  Serial.printf("    Topic Voltage: %s\n", ac.mqtt_topic_voltage);
  Serial.printf("    Topic Current: %s\n", ac.mqtt_topic_current);
  Serial.printf("    Topic Power: %s\n", ac.mqtt_topic_power);
  Serial.printf("    Topic JSON: %s\n", ac.mqtt_topic_json);
#endif
  Serial.printf("    Sending Interval: %ld\n", ac.mqtt_sending_interval);

/*
  Serial.println("\n  Syslog:");
  Serial.printf("    Enabled: %s\n",
                (ac.syslog_enabled ? "true" : "false"));
  Serial.printf("    Host: %s\n", ac.syslog_host);
  Serial.printf("    Port: %d\n", ac.syslog_port);
  Serial.printf("    App Name: %s\n", ac.syslog_app_name);
*/
#ifdef POWER_BUTTON_IS_MULTIMODE
  Serial.println("\n  Power button:");
  Serial.printf("    Mode: %d\n", ac.power_button_mode);
#endif
#ifdef WIFI_LED
  Serial.println("\n  LED night mode:");
  Serial.printf("    Enabled: %s\n",
                (ac.led_night_mode_enabled ? "true" : "false"));
  Serial.printf("    led_night_mode_timeout: %ds\n", ac.led_night_mode_timeout);
#endif

  Serial.println("\n  Internet check mode:");
  Serial.printf("    Enabled: %s\n",
                (ac.inet_check_enabled ? "true" : "false"));
  Serial.printf("    inet_check_period: %ds\n", ac.inet_check_period);
  Serial.printf("    inet_check_action: %d\n", ac.inet_check_action);

  Serial.println("---------------------------------------------------------");
  Serial.println();
}

void App::delayedSystemRestart()
{
  doSystemRestart = true;
  systemRestartTimestamp = millis();
  systemRestartCounter = 6;
  LOG0("*** delayedSystemRestart ***\n");
}

void App::handle(unsigned long timestamp)
{

  if (doSystemRestart && ((timestamp - systemRestartTimestamp) > 1000))
  {

    systemRestartCounter--;

    if (systemRestartCounter > 0)
    {
      LOG1("*** system restart in %us ***\n", systemRestartCounter);
    }
    else
    {
      LOG1("*** do system restart *** (%lu)\n", (timestamp - systemRestartTimestamp));
      writeConfig();
      restartSystem();
    }

    systemRestartTimestamp = millis();
  }

  updateLedStates(timestamp);
}

bool App::loadJsonConfig(const char *filename)
{
  bool readError = false;
  char attributeName[128];

  File tmpConfig = LittleFS.open(filename, "r");

  uJson j = uJson(tmpConfig);

  if (j.readHeader())
  {
    memcpy(&appcfgRD, &appcfg, sizeof(appcfg));

    while (readError == false && j.readAttributeName(attributeName) == true)
    {
      readError |= j.readEntryChars(attributeName, A_wifi_ssid, appcfgRD.wifi_ssid);
      readError |= j.readEntryChars(attributeName, A_wifi_password, appcfgRD.wifi_password);
      readError |= j.readEntryInteger(attributeName, A_wifi_mode, &appcfgRD.wifi_mode);

      readError |= j.readEntryInteger(attributeName, A_net_mode, &appcfgRD.net_mode);
      readError |= j.readEntryChars(attributeName, A_net_host, appcfgRD.net_host);
      readError |= j.readEntryChars(attributeName, A_net_mask, appcfgRD.net_mask);
      readError |= j.readEntryChars(attributeName, A_net_gateway, appcfgRD.net_gateway);
      readError |= j.readEntryChars(attributeName, A_net_dns, appcfgRD.net_dns);

      readError |= j.readEntryBoolean(attributeName, A_ota_enabled, &appcfgRD.ota_enabled);
      readError |= j.readEntryChars(attributeName, A_ota_hostname, appcfgRD.ota_hostname);
      readError |= j.readEntryChars(attributeName, A_ota_password, appcfgRD.ota_password);

      readError |= j.readEntryChars(attributeName, A_admin_password, appcfgRD.admin_password);

      readError |= j.readEntryBoolean(attributeName, A_ohab_enabled, &appcfgRD.ohab_enabled);
      readError |= j.readEntryInteger(attributeName, A_ohab_version, &appcfgRD.ohab_version);
      readError |= j.readEntryChars(attributeName, A_ohab_host, appcfgRD.ohab_host);
      readError |= j.readEntryInteger(attributeName, A_ohab_port, &appcfgRD.ohab_port);
      readError |= j.readEntryBoolean(attributeName, A_ohab_useauth, &appcfgRD.ohab_useauth);
      readError |= j.readEntryChars(attributeName, A_ohab_user, appcfgRD.ohab_user);
      readError |= j.readEntryChars(attributeName, A_ohab_password, appcfgRD.ohab_password);
      readError |= j.readEntryChars(attributeName, A_ohab_itemname, appcfgRD.ohab_itemname);
#ifdef HAVE_ENERGY_SENSOR
      readError |= j.readEntryChars(attributeName, A_ohab_item_voltage, appcfgRD.ohab_item_voltage);
      readError |= j.readEntryChars(attributeName, A_ohab_item_current, appcfgRD.ohab_item_current);
      readError |= j.readEntryChars(attributeName, A_ohab_item_power, appcfgRD.ohab_item_power);
      readError |= j.readEntryULong(attributeName, A_ohab_sending_interval, &appcfgRD.ohab_sending_interval);
#endif

      readError |= j.readEntryBoolean(attributeName, A_alexa_enabled, &appcfgRD.alexa_enabled);
      readError |= j.readEntryChars(attributeName, A_alexa_devicename, appcfgRD.alexa_devicename);

      readError |= j.readEntryBoolean(attributeName, A_mqtt_enabled, &appcfgRD.mqtt_enabled);
      readError |= j.readEntryChars(attributeName, A_mqtt_clientid, appcfgRD.mqtt_clientid);
      readError |= j.readEntryChars(attributeName, A_mqtt_host, appcfgRD.mqtt_host);
      readError |= j.readEntryInteger(attributeName, A_mqtt_port, &appcfgRD.mqtt_port);
      readError |= j.readEntryChars(attributeName, A_mqtt_intopic, appcfgRD.mqtt_intopic);
      readError |= j.readEntryChars(attributeName, A_mqtt_outtopic, appcfgRD.mqtt_outtopic);
      readError |= j.readEntryBoolean(attributeName, A_mqtt_useauth, &appcfgRD.mqtt_useauth);
      readError |= j.readEntryChars(attributeName, A_mqtt_user, appcfgRD.mqtt_user);
      readError |= j.readEntryChars(attributeName, A_mqtt_password, appcfgRD.mqtt_password);
#ifdef HAVE_ENERGY_SENSOR
      readError |= j.readEntryChars(attributeName, A_mqtt_topic_voltage, appcfgRD.mqtt_topic_voltage);
      readError |= j.readEntryChars(attributeName, A_mqtt_topic_current, appcfgRD.mqtt_topic_current);
      readError |= j.readEntryChars(attributeName, A_mqtt_topic_power, appcfgRD.mqtt_topic_power);
      readError |= j.readEntryChars(attributeName, A_mqtt_topic_json, appcfgRD.mqtt_topic_json);
#endif
      readError |= j.readEntryULong(attributeName, A_mqtt_sending_interval, &appcfgRD.mqtt_sending_interval);

/*
      readError |= j.readEntryBoolean(attributeName, A_syslog_enabled, &appcfgRD.syslog_enabled);
      readError |= j.readEntryChars(attributeName, A_syslog_host, appcfgRD.syslog_host);
      readError |= j.readEntryInteger(attributeName, A_syslog_port, &appcfgRD.syslog_port);
      readError |= j.readEntryChars(attributeName, A_syslog_app_name, appcfgRD.syslog_app_name);
*/

#ifdef POWER_BUTTON_IS_MULTIMODE
      readError |= j.readEntryInteger(attributeName, A_power_button_mode, &appcfgRD.power_button_mode);
#endif

#ifdef WIFI_LED
      readError |= j.readEntryBoolean(attributeName, A_led_night_mode_enabled, &appcfgRD.led_night_mode_enabled);
      readError |= j.readEntryInteger(attributeName, A_led_night_mode_timeout, &appcfgRD.led_night_mode_timeout);
#endif
      readError |= j.readEntryBoolean(attributeName, A_inet_check_enabled, &appcfgRD.inet_check_enabled);
      readError |= j.readEntryInteger(attributeName, A_inet_check_period, &appcfgRD.inet_check_period);
      readError |= j.readEntryInteger(attributeName, A_inet_check_action, &appcfgRD.inet_check_action);
    }
  }

  tmpConfig.close();

  return readError;
}

void App::wifiLedOn()
{
#ifdef WIFI_LED
  wifiLedState = 1;
  digitalWrite(WIFI_LED, WIFI_LED_ON);
  ledStateTimestamp = millis();
#endif
}

void App::wifiLedOff()
{
#ifdef WIFI_LED
  wifiLedState = 0;
  digitalWrite(WIFI_LED, WIFI_LED_OFF);
  ledStateTimestamp = millis();
#endif
}

void App::wifiLedToggle()
{
#ifdef WIFI_LED
  if (wifiLedState == 1)
  {
    wifiLedOff();
  }
  else
  {
    wifiLedOn();
  }
#endif
}

void App::powerLedOn()
{
#ifdef POWER_LED
  powerLedState = 1;
  digitalWrite(POWER_LED, POWER_LED_ON);
  ledStateTimestamp = millis();
#endif
}

void App::powerLedOff()
{
#ifdef POWER_LED
  powerLedState = 0;
  digitalWrite(POWER_LED, POWER_LED_OFF);
  ledStateTimestamp = millis();
#endif
}

void App::powerLedToggle()
{
#ifdef POWER_LED
  if (powerLedState == 1)
  {
    powerLedOff();
  }
  else
  {
    powerLedOn();
  }
#endif
}

void App::updateLedStates(unsigned long timestamp)
{
#ifdef WIFI_LED
  if (appcfg.led_night_mode_enabled == true)
  {
    if (timestamp < (ledStateTimestamp + (appcfg.led_night_mode_timeout * 1000)))
    {
      if (ledActiveMode == false)
      {
        ledNightMode = false;
        ledActiveMode = true;
        if (wifiLedState == 1)
        {
          digitalWrite(WIFI_LED, WIFI_LED_ON);
        }
#ifdef POWER_LED
        if (powerLedState == 1)
        {
          digitalWrite(POWER_LED, POWER_LED_ON);
        }
#endif
      }
    }
    else
    {
      if (ledNightMode == false)
      {
        ledActiveMode = false;
        ledNightMode = true;
        digitalWrite(WIFI_LED, WIFI_LED_OFF);
#ifdef POWER_LED
        digitalWrite(POWER_LED, POWER_LED_OFF);
#endif
      }
    }
  }
#endif
}

void App::showLeds()
{
  ledStateTimestamp = millis();
}
