#!/bin/sh

echo $PBS_O_WORKDIR
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#


if [ $filenum -lt 10 ]
then
    ./run_tauHad "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_00$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
elif [ $filenum -lt 100 ]
then
    ./run_tauHad "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_0$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
else
    ./run_tauHad "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHadMultiple" "00" "0"
fi


