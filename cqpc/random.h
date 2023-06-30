#ifndef _RANDOM_H_INCLUDED
#define _RANDOM_H_INCLUDED

#include <stdlib.h>
#include <math.h>

/** Random uniform generator [0,1) using <math.h> rand()*/
double cqpc_random_standard_rand();

/** Random uniform generator [0,1) using Parisi Rapuano method*/
double cqpc_random_parisi_rapuano();

/** Random uniform generator [min, max) using given random funciton
 * * min: left boundary of uniform distribution
 * * max: right boundary of uniform distribution
 * * random_funciton: random uniform generator [0,1)
*/
double cqpc_random_uniform(double min, double max, double (*random_function)(void));


/** Random normal generator using Box Muller method */
double cqpc_random_box_muller();

/** Random gaussian generator using Box Muller method
 * * mean: mean of random distribution
 * * sigma: standard deviation
*/
double cqpc_random_gaussian(double mean, double sigma);

#ifndef _RANDOM_C_INCLUDED
#include "random.c"
#endif

#endif //_RANDOM_H_INCLUDED
