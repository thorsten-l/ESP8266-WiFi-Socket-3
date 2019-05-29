# ESP8266-WiFi-Socket-II

## Alternative Firmware for

- BlitzWolf SHP6, [https://www.blitzwolf.com/2300W-EU-WIFI-Smart-Socket-p-300.html](https://www.blitzwolf.com/2300W-EU-WIFI-Smart-Socket-p-300.html)

> At this it time the performance of the OpenHAB Callbacks for the energy monitor is really bad!. It is much faster to use a MQTT broker or the RESTFul API. In a future release i will implement the HTTP requests asynchronously and non-blocking.

- OBI Version 2, [https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko-weiss/p/4077806](https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko-weiss/p/4077806)

- OBI Version 1, [https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706](https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706)

## Precompiled Firmware

### Attention - update OTA from 1.1.x to 2.x for OBI Version 1 only

> If you like to update from firmware 1.1.x (OBI Version 1) to 2.x over the air (OTA) please first flash firmware version 1.2.0migration.
>
> If you try to update direct from 1.1.x to the current 2.x firmware
> version you will get an "out of memory" error
>
> Due to a new memory model the firmware will reset all settings to the default values

Find the precompiled firmware in the [firmware](https://github.com/thorsten-l/ESP8266-WiFi-Socket-II/tree/master/firmware) directory.

## Compile

This is a [PlatformIO](https://platformio.org/) project.

## Firmware Reset

1. Power on the WiFi Socket (plug in)
2. Press down and hold the Power-Button during the first 5s
3. Release Power-Button when the WiFi LED starts rapidly fast blinking
4. The firmware reset ends after about 30s indicated by a permanent WiFi LED on.

## Default Settings

After a firmware reset the following values are set.

|                            | value                 | description                           |
| -------------------------- | --------------------- | ------------------------------------- |
| Admin password             | admin                 | Setup admin password                  |
| WiFi Mode                  | AP                    | Accesspoint- or Station-Mode          |
| WiFi SSID                  | WifiSocketII-`esp id` | SSID in accesspoint mode              |
| WiFi Password              | 12345678              | Default WiFi password                 |
| Network Mode               | DHCP                  | Network mode STATIC or DHCP \*1       |
| Network IP-Address         | 192.168.192.1         | device ip address \*2                 |
| Network Mask               | 255.255.255.0         | Subnet mask \*2                       |
| Network Gateway            | 192.168.192.1         | Default gateway \*2                   |
| Network DNS                | 192.168.192.1         | DNS server address \*2                |
| OTA Hostname               | wifi-socket-1         | *O*ver *T*he *A*ir hostname           |
| OTA Password               | otapass               | OTA firmware upload password          |
| OpenHAB Callback Enabled   | **false**             |                                       |
| OpenHAB Version            | 1                     | Callback for OpenHAB version 1 or 2   |
| OpenHAB Item Voltage       | PowerMeter1Voltage    | ('-' = disables this item)            |
| OpenHAB Item Current       | PowerMeter1Current    | ('-' = disables this item)            |
| OpenHAB Item Power         | PowerMeter1Power      | ('-' = disables this item)            |
| OpenHAB Item Name          | WifiSocket1           |                                       |
| OpenHAB Host               | 192.168.1.1           |                                       |
| OpenHAB Port               | 80                    |                                       |
| OpenHAB Use Authentication | false                 |                                       |
| OpenHAB User               | user                  |                                       |
| OpenHAB Password           | password              |                                       |
| OpenHAB SendingInterval    | 60                    | interval time in seconds (0=disable)  |
| Alexa enabled              | **false**             |                                       |
| Alexa devicename           | Socket                | Devicename for Alexa HA               |
| MQTT Enabled               | **false**             |                                       |
| MQTT ClientId              | WifiSocket1           |                                       |
| MQTT Host                  | 192.168.1.1           |                                       |
| MQTT Port                  | 1883                  |                                       |
| MQTT Use Authentication    | false                 |                                       |
| MQTT User                  | user                  |                                       |
| MQTT Password              | password              |                                       |
| MQTT In Topic              | socket1/in            |  receiving commands                   |
| MQTT Out Topic             | socket1/out           |  sending callback                     |
| MQTT Topic Voltage         | powermeter1/voltage   |  ('-' = disables this topic)          |
| MQTT Topic Current         | powermeter1/current   |  ('-' = disables this topic)          |
| MQTT Topic Power           | powermeter1/power     |  ('-' = disables this topic)          |
| MQTT Topic JSON            | powermeter1/json      |  sending all values in JSON format    |
| MQTT Sending Interval      | 60                    |  interval time in seconds (0=disable) |
| Syslog Enabled             | **false**             |                                       |
| Syslog Host                | 192.168.1.1           |                                       |
| Syslog Port (TCP)          | 514                   |                                       |
| Syslog App Name            | gateway-doorbell      |                                       |

\*1 In WiFi Station-Mode only

\*2 will be overwritten from DHCP-Response

## Hardware

- [BlitzWolf SHP6](https://www.blitzwolf.com/2300W-EU-WIFI-Smart-Socket-p-300.html)

- [OBI Wifi Stecker Schuko - Version 2](https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko-weiss/p/4077806)

- [OBI Wifi Stecker Schuko - Version 1](https://www.obi.de/hausfunksteuerung/wifi-stecker-schuko/p/2291706)

## OpenHAB

### OpenHAB - HTTP Binding sample

```t
Switch LivingRoom_Power "Living Room Outlet" {http=">[ON:GET:http://wifi-socket.ip/on] >[OFF:GET:http://wifi-socket.ip/off]"}
```

If you like to read the state of the socket by OpenHAB. First create a file `getState.js` in the `transform` directory with the following content:

```javascript
JSON.parse(input).state;
```

add

```t
<[http://wifi-socket.ip/state:30000:JS(getState.js)]
```

to the `Switch` definition. Every 30s the state of the socket will be checked.

### OpenHAB - MQTT binding sample for the switch and the energy monitor values

#### MQTT Binding Version 1.x

```t
Switch WifiSocketBwShp6 "BW SHP6 Ein / Aus" {mqtt=">[mosquitto:socket-bw/in:command:ON:1],>[mosquitto:socket-bw/in:command:OFF:0],<[mosquitto:socket-bw/out:state:default]"}

Number WifiSocketBwShp6Voltage "BW SHP6 Voltage [%.1fV]" {mqtt="<[mosquitto:socket-bw/voltage:state:default]"}

Number WifiSocketBwShp6Current "BW SHP6 Current [%.2fA]" {mqtt="<[mosquitto:socket-bw/current:state:default]"}

Number WifiSocketBwShp6Power "BW SHP6 Power [%.1fW]" {mqtt="<[mosquitto:socket-bw/power:state:default]"}
```

#### MQTT Binding Version 2.x

For the Binding Version 2.x it is necessary to configure the MQTT Thing and according to that the items.

mqtt.things:

```t
Bridge mqtt:broker:mosquitto "Mosquitto" [ host="127.0.0.1", secure=false, clientID="MotherShip", username="$MQTT_USERNAME", password="$MQTT_PASSWORD"]
{
   Thing topic socket-bw "BW SHP6" {
    Channels:
     Type switch : state "BW SHP6 Ein / Aus" [ stateTopic="wsocket-bw/out", on="ON", off="OFF", commandTopic="socket-bw/in", on="ON", off="OFF" ]
     Type string : voltage [ stateTopic="wsocket-bw/voltage" ]
     Type string : current [ stateTopic="wsocket-bw/current" ]
     Type string : power [ stateTopic="wsocket-bw/power" ]
   }

}
```

mqtt.items:

```t
Switch WifiSocketBwShp6 "BW SHP6 Ein / Aus" { channel="mqtt:topic:mosquitto:socket-bw:state" }

Number WifiSocketBwShp6Voltage "BW SHP6 Voltage [%.1fV]" { channel="mqtt:topic:mosquitto:socket-bw:voltage" }

Number WifiSocketBwShp6Current "BW SHP6 Current [%.2fA]" { channel="mqtt:topic:mosquitto:socket-bw:current" }

Number WifiSocketBwShp6Power "BW SHP6 Power [%.1fW]" { channel="mqtt:topic:mosquitto:socket-bw:power" }
```

## References

- [PlatformIO](https://platformio.org/)
- [Arduino core for ESP8266 WiFi chip](https://github.com/esp8266/Arduino)
- [Async TCP Library for ESP8266](https://github.com/me-no-dev/ESPAsyncTCP)
- [Async Web Server for ESP8266 and ESP32](https://github.com/me-no-dev/ESPAsyncWebServer)
- [FauxmoESP - Amazon Alexa support for ESP8266 and ESP32 devices](https://bitbucket.org/xoseperez/fauxmoesp)
- [Arduino-pubsubclient - A client library for the ESP8266 that provides support for MQTT](https://github.com/knolleary/pubsubclient)
- [Pure.CSS - A nice CSS, as GZIP it is less than 4k so it is useful for embedded devices](https://purecss.io/)
