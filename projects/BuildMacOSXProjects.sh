#!/bin/bash

UNAME=`uname`

if [ "$UNAME" == "Darwin" ]; then
  chmod 700 ./premake4b5_macosx
  ./premake4b5_macosx clean
  ./premake4b5_macosx gmake
  ./premake4b5_macosx xcode3
  ./premake4b5_macosx xcode4
else
  echo "You are not using MacOSX.\n"
fi