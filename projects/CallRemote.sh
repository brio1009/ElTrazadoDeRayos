#!/bin/bash

# The MIT License (MIT)
#
# Copyright (c) 2014 CantTouchDis <bauschp@informatik.uni-freiburg.de>
# Copyright (c) 2014 brio1009 <christoph1009@gmail.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

#this is used to call this project on a computer that has no git but checked out the project as svn.
#specify username hostname and what the output Image's should be called and they land in the bin/release
#after the programm finishes.
space=" "
username=$1
hostname=$2
image=$3
storeImagePath=../bin/release
projectDirectory=Documents/ElTrazadoDeRayos/trunk
releaseDir=bin/release
programWithParam=./ConsoleMain$space$image
whattocall="cd $projectDirectory; svn update; cd projects; ./BuildLinuxProjects.sh > /dev/null; cd gmake; make > /dev/null; cd ../../$releaseDir;$programWithParam"
#call the programm remote.
ssh $hostname $whattocall
#test if ssh was interrupted
test $? -gt 128 && exit 1
#copy the image.
scp $hostname:/home/$username/$projectDirectory/$releaseDir/$image* $storeImagePath/
#remove the images from the computer to save precious space.
ssh $hostname "cd $projectDirectory; cd $releaseDir; rm $image*"

