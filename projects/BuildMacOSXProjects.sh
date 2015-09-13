#!/bin/bash

UNAME=`uname`

if [ "$UNAME" == "Darwin" ]; then
  chmod 700 ./premake5_macosx
  ./premake5_macosx clean
  ./premake5_macosx gmake
  ./premake5_macosx xcode3
  ./premake5_macosx xcode4
else
  echo "You are not using MacOSX.\n"
fi