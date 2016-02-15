#!/bin/bash

bash ./remote.sh build

if [ $? -lt 1 ]; then
  bash ./deploy.sh
fi
