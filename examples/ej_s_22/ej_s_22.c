#include <cqpc.h>
#include <time.h>
#include "ej_s_22.h"


int main(int argc, char const *argv[])
{
    srand(clock());

    struct ej_s_22 c;
    
    int config_failure;
    if (config_failure = init_from_stdin(argc, argv, &c)) {
        return config_failure;
    }

    c.A.total_force = c.A.ships * c.A.ship_force;
    c.B.total_force = c.B.ships * c.B.ship_force;

    long iterations;
    while(!evolution(&c)) {
        iterations++;
        // If something is needed between attacks
    };
    
    if (c.A.total_force < c.B.total_force) {
        printf("B");
    } else {
        printf("A");
    }
    printf(" won after %ld iterations\n", iterations);
    return 0;
}
