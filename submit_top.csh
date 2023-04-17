#!/bin/csh

#source /usr/local/sklib_gcc8/skofl_21b/env.csh
source /usr/local/sklib_gcc8/skofl_r30333/env.csh

set homeDIR = "/home/calib/uk_inj/backLog"
set backDIR = "/disk1/disk02/calib3/usr/ukli/backLog/"

set earlyRUN = 082801
#set earlyRUN = `ls -1 /disk02/calib3/usr/ukli/backLog/top_diffuser | head -1 | cut -d . -f2`
set latestRun = `ls -1 /disk02/calib3/usr/ukli/backLog/col/b1/ | tail -1 | cut -d . -f2`
@ latestRun += 1000 

set num = $earlyRUN
while ($num < $latestRun)

    if (-e "$homeDIR/top_diff/top_diffuser/top_diffuser.0$num" && ! -e "$backDIR/top_diffuser/top_diffuser.0$num.root") then
        echo "$backDIR/top_diffuser/top_diffuser.0$num.root"
        cp $homeDIR/top_diff/top_diffuser/top_diffuser.0$num $homeDIR
        /home/calib/uk_inj/zbs2root/example3c $homeDIR/top_diffuser.0$num
        yes | rm $homeDIR/top_diffuser.0$num
        mv $homeDIR/top_diffuser.0$num.root $backDIR/top_diffuser/
    endif
    @ num++
end
#echo $earlyRUN

