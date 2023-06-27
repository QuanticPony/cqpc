
#ifndef EJ_J_23_INCLUDED
#define EJ_J_23_INCLUDED

#include "cqpc/cqpc.h"

struct ej_j_23
{
    size_t n_agents;
    int n_bins, t_max;
    
    double max, min, initial_money, total_money, k_normalization;
    char filename[250];
};

/** Creates a configuration for the given standard input*/
int init_from_stdin(int argc, char const *argv[], struct ej_j_23 *configuration){
    configuration->max = __DBL_MIN__;
    switch (argc)
    {
    case 7:
        sscanf(argv[6],"%lf", &(configuration->max));
    case 6:
        sscanf(argv[5],"%s", configuration->filename);
    case 5:
        sscanf(argv[1],"%ld", &(configuration->n_agents));
        sscanf(argv[2],"%d", &(configuration->n_bins));
        sscanf(argv[3],"%d", &(configuration->t_max));
        sscanf(argv[4],"%lf", &(configuration->total_money));
        break;

    default:
        printf("How to use:\n");
        printf("%s n_agents n_bins t_max total_money [filname] [histogram_max]\n", argv[0]);
        printf("Example:\n");
        printf("%s %d %d %d %d\n", argv[0], 1000, 75, 10000, 50000);
        printf("%s %d %d %d %d %s\n", argv[0], 1000, 75, 10000, 50000, "duck.txt");
        printf("%s %d %d %d %d %s %d\n", argv[0], 1000, 75, 10000, 50000, "other_duck.txt", 400);
        return 1;
    }

    if (argc == 5) {
        sprintf(configuration->filename, "%s_output.txt", argv[0]);
    }
    return 0;
}

/* Returns a random agent*/
size_t random_agent(size_t n_agents){
    return (size_t) cqpc_random_random_uniform(0, n_agents, cqpc_random_parisi_rapuano);
}

/* Returns a new agent to make a transaction with  */
size_t choose_agent(size_t agent, size_t n_agents){
    size_t new_agent, i;
    i = 0;
    do
    {   
        new_agent = random_agent(n_agents);
    } while (agent == new_agent && i < 5000);
    return new_agent;
}

/** Returns transaction direction
 * 0 - transaction i to j
 * 1 - transaction j to i
*/
int transaction_direction(size_t agent_i, size_t agent_j, double* money) {
    return (int) cqpc_random_random_uniform(0, 2, cqpc_random_parisi_rapuano);
}

/* Returns a transaction amount from "agent_i" to "agent_j"*/
double transaction_amount_from_to(size_t agent_i, size_t agent_j, double *money){
    return 1;
}

/* Applies a random transaction to every agent*/
void evolution(double* money, size_t n_agents){

    size_t i, agent_i, agent_j;
    int transaction_direction_i_to_j;
    double transaction_amount;

    double tmp_money[n_agents];
    cqpc_double_array_copy_to(money, tmp_money, n_agents);

    for (i = agent_i = 0; i < n_agents; agent_i=++i)
    {
        agent_j = choose_agent(agent_i, n_agents);

        transaction_direction_i_to_j = transaction_direction(agent_i, agent_j, money);
        if (!transaction_direction_i_to_j) {
            agent_i = agent_j;
            agent_j = i;
        }

        transaction_amount = transaction_amount_from_to(agent_i, agent_j, money);

        if (money[agent_i] == 0) 
            continue;

        if (money[agent_i] < transaction_amount) {
            transaction_amount = money[agent_i];
        }
        tmp_money[agent_j] += transaction_amount;
        tmp_money[agent_i] -= transaction_amount;
    }
    cqpc_double_array_copy_to(tmp_money, money, n_agents);
}

#endif