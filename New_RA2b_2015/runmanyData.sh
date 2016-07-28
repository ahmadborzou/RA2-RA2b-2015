#!/bin/sh

sample=$1
outputStr=$2
submitscript=submitScriptData.sh

njobs=`ls InputFiles_Data/filelist_data_SingleMuon_${sample}_* | wc -l`

#njobs=$[$njobs+1]
echo number of jobs: $njobs
mkdir -p qsub
#for i in `seq 1 $njobs`; do
for i in `seq 0 $njobs`; do

export filenum=$i
export sample=$sample
export outputStr=$outputStr
echo $filenum
echo $code 

#qsub -l walltime=24:00:00 -N Data_${i} -o qsub/ -e qsub/ -V $submitscript 
qsub -l walltime=24:00:00 -N Data_$outputStr_${i} -o qsub/ -e qsub/ -V $submitscript
sleep 1

done

