# @Author: Burgy Benjamin <minidfx>
# @Date:   2016-02-15T18:16:05+01:00
# @Email:  benjamin.burgy@gmail.com
# @Last modified by:   minidfx
# @Last modified time: 2016-03-19T22:10:22+01:00

#!/bin/bash

rsync -zrh --delete --exclude "includes/" --progress . couloir42.local:Developer/Pebble/ClockLight

ssh minidfx@couloir42.local "cd Developer/Pebble/ClockLight;/home/minidfx/pebble-sdk/bin/pebble $@"
