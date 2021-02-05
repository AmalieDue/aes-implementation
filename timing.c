#include <time.h>
#include <stdio.h>

int main() {
	clock_t start_time = clock();

	// do stuff

	clock_t time_spent = clock() - start_time;
	printf("cpu time : %f (sec.)\n", (double)time_spent / CLOCKS_PER_SEC);

}