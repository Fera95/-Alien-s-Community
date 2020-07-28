#!/bin/sh 
sudo apt-get update 
sudo apt-get install git exuberant-ctags dvi2ps dvipdfmx latex2html pandoc 
sudo apt-get  install cmake   libpng-dev libcurl4-nss-dev libfreetype6-dev libjpeg-dev libvorbis-dev libopenal-dev libphysfs-dev libgtk2.0-dev ibasound-dev libpulse-dev libflac-dev libdumb1-dev   libgl1-mesa-dev        
sudo add-apt-repository ppa:allegro/5.2
sudo apt-get install liballegro*5.2 liballegro*5-dev
git clone https://github.com/Fera95/Alien-s-Community.git
cd Alien-s-Community/gui
make
#etc.