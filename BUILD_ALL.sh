#!/bin/bash

COMMAND='platformio run '

for i in `grep '\[env:' platformio.ini | awk -F: '{ print $2 }' | sed -e 's/\\]//'`; do
  echo $i
  COMMAND="$COMMAND -e $i"
done

$COMMAND
