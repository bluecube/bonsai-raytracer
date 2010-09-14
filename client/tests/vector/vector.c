#include <assert.h>
#include <stdio.h>

#include <vector.h>

void test_assign(){
	vector_t x = vector_set(1, 2, 4);

	printf("vector = (%f, %f, %f (%f))\n", x.x, x.y, x.z, x.w);
	assert(x.x == 1);
	assert(x.f[0] == x.x);
	assert(x.y == 2);
	assert(x.f[1] == x.y);
	assert(x.z == 4);
	assert(x.f[2] == x.z);
}

void test_dot(){
	vector_t x = vector_set(2, 0, 0);
	assert(vector_dot(x, x) == 4);

	vector_t y = vector_set(0, 4, 0);
	assert(vector_dot(y, y) == 16);

	vector_t z = vector_set(0, 0, 8);
	assert(vector_dot(z, z) == 64);
}

int main(){
	test_assign();
	test_dot();
}
