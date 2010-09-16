#include <assert.h>
#include <stdio.h>
#include <math.h>

#include <bounding_box.h>
#include <ray.h>

struct bounding_box b;
struct ray r;

void test_hit(){
	ray_from_points(&r, vector_set(12, 13, 14), vector_set(0, 0, 0));

	float distance = bounding_box_ray_intersection(&b, &r, 0, INFINITY);
	assert(!isnan(distance));
}

int main(){
	b.p[0].x = -1;
	b.p[0].y = -1;
	b.p[0].z = -1;
	b.p[1].x = 1;
	b.p[1].y = 1;
	b.p[1].z = 1;

	test_hit();

	return 0;
}
