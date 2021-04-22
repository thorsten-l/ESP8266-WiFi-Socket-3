#include <App.hpp>
#include <ConfigAttributes.hpp>
#include "WebPages.h"

void paramChars(char *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    strncpy(dest, value, (ATTRIBUTE_SIZE-1));
    dest[(ATTRIBUTE_SIZE-1)] = 0;
  }
}

void paramInt(int *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    int v = 0;

    if (value != 0 && strlen(value) > 0)
    {
      v = atoi(value);
    }

    *dest = v;
  }
}

void paramUnsignedLong(unsigned long *dest, const char *paramName, const char *name,
                       const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    unsigned long v = 0;

    if (value != 0 && strlen(value) > 0)
    {
      v = atol(value);
    }

    *dest = v;
  }
}

void paramBool(bool *dest, const char *paramName, const char *name, const char *value)
{
  if (strcmp(name, paramName) == 0)
  {
    if (value != 0 && strlen(value) > 0)
    {
      *dest = strcmp("true", value) == 0;
    }
  }
}

void storeConfigValue(const char *name, const char *value)
{
  // Security
  paramChars(appcfgWR.admin_password, A_admin_password, name, value);

  // OTA
  paramBool(&appcfgWR.ota_enabled, A_ota_enabled, name, value);
  paramChars(appcfgWR.ota_hostname, A_ota_hostname, name, value);
  paramChars(appcfgWR.ota_password, A_ota_password, name, value);

  // WIFI
  paramInt(&appcfgWR.wifi_mode, A_wifi_mode, name, value);
  paramChars(appcfgWR.wifi_ssid, A_wifi_ssid, name, value);
  paramChars(appcfgWR.wifi_password, A_wifi_password, name, value);

  // Network
  paramInt(&appcfgWR.net_mode, A_net_mode, name, value);
  paramChars(appcfgWR.net_host, A_net_host, name, value);
  paramChars(appcfgWR.net_gateway, A_net_gateway, name, value);
  paramChars(appcfgWR.net_mask, A_net_mask, name, value);
  paramChars(appcfgWR.net_dns, A_net_dns, name, value);

  // MQTT
  paramBool(&appcfgWR.mqtt_enabled, A_mqtt_enabled, name, value);
  paramChars(appcfgWR.mqtt_clientid, A_mqtt_clientid, name, value);
  paramChars(appcfgWR.mqtt_host, A_mqtt_host, name, value);
  paramInt(&appcfgWR.mqtt_port, A_mqtt_port, name, value);
  paramBool(&appcfgWR.mqtt_useauth, A_mqtt_useauth, name, value);
  paramChars(appcfgWR.mqtt_user, A_mqtt_user, name, value);
  paramChars(appcfgWR.mqtt_password, A_mqtt_password, name, value);
  paramChars(appcfgWR.mqtt_intopic, A_mqtt_intopic, name, value);
  paramChars(appcfgWR.mqtt_outtopic, A_mqtt_outtopic, name, value);
  paramUnsignedLong(&appcfgWR.mqtt_sending_interval, A_mqtt_sending_interval, name, value);

  // OpenHAB
  paramBool(&appcfgWR.ohab_enabled, A_ohab_enabled, name, value );
  paramInt(&appcfgWR.ohab_version, A_ohab_version, name, value );
  paramChars(appcfgWR.ohab_itemname, A_ohab_itemname, name, value);
  paramChars(appcfgWR.ohab_host, A_ohab_host, name, value );
  paramInt(&appcfgWR.ohab_port, A_ohab_port, name, value );
  paramBool(&appcfgWR.ohab_useauth, A_ohab_useauth, name, value);
  paramChars(appcfgWR.ohab_user, A_ohab_user, name, value);
  paramChars(appcfgWR.ohab_password, A_ohab_password, name, value);

  // Alexa
  paramBool(&appcfgWR.alexa_enabled, A_alexa_enabled, name, value);
  paramChars(appcfgWR.alexa_devicename, A_alexa_devicename, name, value);

  // Syslog
  /*
  paramBool(&appcfgWR.syslog_enabled, A_syslog_enabled, name, value);
  paramChars(appcfgWR.syslog_host, A_syslog_host, name, value);
  paramInt(&appcfgWR.syslog_port, A_syslog_port, name, value);
  paramChars(appcfgWR.syslog_app_name, A_syslog_app_name, name, value);
*/

#ifdef POWER_BUTTON_IS_MULTIMODE
  paramInt(&appcfgWR.power_button_mode, A_power_button_mode, name, value);
#endif

#ifdef WIFI_LED
  paramBool(&appcfgWR.led_night_mode_enabled, A_led_night_mode_enabled, name, value);
  paramInt(&appcfgWR.led_night_mode_timeout, A_led_night_mode_timeout, name, value);
#endif

  paramBool(&appcfgWR.inet_check_enabled, A_inet_check_enabled, name, value);
  paramInt(&appcfgWR.inet_check_period, A_inet_check_period, name, value);
  paramInt(&appcfgWR.inet_check_action, A_inet_check_action, name, value);
}

void handleSaveConfigPage()
{
  sendAuthentication();
  sendHeader(APP_NAME " - Save Config", true);
  sendPrint_P(PSTR("<form class='pure-form'>"));
  sendLegend_P(PSTR("Configuration saved."));

  int numberOfArguments = server.args();
  sendPrint_P(PSTR("<pre>"));

  memset(&appcfgWR, 0, sizeof(appcfgWR));

  for (int i = 0; i < numberOfArguments - 1; i++)
  {
    const char *argName = server.argName(i).c_str();
    const char *argValue = server.arg(i).c_str();
    storeConfigValue(argName, argValue);
    sendPrintf("%2d. %s = %s\n", (i + 1), argName, argValue);
  }

  sendPrint_P(PSTR("</pre><h3 style='color: red'>Restarting System ... takes about 30s</h3></form>"));
  sendFooter();
  app.delayedSystemRestart();
}
