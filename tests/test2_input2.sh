echo This is the second input for test 2
echo running sleep 100 in the background
sleep 100 &
echo running killall sleep 
killall -9 sleep
echo issuing buil-tin jobs command
jobs
echo test 2 input 2 finished, calling exit
exit 

