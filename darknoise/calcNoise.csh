#!/bin/csh

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

set runDIR = "/home/calib/uk_inj/backLog"
set dataDIR = "/disk02/calib3/usr/ukli/backLog/col/b1"
set exec = "$runDIR/darknoise/noise"

set analysedRun = `tail -1 $runDIR/darknoise/darkRate.dat | cut -c 1-5`
#set analysedRun = 82801
@ analysedRun++
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | tail -1 | cut -d . -f2 | cut -c 2-6`
echo $latestRun
@ latestRun++
#set latestRun = 90500


set run = $analysedRun
while ($run < $latestRun)
    #check one file exists for this
    if (-e "$dataDIR/uklc1.0${run}.root") then 
	echo $run
	$exec -r $run
    endif
    @ run++
end

cat darkRate_recent.dat >> darkRate.dat
\rm -f darkRate_recent.dat


##Make the plots
./plot

##Move these plots to the steering meeting slide directory
\cp -vf *.png /home/calib/uk_inj/steering/slides/images/


