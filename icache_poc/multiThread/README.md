# I Cache PoC
This PoC demonstrates how limited Reservation Station entries can lead to
a timing channel attack using the I-Cache.

## Build
`make`

We need to add the current directory to `LD_LIBRARY_PATH` so that the shared
libraries are used: `export LD_LIBRARY_PATH=$PWD`

The built binary can be run on the same core (with SMT enabled) or cross cores.

## Run
Usage: `./multiThreadIC [mode] [training_loops]`
```
mode: S - The PoC runs on same physical cores (1,5)
      C - The PoC runs on different physical cores (1,2)

training_loops: program parameter which determines how many times a branch
is trained to ensure misprediction for N+1th run. A large value guarantees
misprediction but delays the bit leak period since the branches are trained for
each bit leak. A smaller value leaks the bits faster but with lower accuracy.
Default value: 1000000
```

This PoC is developed for a 8 core (4 Physical cores) machine, so the numbers
need to be changed if the mappings are different.

## Scripts
The repository contains 2 scripts to run the attack at various bit-rate leaks
and to visualize the efficiency. The `script.sh` runs the attack at various
`training_loop` values and the `parse.py` script creates a bit error probability 
against bit rates csv which is used by the `pareto.py` to generate the pareto
curves.

The scripts are run as follows:
```
$> bash script.sh > out

$> python3 parse.py > icache

$> python3 pareto.py
```
