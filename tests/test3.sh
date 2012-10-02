/bin/invalid_program param1 param2
echo Second Job
pwd ; pwd ; sleep 1 &
jobs
/bin/invalid_program &
jobs
sleep 1 ; jobs
exit


