/**
 * \file
 * Macros for measuring the performance of the raytracer.
 * If the macro DISABLE_MEASUREMENTS is defined, then all the
 * measurement macros are only a empty command.
 */
#ifndef BONSAI_CLIENT_MEASUREMENTS_H
#define BONSAI_CLIENT_MEASUREMENTS_H

#ifdef DISABLE_MEASUREMENTS

#define MEASUREMENTS_WARMUP() ((void)0)
#define MEASUREMENTS_START() ((void)0)
#define MEASUREMENTS_RAY_SCENE_INTERSECTION() ((void)0)
#define MEASUREMENTS_PRINT() ((void)0)

#undef MEASUREMENTS_KD_TREE_STATS

#else

extern unsigned measurementsIntersectionCounter;

void measurements_start();
void measurements_print();
void measurements_warmup();

/**
 * Get the cpu from power saving states.
 */
#define MEASUREMENTS_WARMUP() \
	measurements_warmup()

/**
 * Reset the counters and remember the start time.
 */
#define MEASUREMENTS_START() \
	measurements_start()

/**
 * Register another ray-scene intersection being computed.
 */
#define MEASUREMENTS_RAY_SCENE_INTERSECTION() \
	(++measurementsIntersectionCounter)

/**
 * Print all the measured data since the last MEASUREMENTS_START().
 */
#define MEASUREMENTS_PRINT() \
	measurements_print()

#endif

#ifndef MEASUREMENTS_KD_TREE_STATS

#define MEASUREMENTS_TREE_TRAVERSAL() ((void)0)
#define MEASUREMENTS_OBJECT_INTERSECTION() ((void)0)

#define MEASUREMENTS_CLEAR_KD_TREE_STATS() ((void)0)

#else

extern unsigned measurementsTreeTraversalCounter;
extern unsigned measurementsObjectIntersectionCounter;

void measurements_clear_kd_tree_stats();

#define MEASUREMENTS_TREE_TRAVERSAL() \
	(++measurementsTreeTraversalCounter)

#define MEASUREMENTS_OBJECT_INTERSECTION() \
	(++measurementsObjectIntersectionCounter)

#define MEASUREMENTS_CLEAR_KD_TREE_STATS() \
	measurements_clear_kd_tree_stats()

#endif

#endif
