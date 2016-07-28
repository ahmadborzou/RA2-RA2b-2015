#!/bin/sh

echo $PBS_O_WORKDIR
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#module load root
#

if [ $filenum -lt 10 ]
then
    ./run_tauHad2 "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_00$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
elif [ $filenum -lt 100 ]
then
    ./run_tauHad2 "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_0$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
else
    ./run_tauHad2 "InputFiles_WJet_${WJetStr}/filelist_Spring15_WJet_HT_${WJetStr}_$filenum" "WJet_${WJetStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
fi



