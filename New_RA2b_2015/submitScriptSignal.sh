#!/bin/sh

echo $PBS_O_WORKDIR
cd $PBS_O_WORKDIR
source ~cmssoft/shrc
eval `scramv1 runtime -sh`
#


./run_tauHad2 "InputFiles_Signal/${filename}" "${filename}-${outStr}" "TauHad2Multiple" "00" "0"
#echo "submitScript " InputFiles_Signal/${filename} T1tttt_mMother_${m}_mLSP_${d}_${outStr} 
#./run_tauHad2 "InputFiles_Signal/${filename}" "T1tttt_mMother_${m}_mLSP_${d}_${outStr}" "TauHad2Multiple" "00" "0"
#./run_tauHad2 "InputFiles_Signal/${filename}" "${filename}_${outStr}" "TauHad2Multiple" "00" "0"