#!/bin/sh

echo $PBS_O_WORKDIR
cd $PBS_O_WORKDIR
hostname
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#

echo InputFiles_Data/filelist_data_SingleMuon_${sample}_00$filenum
echo data_SingleMuon_${sample}_${outputStr}_$filenum

if [ $filenum -lt 10 ]
then
  ./run_tauHad2 "InputFiles_Data/filelist_data_SingleMuon_${sample}_00$filenum" "data_SingleMuon_${sample}_${outputStr}_$filenum" "TauHad2Multiple" "00" "0"
elif [ $filenum -lt 100 ]
then
  ./run_tauHad2 "InputFiles_Data/filelist_data_SingleMuon_${sample}_0$filenum" "data_SingleMuon_${sample}_${outputStr}_$filenum" "TauHad2Multiple" "00" "0"
else
  ./run_tauHad2 "InputFiles_Data/filelist_data_SingleMuon_${sample}_$filenum" "data_SingleMuon_${sample}_${outputStr}_$filenum" "TauHad2Multiple" "00" "0"
fi


