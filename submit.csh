#!/bin/csh


#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh

set homeDIR = "/home/calib/uk_inj/backLog"
set backDIR = "/disk1/disk02/calib3/usr/ukli/backLog/"


set earlyRUN = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | head -1 | cut -d . -f2`
#set earlyRUN = 082801
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | tail -1 | cut -d . -f2`
@ latestRun += 10000

foreach i (1 2 3 4 5)

	set num = $earlyRUN
	while ($num < $latestRun)

	    if (-e "$homeDIR/zbs/col/b${i}/uklc${i}.0$num" && ! -e "$backDIR/col/b${i}/uklc${i}.0$num.root") then
		echo "$backDIR/col/b${i}/uklc${i}.0$num.root"
		cp $homeDIR/zbs/col/b${i}/uklc${i}.0$num $homeDIR
		/home/calib/uk_inj/zbs2root/example3c $homeDIR/uklc${i}.0$num
		yes | rm $homeDIR/uklc${i}.0$num
		mv $homeDIR/uklc${i}.0$num.root $backDIR/col/b${i}/
	    endif
	    @ num++
	end

end
#echo $earlyRUN
