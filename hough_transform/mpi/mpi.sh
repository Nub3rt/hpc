#!/bin/bash

watch=0
print_time=0
print_lines=0

usage() {
    echo "usage: $0 <cores> <img_path> <theta_multiplier> [-w] [-t] [-l]"
    echo "  cores             : number of cores (mandatory)"
    echo "  img_path          : image path (mandatory)"
    echo "  theta_multiplier  : theta multiplier value (mandatory)"
    echo "  -w                : watch job after submission"
    echo "  -t                : executable prints time (this or -l)"
    echo "  -l                : executable prints lines (this or -t)"
    echo
    exit 1
}

if [ $# -lt 3 ]; then
    usage
fi

cores=$1
img_path=$2
theta_multiplier=$3
shift 3

while getopts "wtl" opt; do
    case $opt in
        w)
            watch=1
            ;;
        t)
            print_time=1
            ;;
        l)
            print_lines=1
            ;;
        *)
            usage
            ;;
    esac
done


if [[ ! -f "${img_path}" ]]; then
    echo "couldn't find $img_path"
    exit 2
fi


name=$(basename $0 .sh)

basepath=$(basename $img_path)
edges=${basepath//[!0-9]/}

job_id=$(cat <<EOS | qsub
#!/bin/bash

#PBS -l select=1:ncpus=$cores:mem=8gb
#PBS -l walltime=0:10:00
#PBS -q shortCPUQ

#PBS -o ../mpi_out/${name}.o.${cores}x${edges}x${theta_multiplier}
#PBS -e ../mpi_out/${name}.e.${cores}x${edges}x${theta_multiplier}

cd \$PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
if [[ "$name.c" -nt "$name.out" ]]; then
  mpicc -lm -Wall "$name.c" -o "$name.out" || { echo "Compilation failed, exiting..."; exit 1; }
fi

mpirun -n $cores "./$name.out" $img_path $theta_multiplier $print_time $print_lines

EOS
)

if [[ "$watch" -ne 0 ]]; then
  watch "qstat -f $job_id"
else
  echo $job_id
fi
