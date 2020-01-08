BHIM=../../build/bhim
$BHIM -h 0 -x ./Bluetooth/Bluetooth.xml | tail -n 2
$BHIM -h 0 -x ./"Bluetooth v2(err1)"/"Bluetooth v2(err1).xml" | tail -n 1
$BHIM -h 2 -x ./"Bluetooth v2(err2)"/"Bluetooth v2(err2).xml" | tail -n 1
$BHIM -h 2 -x ./MultiProdCon/MultiProdCons.xml | tail -n 1
$BHIM -h 3 -x ./dm_target/dm_target.xml | tail -n 1
$BHIM -h 2 -x ./BinarySearchTree/BinarySearchTree.xml | tail -n 1
$BHIM -h 2 -x ./untimed-Lcrit/untimed-Lcrit.xml | tail -n 1
$BHIM -h 0 -x ./untimed-Maze/untimed-Maze.xml | tail -n 1
$BHIM -h 2 -x ./Lbh/Lbh.xml | tail -n 1
