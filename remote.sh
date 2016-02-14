#!/bin/bash

rsync -zrh --delete --delete-excluded "build/" --progress . couloir42.local:Developer/Pebble/clock_with_next_event

ssh minidfx@couloir42.local "cd Developer/Pebble/clock_with_next_event;/home/minidfx/pebble-sdk/bin/pebble $@"
