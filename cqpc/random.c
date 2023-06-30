#ifndef _RANDOM_C_INCLUDED
#define _RANDOM_C_INCLUDED

#ifndef _RANDOM_H_INCLUDED
#include "random.h"
#endif


#define normaRandom (2.3283063671E-10F)
#define random (rand() / ((double)__INT_MAX__ + 1))

double cqpc_random_standard_rand() {
    return random;
}

double cqpc_random_parisi_rapuano() {
    static unsigned int irr[256];
    static unsigned char ig1, ig2, ig3, ind_Ran;
    static unsigned short initialized = 0;

    if (!initialized) {
        initialized = 1;
        int i;
        for (i = 0; i < 256; i++)
        {
            irr[i] = pow(2, 32) * random;
        }
        ind_Ran = ig1 = ig2 = ig3 = 0;
    }

    ig1 = ind_Ran - 24;
    ig2 = ind_Ran - 55;
    ig3 = ind_Ran - 61;
    irr[ind_Ran] = irr[ig1] + irr[ig2];

    return (irr[ind_Ran++] ^ irr[ig3]) * normaRandom;
}

double cqpc_random_uniform(double min, double max, double (*random_function)(void)) {
    return (max - min) * (*random_function)() + min;
}

double cqpc_random_box_muller()
{
    static unsigned short initialized = 0;
    static unsigned short acum = 0;
    static double epsilon = __DBL_EPSILON__;
    static double two_pi;
    static double X, W;
    
    if (!initialized) { 
        two_pi = 2.0 * acos(-1);
        initialized = 1;
    } 

    if (acum) {
        acum = 0;
        return W;
    }
    
    double u1, u2;    
    do
    {
        u1 = cqpc_random_parisi_rapuano();
    }
    while (u1 <= epsilon);
    u2 = cqpc_random_parisi_rapuano();

    double mag =  sqrt(-2.0 * log(u1));
    X  = mag * cos(two_pi * u2);
    W  = mag * sin(two_pi * u2);
    
    acum = 1;
    return X;
}

double cqpc_random_gaussian(double mean, double sigma) {
    return mean + sigma * cqpc_random_box_muller();
}


#undef normaRandom
#undef random

#endif //_RANDOM_C_INCLUDED