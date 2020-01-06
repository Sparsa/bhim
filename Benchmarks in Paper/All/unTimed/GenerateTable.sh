BHIM=../../../build/bhim
$BHIM -h 0 -x ./Bluetooth.xml | tail -n 2
$BHIM -h 2 -x ./MultiProdCons.xml | tail -n 1
$BHIM -h 2 -x ./BinarySearchTree.xml | tail -n 1
$BHIM -h 2 -x ./untimed-Lcrit.xml | tail -n 1
$BHIM -h 2 -x ./untimed-Maze.xml | tail -n 1
$BHIM -h 2 -x ./Lbh.xml | tail -n 1
