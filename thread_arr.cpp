#include<iostream>
#include<cstdlib>
#include<cstdint>
#include<pthread.h>

using namespace std;
#define NUMTHREAD 200

void *call_thread_fun(void *in)
{
	cout<<"\tYou called thread "<<(long)in<<"\n";
	cout<<"\tMy name is: "<<(long)in<<"\n";
	pthread_exit(NULL);
}

int main()
{
	pthread_t thread_array[NUMTHREAD];
	int rc;
	cout<<"Start the work\n";
	for(int i=0; i<NUMTHREAD; ++i)
	{
		cout<<"Creating thread"<< i<<endl;
		int thread_id=i;
		rc=pthread_create(&thread_array[i], NULL, call_thread_fun, (void*)(intptr_t)thread_id);
		if(rc)
		{
			cout<<"Error: Unable to create thread, "<<rc<<endl;
			exit(-1);
		}
	}
	
	cout<<"Finished calling threads\n";
	cout<<"Waiting for threads\n";
	
	for(int i=0; i<NUMTHREAD; ++i)
		pthread_join(thread_array[i], NULL);
	
	cout<<"Finished the work\n";
	pthread_exit(NULL);
	
	return 0;
}

