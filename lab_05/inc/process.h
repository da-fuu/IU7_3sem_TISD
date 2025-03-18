#ifndef PROCESS_H__
#define PROCESS_H__

#include "struct.h"

#define REQ_1_MAX 1000


typedef struct
{
    double t1_lo;
    double t1_hi;
    double t2_lo;
    double t2_hi;
    double t3_lo;
    double t3_hi;
    double t4_lo;
    double t4_hi;
} params_t;


int run_simulation(params_t *params, abstract_queue_t *queue_1, abstract_queue_t *queue_2, bool verbose, int *max_queue_len);

double expected_time(double lo, double hi);


#endif // #ifndef PROCESS_H__
