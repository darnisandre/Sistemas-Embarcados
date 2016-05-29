export PATH=$PATH:~/Downloads/gcc-4.6.1/bin
make clean
make filter
rm ../mpsoc_sim/objects/*
rm ../mpsoc_sim/reports/*
cp code*.bin ../mpsoc_sim/objects/
cd ~/Desktop/Sistemas-Embarcados/mpsoc_sim
./mpsoc_sim 7 s
cd ~/Desktop/Sistemas-Embarcados/hellfire_lite
