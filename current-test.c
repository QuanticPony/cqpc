#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include "cqpc/random.h"
#include "cqpc/histogram.h"
#include "cqpc/arrays.h"

int main()
{
    int i, j;

    double a[200];

    empty_double_array(a, 200);
    set_double_array_to(5., a, 200);

    int histogram1_id, histogram2_id;

    histogram1_id = prep_histogram(0., 1., 10);
    histogram2_id = prep_histogram(1., 2., 10);

    double array[500];

    for (j = 0; j < 100; j++)
    {
        for (i = 0; i < 500; i++)
        {
            array[i] = random_uniform(0.5,1.5, parisi_rapuano);
        }    

        double min, max;
        fill_histogram(histogram1_id, 500, array);
        fill_histogram(histogram2_id, 500, array);
    }

    double *histogram1, *histogram2;
    histogram1 = get_histogram(histogram1_id);
    histogram2 = get_histogram(histogram2_id);

    for (i = 0; i < 10; i++)
    {
        printf("%f\t%f\n", histogram1[i], histogram2[i]);
    }
    
    FILE *file;
    FILE *file2;


    file = fopen("histogram1.txt", "w");
    dump_to_file(histogram1_id, file);
    fclose(file);


    file = fopen("histogram1.txt", "r");
    histogram2_id = load_from_file(file);
    fclose(file);

    
    file2 = fopen("histogram2.txt", "w");
    dump_to_file(histogram2_id, file2);
    fclose(file2);

    clean_histograms();
}
