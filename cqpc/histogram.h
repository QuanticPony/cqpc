#ifndef _HISTOGRAM_H_INCLUDED
#define _HISTOGRAM_H_INCLUDED

#include <stdio.h>
#include <stdlib.h> 
#include <math.h>

/** Prepares a new histogram and returns the ID
 * * nbins: number of bins
 * * max: max value to accept when filling the histogram
 * * min: min value to accept when filling the histogram
*/
int prep_histogram(int nbins, double max, double min);


/** Fill histogram of given ID with nvalues from the array values
 * * histogram_id: histogram to add values to
 * * nvalues: number of values to add from values
 * * values: array of values to add
*/
void fill_histogram(int histogram_id, int nvalues, double* values);

/** Gets a pointer to the normalized histogram of given ID
 * * histogram_id: histogram to get copy of
*/
double* get_histogram(int histogram_id);


/** Clean the histogram of given ID from memory
 * * histogram_id: histogram to remove
*/
void clean_histogram(int histogram_id);

/** Cleans all histograms*/
void clean_histograms();

/** Loads histogram to cache and returns the ID assigned
 * * file: pointer to file to read from
*/
int load_from_file(FILE *file);

/** Saves histogram to file
 * * histogram_id: histogram to save
 * * file: pointer to file to write into
*/
void dump_to_file(int histogram_id, FILE *file);

#ifndef _HISTOGRAM_C_INCLUDED
#include "histogram.c"
#endif

#endif //_HISTOGRAM_H_INCLUDED