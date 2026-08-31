#!/bin/bash
echo "Installing bcm2835 library..."
cd /tmp
wget http://www.airspayce.com/mikem/bcm2835/bcm2835-1.71.tar.gz
tar zxvf bcm2835-1.71.tar.gz
cd bcm2835-1.71
./configure
make
sudo make install
cd ..
rm -rf bcm2835-1.71*
echo "bcm2835 installed successfully!"
