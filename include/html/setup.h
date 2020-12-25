#include <App.hpp>

const char SETUP_HTML_TEMPLATE[] PROGMEM =
  "<!-- %millis% -->\n"
  "<form class=\"pure-form pure-form-aligned\" action=\"savecfg\" method=\"POST\">\n"

#if defined(POWER_BUTTON_IS_MULTIMODE) || defined(WIFI_LED)
  "<div class='accordion'>Hardware</div>\n"
  "<div class='panel'>\n"
  "<fieldset>\n"

#if defined(WIFI_LED)
  "<div class='accordion'>LED Night Mode</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"wlid1\">Enabled</label><input id=\"wlid1\" type=\"checkbox\" name=\"led_night_mode_enabled\" value=\"true\" %led_night_mode_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"wlid2\">Timeout</label><input id=\"wlid2\" type=\"text\" name=\"led_night_mode_timeout\" maxlength=\"64\" value=\"%led_night_mode_timeout%\"></div>\n"
  "</div>\n"
#endif

  "<div class='accordion'>Internet Check Mode</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"inetc1\">Enabled</label><input id=\"inetc1\" type=\"checkbox\" name=\"inet_check_enabled\" value=\"true\" %inet_check_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"inetc4\">Destination</label><input id=\"inetc4\" type=\"text\" readonly value=\"captive.apple.com\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"inetc2\">Period</label><input id=\"inetc2\" type=\"text\" name=\"inet_check_period\" maxlength=\"64\" value=\"%inet_check_period%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"inetc3\">Mode</label><select id=\"inetc3\" name=\"inet_check_action\">"
    "<option %inet_check_action_1% value=\"1\">On disconnect switch off</option>\n"
    "<option %inet_check_action_2% value=\"2\">On disconnect switch on</option>\n"
    "<option %inet_check_action_3% value=\"3\">On connect switch off</option>\n"
    "<option %inet_check_action_4% value=\"4\">On connect switch on</option>\n"
    "<option %inet_check_action_5% value=\"5\">Mirror connection state</option>\n"
    "<option %inet_check_action_6% value=\"6\">Mirror inverse connection state</option>\n"
    "</select></div>\n"
  "</div>\n"

#if defined(POWER_BUTTON_IS_MULTIMODE)
  "<div class='accordion'>Power Button</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pbmid1\">Mode</label><select id=\"pbmid1\" name=\"power_button_mode\"><option %power_button_mode_switch% value=\"1\">Switch</option><option %power_button_mode_toggle% value=\"2\">Toggle</option><option %power_button_mode_toggle_switch% value=\"3\">Toggle Switch</option></select></div>\n"
  "</div>\n"
#endif

  "</fieldset></div>\n"
