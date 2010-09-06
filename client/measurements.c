/**
 * \file
 * Implementation of the performance measurement counters.
 * This file is compiled only when the measurements are not disabled.
 */
#ifndef DISABLE_MEASUREMENTS

#include "measurements.h"

#include <stdint.h>
#include <stdio.h>
#include <sys/time.h>

#define WARMUP_COUNT 10000000UL

/**
 * Number of microseconds since some time in the past (the epoch?)
 */
typedef int_fast64_t timestamp_t;

/**
 * Number of times the ray-scene intersection has been
 * computed since the last init.
 */
unsigned measurementsIntersectionCounter;

/**
 * Time of the last init.
 */
static timestamp_t measurementsStartTime;

/**
 * Return the current time.
 */
static timestamp_t get_timer(){
        struct timeval t;
        gettimeofday(&t, NULL);
        return 1000000 * t.tv_sec + t.tv_usec;
}

/**
 * Reset the counters and remember the start time.
 */
void measurements_start(){
	measurementsIntersectionCounter = 0;
	measurementsStartTime = get_timer();
}

/**
 * Print the measured data.
 */
void measurements_print(){
	timestamp_t endTime = get_timer();

	double timeTaken = (endTime - measurementsStartTime) / 1000000.0;
	
	printf("time taken: %.3f seconds\n", timeTaken);
	printf("ray-scene intersections: %i\n", measurementsIntersectionCounter);
	printf("rate: %.5f intersections per second\n",
		measurementsIntersectionCounter / timeTaken);
}

/**
 * Get the cpu from power saving states.
 */
void measurements_warmup(){
	int x = 0;
	for(int i = 0; i < WARMUP_COUNT; ++i){
		x += i;
	}

	printf("Warmed up (0x%0X :-) )\n", x % 0xff);
}

#endif

#ifdef MEASUREMENTS_KD_TREE_STATS

unsigned measurementsTreeTraversalCounter;
unsigned measurementsObjectIntersectionCounter;

void measurements_clear_kd_tree_stats(){
	measurementsTreeTraversalCounter = 0;
	measurementsObjectIntersectionCounter = 0;
}

#endif
