# @Author: Burgy Benjamin <minidfx>
# @Date:   2016-02-15T18:16:05+01:00
# @Email:  benjamin.burgy@gmail.com
# @Last modified by:   minidfx
# @Last modified time: 2016-03-19T13:29:24+01:00
#!/bin/bash

rsync -zrh --delete --exclude "includes/" --progress . couloir42.local:Developer/Pebble/clock_with_next_event

ssh minidfx@couloir42.local "cd Developer/Pebble/clock_with_next_event;/home/minidfx/pebble-sdk/bin/pebble $@"
