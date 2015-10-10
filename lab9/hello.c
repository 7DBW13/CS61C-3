#include <stdio.h>
#include <omp.h>

int main() {
	#pragma omp parallel
	{
		int thread_ID = omp_get_thread_num();
		int num = omp_get_num_threads();
		printf(" hello world %d\n", num);
	}
}
