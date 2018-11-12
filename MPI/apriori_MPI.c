#include<stdio.h>
#include<stdlib.h>
#include<mpi.h>

//mpicc -w -o apriori_MPI apriori_MPI.c 
//mpirun

void readFromFile(int *minSupportRef, int *minConfRef, int *itemNumRef, int *transNumRef, int **transSizeRef, char **itemSetRef, char ***transSetRef)
{
	int i, j;
	int minSupport, minConf, itemNum, transNum, *transSize;
	char *itemSet, **transSet;
	FILE *fp;
	fp = fopen("transactions.txt", "r");
	fscanf(fp, "%d %d\n", &minSupport, &minConf);
	fscanf(fp, "%d\n", &itemNum);
	itemSet = (char*)malloc(itemNum*sizeof(int));
	for(i=0; i<itemNum; ++i)
		fscanf(fp, "%c ", &itemSet[i]);
	fscanf(fp, "%d\n", &transNum);
	transSize = (int*)malloc(transNum*sizeof(int));
	transSet = (char**)malloc(transNum*sizeof(char*));
	for(i=0; i<transNum; ++i)
	{
		fscanf(fp, "%d\n", &transSize[i]);
		transSet[i] = (char*)malloc(transSize[i]*sizeof(char)); 
		for(j=0; j<transSize[i]; ++j)
			fscanf(fp, "%c ", &transSet[i][j]);
	}
	fclose(fp);
	*minSupportRef = minSupport; *minConfRef = minConf;
	*itemNumRef = itemNum; *transNumRef = transNum;
	*itemSetRef = itemSet; *transSetRef = transSet; *transSizeRef = transSize;
}

int main()
{
	int i, j, k, l;
	int minSupport, minConf, itemNum, transNum, *transSize; 
	char *itemSet; char **transSet;
	
	int numProc, rank, N_LT, *LC, *GC;
	char **LT;
	
	MPI_Init(NULL, NULL);
	MPI_Comm_size(MPI_COMM_WORLD, &numProc);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	
	if(rank==0)
	{
		printf("Reading data by rank 0..\n");
		readFromFile(&minSupport, &minConf, &itemNum, &transNum, &transSize, &itemSet, &transSet);
		printf("minSupport= %d\n", minSupport);	
		printf("minConf= %d\n", minConf);
		printf("itemSet:\n");
		for(i=0; i<itemNum; ++i) printf("%c ",itemSet[i]);
		printf("\n");
		printf("transSet:\n");
		for(i=0; i<transNum; ++i)
		{
			for(j=0; j<transSize[i]; ++j) printf("%c ", transSet[i][j]);
			printf("\n");
		}
		minSupport=minSupport*transNum/100;
	}
	
	MPI_Bcast(&minSupport, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&minConf, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&itemNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(&transNum, 1, MPI_INT, 0, MPI_COMM_WORLD);
	
	if(rank!=0) 
	{
		transSize=(int*)malloc(transNum*sizeof(int));
		itemSet=(char*)malloc(itemNum*sizeof(char)); 
	}
	MPI_Bcast(transSize, transNum, MPI_INT, 0, MPI_COMM_WORLD);
	MPI_Bcast(itemSet, itemNum, MPI_CHAR, 0, MPI_COMM_WORLD);
	
	N_LT = transNum/numProc;
	LT=(char**)malloc(N_LT*sizeof(char*));
	LC=(int*)calloc(itemNum, sizeof(int));
	if(rank==0)
	{
		printf("Scattering data by rank 0..\n");
		k=0;
		for(i=0; i<N_LT; ++i)
		{
			LT[i]=(char*)malloc(transSize[k]*sizeof(char));
			for(j=0; j<transSize[k]; ++j)
				LT[i][j]=transSet[k][j];
			++k;
		}
		for(i=1; i<numProc; ++i)
		{	
			for(j=0; j<N_LT; ++j, ++k)
				MPI_Send(transSet[k], transSize[k], MPI_CHAR, i, 0, MPI_COMM_WORLD);
		}
	}
	else
	{
		k=rank*N_LT;
		for(i=0; i<N_LT; ++i, ++k)
		{
			LT[i]=(char*)malloc(transSize[k]*sizeof(char));
			MPI_Recv(LT[i], transSize[k], MPI_CHAR, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		}
	}
	
	if(rank==0)
	{
		printf("My data rank 0..\n");
		k=rank*N_LT;
		for(i=0; i<N_LT; ++i, ++k)
		{
			for(j=0; j<transSize[k]; ++j)
			printf("%c ", LT[i][j]);
			printf("\n");
		}
	}	
	
	k=rank*N_LT;
	for(i=0; i<N_LT; ++i, ++k)
	{
		for(j=0; j<transSize[k]; ++j)
		{	
			for(l=0; l<itemNum; ++l)
				if(itemSet[l]==LT[i][j]) {++LC[l]; break;}
		}
	}
	
	if(rank==0)
	{
		printf("My local count rank 0..\n");
		for(i=0; i<itemNum; ++i)
			printf("%d ", LC[i]);
		printf("\n");
	}
	
	if(rank!=0)
		MPI_Send(LC, itemNum, MPI_INT, 0, 0, MPI_COMM_WORLD);
	if(rank==0)
	{
		GC=(int*)malloc(itemNum*sizeof(int));
		for(j=0; j<itemNum; ++j)
			GC[j]=LC[j];
		for(i=0; i<numProc-1; ++i)
		{
			MPI_Recv(LC, itemNum, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			for(j=0; j<itemNum; ++j)
				GC[j]+=LC[j];
		}
		printf("Global count..\n");
		for(i=0; i<itemNum; ++i)
			printf("%d ", GC[i]);
		printf("\n");
		printf("Length 1 Candidate itemset:\n");
		for(i=0; i<itemNum; ++i)
		if(GC[i]>=minSupport) printf("%c ", itemSet[i]);
		printf("\n");
	}
	
	MPI_Finalize();	
	return 0;
}
