#!/bin/sh
#cd /home/borzou/CMSSW_7_4_0_pre1/src/RA2-RA2b-2015/New_RA2b_2015
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#


  if [ $filenum -lt 10 ]
  then
    ./run_tauHad "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_00$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
elif [ $filenum -lt 100 ]
then
    ./run_tauHad "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_0$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
  else
    ./run_tauHad "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
  fi


