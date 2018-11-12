#include<stdio.h>
#include<mpi.h>

int main()
{
	int rank, num;
	int tag, source, dest, count;
	int buffer;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	tag=123;
	source=0; dest=1;
	count=1;
	
	if(rank==source)
	{
		buffer=5678;
		MPI_Send(&buffer, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
		printf("process %d sent %d\n", rank, buffer);
	}
	else if(rank==dest)
	{
		MPI_Recv(&buffer, count, MPI_INT, source, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		printf("process %d received %d\n", rank, buffer);
	}
	
	MPI_Finalize();
	
	return 0;
}
