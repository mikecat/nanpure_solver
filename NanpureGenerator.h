#ifndef NANPURE_GENERATOR_H_GUARD_400D3448_EC23_441A_A389_2A42A5DD066C
#define NANPURE_GENERATOR_H_GUARD_400D3448_EC23_441A_A389_2A42A5DD066C

#include "NanpureBoard.h"

/* rng : return random integer in [0, range) */
NanpureBoard generateNanpure(int (*rng)(int range));

#endif
