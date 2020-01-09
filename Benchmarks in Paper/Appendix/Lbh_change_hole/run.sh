BHIM=../../../build/bhim

echo "y" | $BHIM -h 2 -x ./Lbh.xml | tail -n 2
echo "y" | $BHIM -h 3 -x ./Lbh.xml | tail -n 1
echo "y" | $BHIM -h 4 -x ./Lbh.xml | tail -n 1
echo "y" | $BHIM -h 5 -x ./Lbh.xml | tail -n 1

