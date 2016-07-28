#!/bin/sh

type=$1 # 0 prediction, 1 expectation
outStr=$2
submitscript=submitScriptSignal.sh


for m in 600 650 700 750 800 850 900 950 1000 1050 1100 1150 1200 1250 1300 1350 1400 1450 1500 1550 1600 1650 1700 1750 1800 1850 1900 1950 2000 2050 2100 2150 2200 2250 2300 ; do
#for m in 600 ; do

    for d in 1 100 200 300 350 375 400 425 450 475 500 525 550 575 600 625 650 675 700 725 750 775 800 825 850 875 900 925 950 975 1000 1025 1050 1075 1100 1125 1150 1175 1200 1225 1250 1275 1300 1325 1350 1375 1400 1425 1450 1475 1500 1525 1550 1575 1600 ; do 

	if [ "$d" -gt "$m" ] ; then
	    break
	fi

	export filename=RA2bin_T1tttt_${m}_${d}_fast
	export motherM=${m}
	export lspM=${d}

	njobs=`ls InputFiles_Signal/RA2bin_T1tttt_${m}_${d}_fast | wc -l`
	echo number of jobs: $njobs
	mkdir -p qsub


	if [ -e InputFiles_Signal/RA2bin_T1tttt_${m}_${d}_fast ]; then

	    export outStr=$outStr
	    
	    if [ $type -eq 0 ]; then
		if [ -e TauHad2Multiple/HadTauEstimation_T1tttt_mMother_${m}_mLSP_${d}_${outStr}_00.root ]; then
		    echo warning !
		    echo exist TauHad2Multiple/HadTauEstimation_T1tttt_mMother_${m}_mLSP_${d}_${outStr}_00.root
		else
		    echo submitting TauHad2Multiple/HadTauEstimation_T1tttt_mMother_${m}_mLSP_${d}_${outStr}_00.root
		     qsub -l walltime=12:00:00 -N T1tttt_mMother_${m}_mLSP_${d}_$outStr -o qsub/ -e qsub/ -V $submitscript  
		    
		fi
	    fi
	    
	    
	fi
	sleep 1

    done
done