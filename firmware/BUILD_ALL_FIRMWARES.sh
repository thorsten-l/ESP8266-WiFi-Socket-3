#!/bin/bash

cd ..

fw_version=`awk '/APP_VERSION/ { print $3 }' lib/App/App.hpp | sed -e 's/"//g'`

echo "Firmware Version: $fw_version"

for pioenv in dev1 dev1lite obi_v1 obi_v2 bw_shp6 shelly1
do
  echo "Build environment: $pioenv"
  platformio run -s -e $pioenv
  cp ".pioenvs/$pioenv/firmware.bin" "firmware/firmware-$fw_version.$pioenv.bin"
done
