#include<iostream>
#include<ctime>
#include<chrono>
#include<math.h>
#include<iomanip>

using namespace std;

#define PI 3.141592653589793
#define ARRSIZE 500000
#define NUMTHREAD 4
#define NUMTERM 1000

double array[ARRSIZE];
double cos_array[ARRSIZE];

double cos_x(double x, double t)
{
	double pres, res, xval, fval;
	int sign;
	sign=-1;
	xval=x*x; fval=2;
	res=1+sign*xval/fval;
	for(int k=2; k<=t; ++k)
	{
		xval*=x*x;
		fval*=(2*k-1)*(2*k);
		sign*=-1;
		pres=res;
		res+=sign*xval/fval;
		if(isnan(res)) return pres;
	}
	return res;	
}

void *cos_parallel(void* in)
{
	double res;
	int idx;
	idx=(intptr_t)in;
	for(int i=idx; i<ARRSIZE; i+=NUMTHREAD)
	{
		res=cos_x(array[i], NUMTERM);
		cos_array[i]=res;
	}
	pthread_exit(NULL);	
}

int main()
{
	pthread_t thread_array[NUMTHREAD];
	int rc;
	
	cout<<"Start the work\n";
	cout<<setprecision(15);
	
	srand(time(NULL));
	for(int i=0; i<ARRSIZE; ++i)
		array[i]=(rand()%360)*PI/180.0;
	
	cout<<"Data generated\n";
	
	auto begin=chrono::high_resolution_clock::now();
	for(int i=0; i<NUMTHREAD; ++i)
	{
		int thread_id=i;
		rc=pthread_create(&thread_array[i], NULL, cos_parallel, (void*)(intptr_t)thread_id);
		if(rc)
		{
			cout<<"Error: Unable to create thread, "<<rc<<endl;
			exit(-1);
		}
	}
	
	for(int i=0; i<NUMTHREAD; ++i)
		pthread_join(thread_array[i], NULL);	
	auto end=chrono::high_resolution_clock::now();
		
	cout<<"Data processed\n";
	
	/*
	for(int i=0; i<ARRSIZE; ++i)
	{
		cout<<array[i]<<"\t: "<<cos_array[i]<<endl;
	}*/
	cout<<array[0]<<"\t: "<<cos_array[0]<<endl;
	
	cout<<"End of work\n";
	cout<< "Time in processing: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	
	pthread_exit(NULL);
	return 0;
}
