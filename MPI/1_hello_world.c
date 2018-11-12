#include<stdio.h>
#include<mpi.h>

int main()
{
	int rank, num;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	printf("Hello from %d of %d\n", rank, num);
	
	MPI_Finalize();
	
	return 0;
}
