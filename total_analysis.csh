#!/bin/csh
set HOMEDIR = "/home/calib/uk_inj/backLog"

source $HOMEDIR/submit.csh
source $HOMEDIR/extract.csh
source $HOMEDIR/extracted/combine.csh


source $HOMEDIR/submit_dif.csh
source $HOMEDIR/extract_dif.csh
source $HOMEDIR/extracted/combine_dif.csh


source $HOMEDIR/submit_top.csh
source $HOMEDIR/extract_top.csh
source $HOMEDIR/extracted/combine_top.csh


#source $HOMEDIR/extracted/copy.csh
cd $HOMEDIR/darknoise
source calcNoise.csh
cd $HOMEDIR
