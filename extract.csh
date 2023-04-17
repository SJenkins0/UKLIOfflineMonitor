#!/bin/csh

set runDIR = "/home/calib/uk_inj/backLog"
set dataDIR = "/disk02/calib3/usr/ukli/backLog/col"

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

set exec = "$runDIR/scripts/monitor"

set analysedRun = `tail -1 $runDIR/extracted/col_B1.dat | cut -c 1-5`
#set analysedRun = 82800
@ analysedRun++
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | tail -1 | cut -d . -f2`
#echo $latestRun
@ latestRun++
#echo $latestRun

foreach inj (1 2 3 4 5)

    set run = $analysedRun
    while ($run < $latestRun)
	if (-e "$dataDIR/b${inj}/uklc${inj}.0${run}.root") then 
	    echo $run
	    $exec -f $dataDIR/b${inj}/uklc${inj}.0${run}.root -c -i B${inj} -o $runDIR/extracted/col_0${run}_B${inj}.dat
	endif
	@ run++
    end
end



