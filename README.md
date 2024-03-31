# Toy4Joy_SystemC
Toy project to practice SystemC skills

download and extract SystemC (2.3.3) files from SC website
####
mkdir objdir
sudo mkdir /usr/local/systemc-2.3.3
cd objdir
sudo apt-get install build-essential libpcap-dev
../configure --prefix=/usr/local/systemc-2.3.3
make
sudo make install
####
SystemC now is ready. Every time you have to set the environment variable in a new terminal
####
export SYSTEMC=/usr/local/systemc-2.3.3
####
