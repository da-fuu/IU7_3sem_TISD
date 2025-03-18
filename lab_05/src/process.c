#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "input.h"
#include "process.h"

#define EPS 0.0000001
#define IS_ZERO(time) (fabs(time) < EPS)


double expected_time(double lo, double hi)
{
    return (lo + hi) / 2.0;
}


double calc_total_time(params_t *params)
{
    double t1 = expected_time(params->t1_lo, params->t1_hi);
    double t2 = expected_time(params->t2_lo, params->t2_hi);
    double t3 = expected_time(params->t3_lo, params->t3_hi);
    double t4 = expected_time(params->t4_lo, params->t4_hi);

    // Очередь 1 никогда не отдает приоритет
    if (t3 > t1)
        return t3 * REQ_1_MAX;

    return fmax(t1 * REQ_1_MAX, t3 * REQ_1_MAX + t4 * (t1 / t2 * REQ_1_MAX));
}


double generate_float(double lo, double hi)
{
    return lo + (double)rand() / (double)RAND_MAX * (hi - lo);
}

double find_min_time(double time_1, double time_2, double proc_time, bool is_running)
{
    if (!is_running)
        return fmin(time_1, time_2);
    return fmin(fmin(time_1, time_2), proc_time);
}


void print_stats_hundreds(int processed_1, int cur_len_q_1, double avg_len_q_1, int cur_len_q_2, double avg_len_q_2)
{
    printf("Обработано заявок первого типа: %d\n", processed_1);
    printf("Текущая длина первой очереди: %d, средняя длина: %.2lf\n", cur_len_q_1, avg_len_q_1);
    printf("Текущая длина второй очереди: %d, средняя длина: %.2lf\n\n", cur_len_q_2, avg_len_q_2);
}

void print_results(double total_time, double calculated_time, double used_time, double wasted_time, int processed_1, int processed_2, int received_1, int received_2)
{
    printf("Симуляция заняла всего: %.2lf е.в.\n", total_time);
    printf("Теоретический расчет времени: %.2lf е.в.\n", calculated_time);
    printf("Погрешность расчета: %.2lf%%\n", fabs(1.0 - calculated_time / total_time) * 100.0);
    printf("Из них ОА работал: %.2lf е.в., простаивал: %.2lf е.в.\n", used_time, wasted_time);
    printf("Всего пришло заявок первого типа: %d, из них обработано: %d\n", received_1, processed_1);
    printf("Всего пришло заявок второго типа: %d, из них обработано: %d\n\n", received_2, processed_2);
}


int run_simulation(params_t *params, abstract_queue_t *queue_1, abstract_queue_t *queue_2, bool verbose, int *max_queue_len)
{
    int processed_1 = 0;
    int processed_2 = 0; 
    int received_1 = 0;
    int received_2 = 0;
    double total_time = 0.0;
    double time_used = 0.0;
    double time_wasted = 0.0;
    bool is_running = false;
    double process_time_left = 0.0;
    double time_to_arrive_1_left = generate_float(params->t1_lo, params->t1_hi);
    double time_to_arrive_2_left = generate_float(params->t2_lo, params->t2_hi);
    double time_step;
    double sum_all_sizes_per_tick_1 = 0.0;
    double sum_all_sizes_per_tick_2 = 0.0;
    int type_in_machine = 0;
    bool was_empty_1 = true;
    bool was_empty_2 = true;
    int max_len = 0;
    value_t time;
    
    
    while (processed_1 < REQ_1_MAX)
    {
        time_step = find_min_time(time_to_arrive_1_left, time_to_arrive_2_left, process_time_left, is_running);
        total_time += time_step;
        
        sum_all_sizes_per_tick_1 += queue_1->get_len(queue_1->queue) * time_step;
        sum_all_sizes_per_tick_2 += queue_2->get_len(queue_2->queue) * time_step;
        
        time_to_arrive_1_left -= time_step;
        time_to_arrive_2_left -= time_step;
        if (is_running)
        {
            time_used += time_step;
            process_time_left -= time_step;
        }
        else
        {
            time_wasted += time_step;
        }
        
        if (IS_ZERO(time_to_arrive_1_left))
        {
            time.time = generate_float(params->t3_lo, params->t3_hi);
            if (!queue_1->push(queue_1->queue, time))
            {
                puts("Очередь 1 переполнена!");
                return ERR_CRITICAL;
            }
            received_1++;
            time_to_arrive_1_left = generate_float(params->t1_lo, params->t1_hi);
        }
        if (IS_ZERO(time_to_arrive_2_left))
        {
            time.time = generate_float(params->t4_lo, params->t4_hi);
            if (!queue_2->push(queue_2->queue, time))
            {
                puts("Очередь 2 переполнена!");
                return ERR_CRITICAL;
            }
            received_2++;
            time_to_arrive_2_left = generate_float(params->t2_lo, params->t2_hi);
        }
        if (is_running && IS_ZERO(process_time_left))
        {
            if (type_in_machine == 1)
            {
                processed_1++;
                if (verbose && processed_1 % 100 == 0)
                    print_stats_hundreds(processed_1, queue_1->get_len(queue_1->queue), sum_all_sizes_per_tick_1 / total_time, 
                                         queue_2->get_len(queue_2->queue), sum_all_sizes_per_tick_2 / total_time);
            }
            else if (type_in_machine == 2)
            {
                processed_2++;
            }
            is_running = false;
        }
        if (!is_running)
        {
            if (((type_in_machine == 1 && !was_empty_1) || (type_in_machine == 2 && was_empty_2) || (was_empty_1 && was_empty_2)) && queue_1->pop(queue_1->queue, &time))
            {
                process_time_left = time.time;
                is_running = true;
                was_empty_1 = queue_1->get_len(queue_1->queue) == 0;
                type_in_machine = 1;
            }
            else if (((type_in_machine == 2 && !was_empty_2) || (type_in_machine == 1 && was_empty_1) || (was_empty_1 && was_empty_2)) && queue_2->pop(queue_2->queue, &time))
            {
                process_time_left = time.time;
                is_running = true;
                was_empty_2 = queue_2->get_len(queue_2->queue) == 0;
                type_in_machine = 2;
            }
            else
            {
                type_in_machine = 0;
                was_empty_1 = true;
                was_empty_2 = true;
            }
        }
        if (max_queue_len != NULL && max_len < queue_1->get_len(queue_1->queue) + queue_2->get_len(queue_2->queue))
            max_len = queue_1->get_len(queue_1->queue) + queue_2->get_len(queue_2->queue);
            
    }
    if (verbose)
        print_results(total_time, calc_total_time(params),time_used, time_wasted, processed_1, processed_2, received_1, received_2);
    if (max_queue_len != NULL && *max_queue_len < max_len)
        *max_queue_len = max_len;
    return 0;
}
