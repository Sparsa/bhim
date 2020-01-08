BHIM=../../../build/bhim

echo "y" | $BHIM -h 2 -x Lcrit_clock_2.xml |tail -n 2
echo "y" | $BHIM -h 2 -x Lcrit_clock_3.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_clock_4.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_clock_5.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_clock_6.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_clock_7.xml |tail -n 1
