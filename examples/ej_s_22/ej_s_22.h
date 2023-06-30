#ifndef EJ_S_22_INCLUDED
#define EJ_S_22_INCLUDED

struct fleet
{
    double total_force;
    size_t ships;
    double ship_force;
    double damage_capavility;
    double damage_spread;
    double defensive_capavility;
    double damage_resistance;
};


struct ej_s_22
{
    struct fleet A, B;
    char filename[250];
};

/** Creates a configuration for the given standard input*/
int init_from_stdin(int argc, char const *argv[], struct ej_s_22 *c){
    switch (argc)
    {
    case 4:
        sscanf(argv[3],"%s", c->filename);
    case 3:
        sscanf(argv[1],"[%ld, %lf, %lf, %lf, %lf, %lf]", &(c->A.ships), &(c->A.ship_force), &(c->A.damage_capavility), &(c->A.damage_spread), &(c->A.defensive_capavility), &(c->A.damage_resistance));
        sscanf(argv[2],"[%ld, %lf, %lf, %lf, %lf, %lf]", &(c->B.ships), &(c->B.ship_force), &(c->B.damage_capavility), &(c->B.damage_spread), &(c->B.defensive_capavility), &(c->B.damage_resistance));
        break;

    default:
        printf("How to use:\n");
        printf("%s ",  argv[0]); 
        printf("\"[number_ships, ship_power, damage_capavility, damage_spread, defensive_capavility, damage_resistance]\" ");
        printf("\"[ships, pwr, dmg, sprd, def, res]\" ");
        printf("[filname]\n");
        printf("Example:\n");
        printf("%s ", argv[0]);
        printf("\"[%ld, %lf, %lf, %lf, %lf, %lf]\" ", 230L, 320., 40., 10., 15., 300.);
        printf("\"[%ld, %lf, %lf, %lf, %lf, %lf]\"\n", 400L, 110., 30., 5., 15., 500.);
        return 1;
    }

    if (argc == 2) {
        sprintf(c->filename, "%s_output.txt", argv[0]);
    }
    return 0;
}

double random_attack_power(struct fleet *from) {
    return cqpc_random_gaussian(from->damage_capavility, from->damage_spread);
}

double attack(struct fleet *from, struct fleet *to) {
    double damage = random_attack_power(from) * from->total_force;
    double defence = to->defensive_capavility * to-> total_force;
    return (damage - defence) / from->damage_resistance;
}


int evolution(struct ej_s_22 *c){
    double delta_A, delta_B;
    delta_A = attack(&(c->B), &(c->A));
    delta_B = attack(&(c->A), &(c->B));

    c->A.total_force -= delta_A;
    c->B.total_force -= delta_B;

    if (c->A.total_force < 0 || c->B.total_force < 0) {
        return 1;
    }
    return 0;   
}

#endif