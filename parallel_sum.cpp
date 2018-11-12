#include<iostream>
#include<vector>
#include<ctime>
#include<chrono>
#include<math.h>
#include<iomanip>

using namespace std;

#define vi vector<int>
#define rep($, a, b) for(int $=(a); $<(b); ++$)

int NUMTHREAD;
int N;
vi A;

int _IT;

void *sum_arr_parallel(void* in)
{
	int iter, id;
	iter=_IT;
	id=(intptr_t)in;
	int diff=pow(2, iter-1);
	int idx=(pow(2, iter)*(id+1))-1;
	//cout<<"iter "<<iter<<" id "<<id<<" idx "<<idx<<endl;
	A[idx]=A[idx]+A[idx-diff];
	pthread_exit(NULL);	
}

int main()
{
	vector<pthread_t> thread_vc;
	int rc, iter;
	
	cout<<"Enter N: ";
	cin>>N;
	
	A.resize(N);
	
	srand(time(NULL));
	rep(i, 0, N)
	{
		A[i]=rand()%10;
	}
	cout<<"Data generated!\n";
	cout<<"ARRAY: ";
	rep(i, 0, N)
	cout<<A[i]<<" ";
	cout<<endl;
	
	auto begin=chrono::high_resolution_clock::now();
	
	iter=1;
	while(iter<=ceil(log2(N)))
	{
		_IT=iter;
		NUMTHREAD=ceil(N/pow(2, iter));
		thread_vc.clear();
		thread_vc.resize(NUMTHREAD);
		rep(i, 0, NUMTHREAD)
		{
			int thread_id=i;
			rc=pthread_create(&thread_vc[i], NULL, sum_arr_parallel, (void*)(intptr_t)thread_id);
			if(rc)
			{
				cout<<"Error: Unable to create thread, "<<rc<<endl;
				exit(-1);
			}
		}
		rep(i, 0, NUMTHREAD)
			pthread_join(thread_vc[i], NULL);
		cout<<"ITER "<<iter<<": ";
		rep(i, 0, N)
		cout<<A[i]<<" ";
		cout<<endl;
		++iter;
	}
	auto end=chrono::high_resolution_clock::now();
	
	cout<<"Data processed!\n";
	cout<<"ARRAY: ";
	rep(i, 0, N)
	cout<<A[i]<<" ";
	cout<<endl;
	cout<< "Time in processing: "<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s"<<endl;
	cout<<"SUM= "<<A[N-1]<<endl;
	return 0;
}
