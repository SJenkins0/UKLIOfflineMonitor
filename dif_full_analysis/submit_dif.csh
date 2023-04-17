#!/bin/csh

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh

set homeDIR = "/home/calib/uk_inj/backLog"
set difDIR = "$homeDIR/dif_full_analysis"
set backDIR = "/disk1/disk02/calib3/usr/ukli/backLog/"

#set earlyRUN = `ls -1 /disk02/calib3/usr/ukli/backLog/dif/b1/ | head -1 | cut -d . -f2`
set earlyRUN = 082801
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | tail -1 | cut -d . -f2`
@ latestRun += 1000

foreach i (1 2 3 4 5)

    set num = $earlyRUN
    while ($num < $latestRun)

	if (-e "$homeDIR/zbs/dif/b${i}/ukld${i}.0$num" && ! -e "$backDIR/dif/b${i}/ukld${i}.0$num.root") then
	    echo "$backDIR/dif/b${i}/ukld${i}.0$num.root"
	    cp $homeDIR/zbs/dif/b${i}/ukld${i}.0$num $homeDIR
	    /home/calib/uk_inj/zbs2root/example3c $homeDIR/ukld${i}.0$num
	    yes | rm $homeDIR/ukld${i}.0$num
	    mv $homeDIR/ukld${i}.0$num.root $backDIR/dif/b${i}/
	endif
	@ num++
    end
    
end
#echo $earlyRUN

