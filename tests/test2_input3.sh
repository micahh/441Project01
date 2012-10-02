echo This is the third input for test 2
echo running sleep 10 and sleep 9 in background followed by ps -a
sleep 10 & sleep 9 & ps -a
echo running sleep 2
sleep 2 ;
echo invoking builtin jobs command
jobs
echo test 2 input 3 finished
exit 
echo this point is never reached
