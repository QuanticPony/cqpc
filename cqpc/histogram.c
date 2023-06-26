#ifndef _HISTOGRAM_C_INCLUDED
#define _HISTOGRAM_C_INCLUDED

#ifndef _HISTOGRAM_H_INCLUDED
#include "histogram.h"
#endif

static struct cqpc_histogram_cache
{
    int size;
    cqpc_histogram *histograms;
} CACHE = {0};

// static struct cqpc_histogram_cache CACHE = {0};

static double **__histograms_return;

static int get_cache_position()
{
    int i;
    for (i = 0; i < CACHE.size; i++)
    {
        if (!CACHE.histograms[i].prep)
        {
            return i;
        }
    }
    return CACHE.size;
}

static void realloc_cache(int size)
{
    if (CACHE.size == 0)
    {
        CACHE.size = size;
        CACHE.histograms = malloc(CACHE.size * sizeof(cqpc_histogram));
        __histograms_return = malloc(CACHE.size * sizeof(double *));
    }
    else
    {
        if (size < CACHE.size)
        {
            int i;
            i = get_cache_position();
            CACHE.histograms[i] = CACHE.histograms[CACHE.size - 1];
        }
        CACHE.size = size;
        CACHE.histograms = realloc(CACHE.histograms, CACHE.size * sizeof(cqpc_histogram));
        __histograms_return = realloc(__histograms_return, CACHE.size * sizeof(double *));
    }
}

cqpc_histogram *cqpc_histogram_get_cqpc(int histogram_id)
{
    if (histogram_id >= CACHE.size)
        return (cqpc_histogram*) 0;

    return CACHE.histograms + histogram_id;
}

int cqpc_histogram_prep(double min, double max, int nbins)
{
    int i;
    i = get_cache_position();

    // If not, realloc to increase size
    if (i == CACHE.size)
    {
        realloc_cache(CACHE.size + 1);
    }

    CACHE.histograms[i].cumulative = calloc(nbins, sizeof(long));
    CACHE.histograms[i].nbins = nbins;
    CACHE.histograms[i].max = max;
    CACHE.histograms[i].min = min;
    CACHE.histograms[i].delta = (max - min) / ((double)nbins);
    CACHE.histograms[i].prep = 1;
    CACHE.histograms[i].count = 0;

    return i;
}

void cqpc_histogram_fill(int histogram_id, int nvalues, double *values)
{
    int i, index;
    double value;
    cqpc_histogram *hist;
    hist = cqpc_histogram_get_cqpc(histogram_id);

    for (i = 0; i < nvalues; i++)
    {
        value = values[i];
        if (value < hist->min ||
            value > hist->max)
            continue;

        index = (value - hist->min) / hist->delta;

        hist->cumulative[index]++;
    }
    hist->count += nvalues;
}

double* cqpc_histogram_get(int histogram_id)
{

    int i;
    double *histogram;
    double k_normalization = 1 / (CACHE.histograms[histogram_id].count * CACHE.histograms[histogram_id].delta);

    __histograms_return[histogram_id] = malloc(CACHE.histograms[histogram_id].nbins * sizeof(double));
    histogram = __histograms_return[histogram_id];

    cqpc_histogram *hist;
    hist = cqpc_histogram_get_cqpc(histogram_id);
    for (i = 0; i < hist->nbins; i++)
    {
        histogram[i] = k_normalization * hist->cumulative[i];
    }

    return histogram;
}

void cqpc_histogram_clean(int histogram_id)
{
    if (histogram_id >= CACHE.size)
        return;

    if (CACHE.histograms[histogram_id].prep)
    {
        free(CACHE.histograms[histogram_id].cumulative);
        free(__histograms_return[histogram_id]);
        CACHE.histograms[histogram_id].count = 0;
        CACHE.histograms[histogram_id].nbins = 0;
        CACHE.histograms[histogram_id].max = 0;
        CACHE.histograms[histogram_id].min = 0;
        CACHE.histograms[histogram_id].delta = 0.;
        CACHE.histograms[histogram_id].prep = 0;
    }
    realloc_cache(CACHE.size - 1);
}

void cqpc_histogram_clean_all()
{
    int i;

    for (i = 0; i < CACHE.size; i++)
    {
        cqpc_histogram_clean(i);
    }
}

int cqpc_histogram_load_from_file(FILE *file)
{
    int histogram_id, i;
    double max, min, delta;
    long nbins, count;

    fscanf(file, "// nbins=%ld\n", &nbins);
    fscanf(file, "// min=%lf\n", &min);
    fscanf(file, "// max=%lf\n", &max);
    fscanf(file, "// delta=%lf\n", &delta);
    fscanf(file, "// count=%ld\n", &count);

    histogram_id = cqpc_histogram_prep(min, max, nbins);
    cqpc_histogram *hist = cqpc_histogram_get_cqpc(histogram_id);

    long read_value;

    for (i = 0; i < nbins; i++)
    {
        fscanf(file, "%ld\n", &read_value);
        hist->cumulative[i] = read_value;
    }
    hist->count = count;

    return histogram_id;
}

void cqpc_histogram_dump_to_file(int histogram_id, FILE *file)
{
    int i;
    cqpc_histogram *hist = cqpc_histogram_get_cqpc(histogram_id);

    fprintf(file, "// nbins=%ld\n", hist->nbins);
    fprintf(file, "// min=%lf\n", hist->min);
    fprintf(file, "// max=%lf\n", hist->max);
    fprintf(file, "// delta=%f\n", hist->delta);
    fprintf(file, "// count=%ld\n", hist->count);

    for (i = 0; i < hist->nbins; i++)
    {
        fprintf(file, "%ld\n", hist->cumulative[i]);
    }
}


void cqpc_histogram_dump_to_file_normalized(int histogram_id, FILE *file)
{
    int i;
    cqpc_histogram *hist = cqpc_histogram_get_cqpc(histogram_id);
    double *hist_norm = cqpc_histogram_get(histogram_id);

    fprintf(file, "// nbins=%ld\n", hist->nbins);
    fprintf(file, "// min=%lf\n", hist->min);
    fprintf(file, "// max=%lf\n", hist->max);
    fprintf(file, "// delta=%f\n", hist->delta);
    fprintf(file, "// count=%ld\n", hist->count);

    for (i = 0; i < hist->nbins; i++)
    {
        fprintf(file, "%lf\n", hist_norm[i]);
    }
}

#endif //_HISTOGRAM_C_INCLUDED
