# @Author: Burgy Benjamin <minidfx>
# @Date:   2016-02-15T20:37:15+01:00
# @Email:  benjamin.burgy@gmail.com
# @Last modified by:   minidfx
# @Last modified time: 2016-03-22T20:11:24+01:00

#!/bin/bash

if [ -z $PROJECT_PATH ]; then
  echo "PROJECT_PATH environment variable is empty or not defined."
  exit 1
fi

if [ -z $PEBBLE_SDK_PATH ]; then
  echo "PEBBLE_SDK_PATH environment variable is empty or not defined."
  exit 2
fi

ssh minidfx@couloir42.local "cd $PROJECT_PATH;$PEBBLE_SDK_PATH/pebble install --phone $PHONE_IP"
