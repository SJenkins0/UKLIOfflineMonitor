#!/bin/csh

set runDIR = "/home/calib/uk_inj/backLog"
set dataDIR = "/disk02/calib3/usr/ukli/backLog/top_diffuser"

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

set exec = "$runDIR/top_diff/top_diff"

#set analysedRun = 082801
#set analysedRun = 90619
set analysedRun = `tail -1 $runDIR/extracted/top_diffuser.dat | cut -c 1-5`
@ analysedRun++
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/top_diffuser/ | tail -1 | cut -d . -f2`
#echo $latestRun
@ latestRun++
#echo $latestRun


set run = $analysedRun
while ($run < $latestRun)
    if (-e "$dataDIR/top_diffuser.0${run}.root") then 
        echo $run
        $exec -f $dataDIR/top_diffuser.0${run}.root -o $runDIR/extracted/top_diffuser_0${run}.dat
    endif
    @ run++
end




