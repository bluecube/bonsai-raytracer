/**
 * \file
 * Implementation of the performance measurement counters.
 * This file is compiled only when the measurements are not disabled.
 */
#ifndef DISABLE_MEASUREMENTS

#include "measurements.h"

#include <stdio.h>
#include <time.h>

#define WARMUP_COUNT 10000

/**
 * Number of times the ray-scene intersection has been
 * computed since the last init.
 */
unsigned measurementsIntersectionCounter;

/**
 * Time of the last init.
 */
static time_t measurementsStartTime;

/**
 * Reset the counters and remember the start time.
 */
void measurements_start(){
	measurementsIntersectionCounter = 0;
	measurementsStartTime = time(NULL);
}

/**
 * Print the measured data.
 */
void measurements_print(){
	time_t endTime = time(NULL);

	double timeTaken = difftime(endTime, measurementsStartTime);
	
	printf("time taken: %.1f seconds\n", timeTaken);
	printf("ray-scene intersections: %i\n", measurementsIntersectionCounter);
	printf("rate: %.3f intersections per second\n",
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