#endif

  "<div class='accordion'>Security</div>\n"
  "<div class='panel'>\n"
  "<fieldset>\n"
  
  "<div class='accordion'>Setup 'admin' user</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid0\">Password</label><input id=\"pgid0\" type=\"text\" name=\"admin_password\" maxlength=\"64\" value=\"%admin_password%\"></div>\n"
  "</div>\n"
  "</fieldset></div>\n"

  "<div class='accordion'>Network</div>\n"
  "<div class='panel'>\n"
  "<fieldset>\n"
  "<div class='accordion'>General</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid9\">Hostname</label><input id=\"pgid9\" type=\"text\" name=\"ota_hostname\" maxlength=\"64\" value=\"%ota_hostname%\"></div>\n"
  "</div>\n"

  "<div class='accordion'>WiFi</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid1\">Mode</label><select id=\"pgid1\" name=\"wifi_mode\"><option %wifi_mode_ap% value=\"2\">Access Point</option><option %wifi_mode_station% value=\"1\">Station</option></select></div>\n"
    "<div class=\"pure-control-group\"><label for=\"scannedNetwork\">Scanned Networks</label><select id=\"scannedNetwork\" onchange=\"document.getElementById('pgid2').value = this.value\"><option> </option>%scanned_network_options%</select></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid2\">SSID</label><input id=\"pgid2\" type=\"text\" name=\"wifi_ssid\" maxlength=\"64\" value=\"%wifi_ssid%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid3\">Password</label><input id=\"pgid3\" type=\"text\" name=\"wifi_password\" maxlength=\"64\" value=\"%wifi_password%\"></div>\n"
  "</div>\n"

  "<div class='accordion'>Interface</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid4\">Mode</label><select id=\"pgid4\" name=\"net_mode\"><option %net_mode_dhcp% value=\"2\">DHCP</option><option %net_mode_static% value=\"1\">Static</option></select></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid5\">Host Address</label><input id=\"pgid5\" type=\"text\" name=\"net_host\" maxlength=\"64\" value=\"%net_host%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid6\">Gateway</label><input id=\"pgid6\" type=\"text\" name=\"net_gateway\" maxlength=\"64\" value=\"%net_gateway%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid7\">Netmask</label><input id=\"pgid7\" type=\"text\" name=\"net_mask\" maxlength=\"64\" value=\"%net_mask%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid8\">DNS Server</label><input id=\"pgid8\" type=\"text\" name=\"net_dns\" maxlength=\"64\" value=\"%net_dns%\"></div>\n"
  "</div>\n"
  "</fieldset></div>\n"

  "<div class='accordion'>Services</div>\n"
  "<div class='panel'>\n"
  "<fieldset>\n"

  "<div class='accordion'>Over The Air - firmware update (OTA)</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"ogid01\">Enabled</label><input id=\"ogid01\" type=\"checkbox\" name=\"ota_enabled\" value=\"true\" %ota_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"ogid02\">Password</label><input id=\"ogid02\" type=\"text\" name=\"ota_password\" maxlength=\"64\" value=\"%ota_password%\"></div>\n"
  "</div>\n"

  "<div class='accordion'>OpenHAB</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid11\">Callback Enabled</label><input id=\"pgid11\" type=\"checkbox\" name=\"ohab_enabled\" value=\"true\" %ohab_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid12\">OpenHAB Version</label><select id=\"pgid12\" name=\"ohab_version\"><option %ohab_v1% value=\"1\">1.8</option><option %ohab_v2% value=\"2\">&gt;=2.0</option></select></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid14\">Host</label><input id=\"pgid14\" type=\"text\" name=\"ohab_host\" maxlength=\"64\" value=\"%ohab_host%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid15\">Port</label><input id=\"pgid15\" type=\"text\" name=\"ohab_port\" maxlength=\"64\" value=\"%ohab_port%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid16\">Use Authentication</label><input id=\"pgid16\" type=\"checkbox\" name=\"ohab_useauth\" value=\"true\" %ohab_useauth%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid17\">User</label><input id=\"pgid17\" type=\"text\" name=\"ohab_user\" maxlength=\"64\" value=\"%ohab_user%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid18\">Password</label><input id=\"pgid18\" type=\"text\" name=\"ohab_password\" maxlength=\"64\" value=\"%ohab_password%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid13\">Item Name</label><input id=\"pgid13\" type=\"text\" name=\"ohab_itemname\" maxlength=\"64\" value=\"%ohab_itemname%\"></div>\n"
#ifdef HAVE_ENERGY_SENSOR
    "<div class=\"pure-control-group\"><label for=\"emid01\">Item Voltage</label><input id=\"emid01\" type=\"text\" name=\"ohab_item_voltage\" maxlength=\"64\" value=\"%ohab_item_voltage%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid02\">Item Current</label><input id=\"emid02\" type=\"text\" name=\"ohab_item_current\" maxlength=\"64\" value=\"%ohab_item_current%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid03\">Item Power</label><input id=\"emid03\" type=\"text\" name=\"ohab_item_power\" maxlength=\"64\" value=\"%ohab_item_power%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid04\">Sending Interval</label><input id=\"emid04\" type=\"text\" name=\"ohab_sending_interval\" maxlength=\"64\" value=\"%ohab_sending_interval%\"></div>\n"
#endif
    "</div>\n"

