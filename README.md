#####################
###Toy4Joy_SystemC###
#####################

Toy project to practice SystemC skills
	
	1.FIFO_Camparison: Including 4 fifos, normal fifo in signal level, FWFT fifo in signal level, transaction-level fifo with sc_interface, transaction-level fifo with TLM. In testbench, you can easily see the advantages of TLM and high level communication.
	
	2.

##########################
###Setting environment:###
##########################

Download and extract SystemC (2.3.3) files from SC website
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
