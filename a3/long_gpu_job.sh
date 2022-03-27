echo "#!/bin/bash
### Set the job name (for your reference)
#PBS -N ML-Aggressive-big-7
### Set the project name, your department code by default
#PBS -P workshop
### Request email when job begins and ends
#PBS -m bea
### Specify email address to use for notification.
#PBS -M $USER@iitd.ac.in
#PBS -q high
####
#PBS -l select=1:ncpus=4:ngpus=1:centos=skylake
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=06:00:00

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

python3 final_v2.py
$*" > temp.sh
qsub temp.sh 
# rm temp.sh
# 5k data 7-8 epochs