/*
  "<div class='accordion'>Alexa</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid19\">Enabled</label><input id=\"pgid19\" type=\"checkbox\" name=\"alexa_enabled\" value=\"true\" %alexa_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid20\">Devicename</label><input id=\"pgid20\" type=\"text\" name=\"alexa_devicename\" maxlength=\"64\" value=\"%alexa_devicename%\"></div>\n"
  "</div>\n"
*/

  "<div class='accordion'>MQTT</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid21\">Enabled</label><input id=\"pgid21\" type=\"checkbox\" name=\"mqtt_enabled\" value=\"true\" %mqtt_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid22\">Client ID</label><input id=\"pgid22\" type=\"text\" name=\"mqtt_clientid\" maxlength=\"64\" value=\"%mqtt_clientid%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid23\">Host</label><input id=\"pgid23\" type=\"text\" name=\"mqtt_host\" maxlength=\"64\" value=\"%mqtt_host%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid24\">Port</label><input id=\"pgid24\" type=\"text\" name=\"mqtt_port\" maxlength=\"64\" value=\"%mqtt_port%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid25\">Use Authentication</label><input id=\"pgid25\" type=\"checkbox\" name=\"mqtt_useauth\" value=\"true\" %mqtt_useauth%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid26\">User</label><input id=\"pgid26\" type=\"text\" name=\"mqtt_user\" maxlength=\"64\" value=\"%mqtt_user%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid27\">Password</label><input id=\"pgid27\" type=\"text\" name=\"mqtt_password\" maxlength=\"64\" value=\"%mqtt_password%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid28\">In Topic</label><input id=\"pgid28\" type=\"text\" name=\"mqtt_intopic\" maxlength=\"64\" value=\"%mqtt_intopic%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid29\">Out Topic</label><input id=\"pgid29\" type=\"text\" name=\"mqtt_outtopic\" maxlength=\"64\" value=\"%mqtt_outtopic%\"></div>\n"
#ifdef HAVE_ENERGY_SENSOR
    "<div class=\"pure-control-group\"><label for=\"emid05\">Topic Voltage</label><input id=\"emid05\" type=\"text\" name=\"mqtt_topic_voltage\" maxlength=\"64\" value=\"%mqtt_topic_voltage%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid06\">Topic Current</label><input id=\"emid06\" type=\"text\" name=\"mqtt_topic_current\" maxlength=\"64\" value=\"%mqtt_topic_current%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid07\">Topic Power</label><input id=\"emid07\" type=\"text\" name=\"mqtt_topic_power\" maxlength=\"64\" value=\"%mqtt_topic_power%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid08\">Topic JSON</label><input id=\"emid08\" type=\"text\" name=\"mqtt_topic_json\" maxlength=\"64\" value=\"%mqtt_topic_json%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"emid09\">Sending Interval</label><input id=\"emid09\" type=\"text\" name=\"mqtt_sending_interval\" maxlength=\"64\" value=\"%mqtt_sending_interval%\"></div>\n"
#endif
    "</div>\n"

/*
  "<div class='accordion'>Syslog</div>\n"
  "<div class='panel sub-panel'>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid30\">Enabled</label><input id=\"pgid30\" type=\"checkbox\" name=\"syslog_enabled\" value=\"true\" %syslog_enabled%></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid31\">Host</label><input id=\"pgid31\" type=\"text\" name=\"syslog_host\" maxlength=\"64\" value=\"%syslog_host%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid32\">Port (TCP)</label><input id=\"pgid32\" type=\"text\" name=\"syslog_port\" maxlength=\"64\" value=\"%syslog_port%\"></div>\n"
    "<div class=\"pure-control-group\"><label for=\"pgid33\">App Name</label><input id=\"pgid33\" type=\"text\" name=\"syslog_app_name\" maxlength=\"64\" value=\"%syslog_app_name%\"></div>\n"
  "</div>\n"
*/

  "</fieldset></div>\n"
  "\n<p><input class=\"pure-button pure-button-primary\" type=\"submit\" value=\"Save Configuration\"></p>\n"
  "</form>\n"

  "<p>&nbsp;</p>\n"
;
