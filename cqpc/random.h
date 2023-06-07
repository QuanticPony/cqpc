#ifndef _RANDOM_H_INCLUDED
#define _RANDOM_H_INCLUDED

#include <math.h>

/** Random uniform generator [0,1) using <math.h> rand()*/
double standard_rand();

/** Random uniform generator [0,1) using Parisi Rapuano method*/
double parisi_rapuano();

/** Random uniform generator [min, max) using given random funciton
 * * min: left boundary of uniform distribution
 * * max: right boundary of uniform distribution
 * * random_funciton: random uniform generator [0,1)
*/
double random_uniform(double min, double max, double (*random_function)(void));

#ifndef _RANDOM_C_INCLUDED
#include "random.c"
#endif

#endif //_RANDOM_H_INCLUDED
