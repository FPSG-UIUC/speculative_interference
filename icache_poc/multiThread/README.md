# I Cache PoC
This PoC demonstrates how limited Reservation Station entries can lead to
a timing channel attack using the I-Cache.

## Build
`make -Dtraining_loops=1000000`
training_loops is a program parameter which determines how many times a branch
is trained to ensure misprediction for N+1th run. A large value guarantees
misprediction but delays the bit leak period since the branches are trained for
each bit leak. A smaller value leaks the bits faster but with lower accuracy.

We need to add the current directory to `LD_LIBRARY_PATH` so that the shared
libraries are used: `export LD_LIBRARY_PATH=$PWD`

## Run
The built binary can be run on the same core (with SMT enabled) or cross cores.
`./multiThreadIC S` runs it on cores 1 and 5 (which maps to the same physical
core).
`./multiThreadIC C` runs it on cores 1 and 2.
This PoC is developed for a 8 core (4 Physical cores) machine, so the numbers
need to be changed if the mappings are different.
