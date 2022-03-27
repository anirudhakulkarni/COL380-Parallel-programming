export OMP_NUM_THREADS=$SLURM_CPUS_PER_TASK 
mpirun --bind-to none ./main $2 $1 $3 $4
