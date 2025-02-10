#!/bin/csh

set runDIR="/home/calib/uk_inj/backLog"
set DIFFILESTUFF="$runDIR/extracted"

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
#source /usr/local/sklib_gcc8/skofl_r30333/env.csh
source /usr/local/sklib_gcc8/skofl_r31719/env.csh
source /usr/local/sklib_gcc8/root_v6.22.06_python3.6/bin/thisroot.csh

set analysedRun = `tail -1 $runDIR/extracted/dif_B1.dat | cut -c 1-5`
#set analysedRun = 82800
@ analysedRun++
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/dif/b1/ | tail -1 | cut -d . -f2`
#echo $latestRun
@ latestRun++
#echo $latestRun



foreach inj (1 2 3 4 5)

    set run = $analysedRun
    while ($run < $latestRun)
	
	if (-e "$DIFFILESTUFF/dif_0${run}_B${inj}.dat") then
	    echo "$DIFFILESTUFF/dif_0${run}_B${inj}.dat"
	    cat $DIFFILESTUFF/dif_0${run}_B${inj}.dat >> $DIFFILESTUFF/dif_B${inj}.dat
	endif
	@ run++
    end
    yes | rm $DIFFILESTUFF/dif_0*_B${inj}.dat
end

