#!/bin/sh

  submitscript=submitScriptT.sh
  submitscript1=submitScriptT_Expectation.sh

type=$1
outStr=$2

  mkdir -p qsub

if [ $type -eq 0 ]; then

  for TStr in t_top tW_top s_channel; do 

    njobs=`ls InputFiles_T/filelist_Spring15_ST_${TStr}_* | wc -l`
    #njobs=$[$njobs+1]
    echo number of jobs: $njobs
    #for i in `seq 1 $njobs`; do
      for i in `seq 0 $njobs`; do

        export filenum=$i
        export TStr=$TStr
        export outStr=$outStr
        echo $filenum
        qsub -l walltime=12:00:00 -N $TStr_$outStr -o qsub/ -e qsub/ -V $submitscript  

        sleep 1

      done

  done


  submitscript=submitScriptT.sh

  for TStr in t_antitop tW_antitop ; do                          

    njobs=`ls InputFiles_T/filelist_Spring15_ST_${TStr}_* | wc -l`
    #njobs=$[$njobs+1]
    echo number of jobs: $njobs
    #for i in `seq 1 $njobs`; do
      for i in `seq 0 $njobs`; do

        export filenum=$i
        export TStr=$TStr
        export outStr=$outStr
        echo $filenum
        qsub -l walltime=12:00:00 -N $TStr_$outStr -o qsub/ -e qsub/ -V $submitscript 

        sleep 1

      done

  done

fi

if [ $type -eq 1 ]; then

  for TStr in t_top tW_top t_antitop tW_antitop s_channel; do

    njobs=`ls InputFiles_T/filelist_Spring15_ST_${TStr}_* | wc -l`
    #njobs=$[$njobs+1]
    echo number of jobs: $njobs
    #for i in `seq 1 $njobs`; do
      for i in `seq 0 $njobs`; do

        export filenum=$i
        export TStr=$TStr
        export outStr=$outStr
        echo $filenum
        qsub -l walltime=12:00:00 -N $TStr_$outStr -o qsub/ -e qsub/ -V $submitscript1 

        sleep 1

      done

  done


fi

