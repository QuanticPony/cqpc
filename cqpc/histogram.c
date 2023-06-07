#ifndef _HISTOGRAM_C_INCLUDED
#define _HISTOGRAM_C_INCLUDED

#ifndef _HISTOGRAM_H_INCLUDED
#include "histogram.h"
#endif

static int __histograms_size;
static short *__histogram_prep;
static long *__histogram_count;
static long **__histograms_cumulative;
static double **__histograms_return;
static long *__histogram_nbins;
static double *__histogram_delta;
static double *__histogram_min;
static double *__histogram_max;

int prep_histogram(int nbins, double max, double min)
{
    if (max < min)
    {
        double tmp;
        tmp = max;
        max = min;
        min = tmp;
    }

    int i;
    int histogram_position;
    histogram_position = __histograms_size;
    // Check if we can use a previous histogram position
    for (i = 0; i < __histograms_size; i++)
    {
        if (__histogram_prep[i])
            continue;
        histogram_position = i;
        break;
    }

    // If not, realloc to increase size
    if (histogram_position == __histograms_size) {
        __histograms_size++;
#define realloc_ptr(ptr, typ) ptr = (typ *)realloc(ptr, __histograms_size * sizeof(typ))
        realloc_ptr(__histograms_cumulative, long *);
        realloc_ptr(__histograms_return, double *);
        realloc_ptr(__histogram_nbins, long);
        realloc_ptr(__histogram_max, double);
        realloc_ptr(__histogram_min, double);
        realloc_ptr(__histogram_delta, double);
        realloc_ptr(__histogram_count, long);
        realloc_ptr(__histogram_prep, short);
#undef realloc_ptr
    }

    __histograms_cumulative[histogram_position] = (long *)calloc(nbins, sizeof(long));

    __histogram_nbins[histogram_position] = nbins;
    __histogram_max[histogram_position] = max;
    __histogram_min[histogram_position] = min;
    __histogram_delta[histogram_position] = (max - min) / ((double)nbins);
    __histogram_prep[histogram_position] = 1;
    __histogram_count[histogram_position] = 0;

    return histogram_position;
}

void fill_histogram(int histogram_id, int nvalues, double *values)
{
    if (histogram_id >= __histograms_size)
        return;

    int i, index;
    double value, min, max, delta;
    long *histogram;
    
    min = __histogram_min[histogram_id];
    max = __histogram_max[histogram_id];
    delta = __histogram_delta[histogram_id];
    histogram = __histograms_cumulative[histogram_id];

    for (i = 0; i < nvalues; i++)
    {
        value = values[i];
        if (value < min ||
            value > max)
            continue;

        index = (value - min) / delta;

        histogram[index]++;
    }
    __histogram_count[histogram_id] += nvalues;
}

double *get_histogram(int histogram_id)
{
    if (histogram_id >= __histograms_size)
        return (double*) 0;

    int i;
    double *histogram;
    double k_normalization = 1 / (__histogram_count[histogram_id] * __histogram_delta[histogram_id]);

    __histograms_return[histogram_id] = (double *)malloc(__histogram_nbins[histogram_id] * sizeof(double));
    histogram = __histograms_return[histogram_id];
    
    long *histogram_ref =  __histograms_cumulative[histogram_id];
    for (i = 0; i < __histogram_nbins[histogram_id]; i++)
    {
        histogram[i] = k_normalization * histogram_ref[i];
    }

    return histogram;
}

void clean_histogram(int histogram_id)
{
    if (histogram_id >= __histograms_size)
        return;

    if (__histogram_prep[histogram_id])
    {
        free(__histograms_cumulative[histogram_id]);
        free(__histograms_return[histogram_id]);
        __histogram_count[histogram_id] = 0;
        __histogram_nbins[histogram_id] = 0;
        __histogram_max[histogram_id] = 0;
        __histogram_min[histogram_id] = 0;
        __histogram_delta[histogram_id] = 0.;
        __histogram_prep[histogram_id] = 0;        
    }
}

void clean_histograms() {
    int i;

    for (i = 0; i < __histograms_size; i++)
    {
        clean_histogram(i);
    }
}

int load_from_file(FILE *file) {
    int histogram_id, i;
    double max, min, delta;
    long nbins, count;
    
    fscanf(file, "// nbins=%ld\n", &nbins);
    fscanf(file, "// min=%lf\n", &min);
    fscanf(file, "// max=%lf\n", &max);
    fscanf(file, "// delta=%lf\n", &delta);
    fscanf(file, "// count=%ld\n", &count);

    histogram_id = prep_histogram(nbins, max, min);

    long read_value;

    for (i = 0; i < nbins; i++)
    {
        fscanf(file, "%ld\n", &read_value);
        __histograms_cumulative[histogram_id][i] = read_value;
    }
    __histogram_count[histogram_id] = count;

    return histogram_id;
}

void dump_to_file(int histogram_id, FILE *file) {
    int i;

    fprintf(file, "// nbins=%ld\n", __histogram_nbins[histogram_id]);
    fprintf(file, "// min=%lf\n", __histogram_min[histogram_id]);
    fprintf(file, "// max=%lf\n", __histogram_max[histogram_id]);
    fprintf(file, "// delta=%f\n", __histogram_delta[histogram_id]);
    fprintf(file, "// count=%ld\n", __histogram_count[histogram_id]);

    for (i = 0; i < __histogram_nbins[histogram_id]; i++)
    {
        fprintf(file, "%ld\n", __histograms_cumulative[histogram_id][i]);
    }
}

#endif //_HISTOGRAM_C_INCLUDED
