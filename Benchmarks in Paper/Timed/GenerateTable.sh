BHIM=../../build/bhim
$BHIM -h 0 -x ./Bluetooth/BluetoothTimed.xml | tail -n 2
$BHIM -h 2 -x ./Lcrit/Lcrit.xml | tail -n 1
$BHIM -h 2 -x ./Maze/Maze.xml | tail -n 1
