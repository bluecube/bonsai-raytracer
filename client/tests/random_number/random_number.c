#include <assert.h>
#include <stdio.h>

#include <random.h>

#define MAX 100
#define COUNT 1000000

unsigned bins[MAX];

int main(){
	for(int i = 0; i < COUNT; ++i){
		float result = random_number(0, MAX);

		int bin = (int)result;
		assert(bin >= 0);
		assert(bin < MAX);

		++bins[bin];
	}

	printf("# bin\tcount\n");

	for(int i = 0; i < MAX; ++i){
		printf("%i\t%u\n", i, bins[i]);
	}

	return 0;
}
