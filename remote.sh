# @Author: Burgy Benjamin <minidfx>
# @Date:   2016-02-15T18:16:05+01:00
# @Email:  benjamin.burgy@gmail.com
# @Last modified by:   minidfx
# @Last modified time: 2016-03-22T20:12:05+01:00

#!/bin/bash

if [ -z $PROJECT_PATH ]; then
  echo "PROJECT_PATH environment variable is empty or not defined."
  exit 1
fi

if [ -z $PEBBLE_SDK_PATH ]; then
  echo "PEBBLE_SDK_PATH environment variable is empty or not defined."
  exit 2
fi

rsync -zrh --delete --exclude ".git/" --exclude "includes/" --progress . couloir42.local:$PROJECT_PATH

ssh minidfx@couloir42.local "cd $PROJECT_PATH;$PEBBLE_SDK_PATH/pebble $@"
