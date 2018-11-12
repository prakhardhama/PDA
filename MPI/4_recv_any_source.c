#include<stdio.h>
#include<mpi.h>

int main()
{
	int rank, num;
	int tag, dest, count;
	int buffer;
	MPI_Status status;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	tag=123;
	dest=0;
	count=1;
	
	if(rank!=dest)
	{
		buffer=rank;
		MPI_Send(&buffer, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else if(rank==dest)
	{
		for(int j=0; j<num-1; ++j)
		{	
			MPI_Recv(&buffer, count, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			printf("process %d received %d from %d\n", rank, buffer, status.MPI_SOURCE);
		}
	}
	
	MPI_Finalize();
	
	return 0;
}
