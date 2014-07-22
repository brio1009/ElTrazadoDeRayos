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

#this constant is used to determine the offset of computers used by this Script
offset=5
lastPeer=20
#timeout arguments for ssh change if not local.
sshTimeoutArgs="-o ConnectTimeout=1 -o ConnectionAttempts=1 -o BatchMode=yes -o StrictHostKeyChecking=no"
space=" "
username=$1
#use NAME%02d to use multiple computers.
computername=$2
hostname=$3
image=$4
chunks=$5
storeImagePath=../bin/release
projectDirectory=Documents/ElTrazadoDeRayos/trunk
releaseDir=bin/release
programWithParam=./ConsoleMain$space$image
whattocall="cd $projectDirectory;cd $releaseDir;$programWithParam"
#find the computers that run linux (are online)
echo "Searching for free peers"
numPeers=0
onlinePeers=();
for (( c=$offset; c<=$lastPeer && $numPeers<=$chunks; c++ ))
do
  exactComputerName=$(printf "$computername" $(($c)) )
  ssh -q $sshTimeoutArgs $exactComputerName.$hostname "exit"
  if [[ $(echo $?) -eq 0 ]]; then
    onlinePeers[$numPeers]=$c
    ((numPeers++))
  fi
done
if [[ $numPeers -lt $chunks ]]; then
  echo "Not enough peers found"
  exit 1
fi
echo "now calling the programm remote on the free peers"
echo "PeerID's: ${onlinePeers[*]}"
#call the programm remote.
for (( c=0; c<=$chunks-1; c++ ))
do
  callExactly="$whattocall""Chunk""$c $chunks $c > /dev/null"
  exactComputerName=$(printf "$computername" ${onlinePeers[$c]} )
  ssh $sshTimeoutArgs $exactComputerName.$hostname $callExactly &
done
wait
#test if ssh was interrupted
test $? -gt 128 && killall ssh && exit 1
#use convert to make one image out of them
exactComputerName=$(printf "$computername" ${onlinePeers[0]} )

convertString="convert $image* -fx \""
for (( c=0; c<=$chunks-2; c++ ))
do
  convertString="$convertString""u[$c]+"
done
convertString="$convertString""u[$(($chunks-1))]\" $image"".png"
ssh $sshTimeoutArgs $exactComputerName.$hostname "cd $projectDirectory; cd $releaseDir;$convertString"
#copy the image.
scp $sshTimeoutArgs $exactComputerName.$hostname:/home/$username/$projectDirectory/$releaseDir/$image.png $storeImagePath/
#remove the images from the computer to save precious space.
ssh $sshTimeoutArgs $exactComputerName.$hostname "cd $projectDirectory; cd $releaseDir; rm $image*"

