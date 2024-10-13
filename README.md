## MPI

```
sudo apt update
sudo apt install openmpi-bin openmpi-common libopenmpi-dev
```

## Сompilation

```
mpicxx -o main main.cpp
```

## Execution

```
mpirun -np 4 ./main
```
