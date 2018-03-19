#!/bin/bash

sudo apt-get update # update repository list
sudo apt-get install -y subversion # install svn
sudo apt-get install -y git # install git
sudo apt-get install -y build-essential # gcc/g++, make, etc.
# sudo apt-get install -y ftp # install ftp client
sudo apt-get install -y gawk libgmp-dev libmpfr-dev libmpc-dev flex bison # requirements to build gcc

