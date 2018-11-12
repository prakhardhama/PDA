#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

int main()
{
	int rank, num;
	int tag, dest, count;
	int *buff;
	MPI_Status status;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &num);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	tag=123;
	dest=0;
	
	if(rank!=dest)
	{
		count=rank;
		buff=(int*)malloc(count*sizeof(int));
		for(int i=0; i<count; ++i)
			buff[i]=i;
		MPI_Send(buff, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	}
	else if(rank==dest)
	{
		for(int j=0; j<num-1; ++j)
		{
			MPI_Probe(MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, &status);
			MPI_Get_count(&status, MPI_INT, &count);
			buff=(int*)malloc(count*sizeof(int));
			MPI_Recv(buff, count, MPI_INT, MPI_ANY_SOURCE, tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			printf("process %d received from %d: ", rank, status.MPI_SOURCE);
			for(int i=0; i<count; ++i)
				printf("%d ", buff[i]);
			printf("\n");
		}
	}
	MPI_Finalize();
	
	return 0;
}
