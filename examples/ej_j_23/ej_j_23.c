#include "cqpc/cqpc.h"
#include <time.h>
#include "ej_j_23.h"


int main(int argc, char const *argv[])
{
    srand(clock());

    struct ej_j_23 c;
    
    int config_failure;
    if (config_failure = init_from_stdin(argc, argv, &c)) {
        return config_failure;
    }
    
    double money[c.n_agents];

    c.initial_money = c.total_money / c.n_agents;
    c.k_normalization = 1 / c.initial_money;
    

    cqpc_double_array_set_to(c.initial_money, money, c.n_agents);

    int t;
    for (t = 0; t < c.t_max; t++)
    {
        evolution(money, c.n_agents);
    }
    
    c.min = 0;
    if (c.max < c.min){
        c.max = cqpc_double_array_get_max_value(money, c.n_agents);
    }

    int hist_id;
    hist_id = cqpc_histogram_prep(c.min, c.max, c.n_bins);

    cqpc_histogram_fill(hist_id, c.n_agents, money);

    FILE *file;
    file = fopen(c.filename, "w");

    cqpc_histogram_dump_to_file_normalized(hist_id, file);

    fclose(file);
    return 0;
}
