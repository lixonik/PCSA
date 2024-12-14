## MPI
### Installation
```sh
sudo apt update
sudo apt install openmpi-bin openmpi-common libopenmpi-dev
```

### Сompilation

```sh
mpicxx -o app ./src/0301_3_4.cpp
```

### Execution

```sh
mpirun -np 4 ./app
```
## OpenMP

### Сompilation

```sh
g++ -fopenmp ./src/0301_3_5.cpp -o app
```

### Execution

```sh
./app
```
