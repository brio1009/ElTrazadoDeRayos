#!/bin/bash

UNAME=`uname`

if [ "$UNAME" == "Linux" ]; then
  chmod 700 ./premake4b5_linux
  ./premake4b5_linux clean
  ./premake4b5_linux gmake
else
  echo "You are not using Linux.\n"
fi
