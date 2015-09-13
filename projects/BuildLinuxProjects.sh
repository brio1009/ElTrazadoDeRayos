#!/bin/bash

UNAME=`uname`

if [ "$UNAME" == "Linux" ]; then
  chmod 700 ./premake5_linux
  ./premake5_linux clean
  ./premake5_linux gmake
else
  echo "You are not using Linux.\n"
fi
