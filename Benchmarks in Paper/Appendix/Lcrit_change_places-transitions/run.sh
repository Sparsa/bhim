BHIM=../../../build/bhim

echo "y" | $BHIM -h 2 -x Lcrit_6_10.xml |tail -n 2
echo "y" | $BHIM -h 2 -x Lcrit_7_11.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_8_12.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_9_13.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_10_14.xml |tail -n 1
echo "y" | $BHIM -h 2 -x Lcrit_11_15.xml |tail -n 1
