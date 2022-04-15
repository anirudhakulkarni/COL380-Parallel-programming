echo "#!/bin/bash
### Set the job name (for your reference)
#PBS -N ML-Aggressive-big-7
### Set the project name, your department code by default
#PBS -P col380.cs5190421
### Request email when job begins and ends
#PBS -m bea
### Specify email address to use for notification.
#PBS -M $USER@iitd.ac.in
#PBS -q high
####
#PBS -l select=10:ncpus=24
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=00:04:00

### #PBS -l software=replace_with_Your_software_name
# After job starts, must goto working directory. 
# $PBS_O_WORKDIR is the directory from where the job is fired. 
echo "==============================="
echo \$PBS_JOBID
cat \$PBS_NODEFILE
echo "==============================="
cd $PWD 
pwd
module load apps/anaconda/3
module load compiler/gcc/9.1.0 
module load compiler/gcc/9.1/openmpi/4.0.2
cd /scratch/cse/dual/cs5190421/2019CS50421
./bf.sh 10 24
$*" > temp.sh
qsub temp.sh 
rm temp.sh
# 5k data 7-8 epochs