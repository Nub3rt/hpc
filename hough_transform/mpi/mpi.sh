#!/bin/bash

placement=
watch=0
print_time=0
print_lines=0

usage() {
  echo "usage: $0 <nodes> <img_path> <theta_multiplier> [-p <placement_strategy>] [-w <cores>] [-t] [-l]"
  echo "  nodes             : number of nodes (mandatory)"
  echo "  img_path          : image path (mandatory)"
  echo "  theta_multiplier  : theta multiplier value (mandatory)"
  echo "  -p <place>        : PBS placement strategy"
  echo "  -w <cores>        : watch job after submission"
  echo "  -t                : executable prints time (this or -l)"
  echo "  -l                : executable prints lines (this or -t)"
  echo
  exit 1
}

if [ $# -lt 3 ]; then
  usage
fi

nodes=$1
img_path=$2
theta_multiplier=$3
shift 3

while getopts "p:w:tl" opt; do
  case $opt in
    p)
      placement="-l place=$OPTARG"
      ;;
    w)
      watch="$OPTARG"
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
mkdir -p "../${name}_out"

basepath=$(basename $img_path)
edges=${basepath//[!0-9]/}


if [[ "$name.c" -nt "$name.out" ]]; then
  module load -s OpenMPI/4.1.1-GCC-11.2.0
  mpicc -lm -O2 -Wall "$name.c" -o "$name.out" || { echo "Compilation failed, exiting..."; exit 1; }
fi


run() {
  job_script=$(mktemp)
  cat > "$job_script" <<EOS
#!/bin/bash

#PBS -l select=$nodes:ncpus=$cores:mpiprocs=$cores:mem=8gb $placement
#PBS -l walltime=0:30:00
#PBS -q shortCPUQ

#PBS -o ../${name}_out/${name}.o.${nodes}x${cores}x${edges}x${theta_multiplier}
#PBS -e ../${name}_out/${name}.e.${nodes}x${cores}x${edges}x${theta_multiplier}

cd \$PBS_O_WORKDIR

module load -s OpenMPI/4.1.1-GCC-11.2.0
mpiexec \
    --hostfile \$PBS_NODEFILE \
    -n $total_cores           \
    --map-by node             \
    --bind-to core            \
    --mca pml ob1             \
    --mca btl tcp,self        \
    --mca btl_tcp_if_include 192.168.108.0/23 \
    "./$name.out" $img_path $theta_multiplier $print_time $print_lines

EOS

  job_id=$(qsub "$job_script")
  rm -f "$job_script"
  echo "$job_id"
}


if [[ "$watch" -ne 0 ]]; then
  cores="$watch"
  total_cores=$((nodes * cores))
  job_id=$(run)
  watch "qstat -f $job_id"
else
  for cores in 16 8 4 2 1; do
    total_cores=$((nodes * cores))
    if [[ $total_cores -le 16 ]]; then
      job_id=$(run)
      echo    "$nodes $cores $edges $theta_multiplier"
      echo -e "\t$job_id"
    fi
  done
fi
