#!/bin/bash
# Copyright 2014 CantTouchDis

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

