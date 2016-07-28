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
      #echo InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_00$filenum
    ./run_tauHad2 "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_00$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
elif [ $filenum -lt 100 ]
then
    ./run_tauHad2 "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_0$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
else
#    echo InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_$filenum
    ./run_tauHad2 "InputFiles_TTbar/filelist_Spring15_TTJets_${TTbarStr}_$filenum" "TTbar_${TTbarStr}_${outStr}_$filenum" "TauHad2Multiple" "00" "0"
fi


