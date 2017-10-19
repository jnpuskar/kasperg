# kasperg

Backtrack solver of the IQLinks puzzle

The project is dedicated to solving the IQLink puzzle. I received this game as a Christmas gift and soon realized 
it will be more fun to solve programatically rather than manually. Finding the proper parametrization of the game 
was not trivial though. At the end I came up with the hexagonal model that implements rotation of pieces using addition 
in modulo 6.



I explored several algorithms to do the job. From elegant(but dead slow) recursive back-track running on 1 CPU thread over 
attempt to employ more sophisticated heuristics (too expensive to compute for each step) to end up with embedded loops using 
fast bitwise operators for state matching that use all threads of given CPU. The last algorithm was then ported to run on 
NVIDA GPU card using CUDA SDK. The quest ends by running the code on several hundreds of Amazon GPU instances (each having 
1,536 CUDA cores).

As this game belongs to the NP class of computational problems the state space grows exponentially, soon reaching dimensions 
that cannot be tackled by any computing means. Even for a decent size of 12 pieces on 24 pins we observe high numbers of 
iterations and any unnecessary operation implies a major time delay.

See Wiki for more
