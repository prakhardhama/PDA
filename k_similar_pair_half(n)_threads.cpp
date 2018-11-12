#include<iostream>
#include<vector>
#include<queue>
#include<algorithm>
#include<set>
#include<string>
#include<cassert>
#include<ctime>
#include<chrono>
#include<iomanip>

using namespace std;

#define ull unsigned long long
#define rep($, a, b) for(ull $=(a); $<(b); ++$)

struct Record
{
	string obj;
	set<string> attr;
};

ull N, K;
vector<Record> records;

struct Similar
{
	ull id1, id2;
	double js;
	bool operator<(const Similar &sim) const
	{
		bool flag=false;
		if(js<sim.js) flag=true;
		else if(js==sim.js && id1<sim.id1) flag=true;
		else if(js==sim.js && id1==sim.id1 && id2==sim.id2) flag=true;
		return flag; 
	}	
};

priority_queue<Similar> sims;

void input()
{
	freopen("k_similar_pair.in", "r", stdin);
	cin>>N>>K;
	cin.ignore();
	string inp;
	set<string> attrib;
	records.resize(N);
	int a=0;
	while(getline(cin, inp))
	{
		size_t pos=inp.find(',');
		string object=inp.substr(0, pos);
		set<string> attrib;
		while(pos!=string::npos)
		{
			inp=inp.substr(pos+1);
			pos=inp.find(',');
			attrib.insert(inp.substr(0, pos));
		}
		Record rec;
		rec.obj=object;
		rec.attr=attrib;
		records[a++]=rec;
	}
	ull T;
	if(N%2==0) T=(N/2)*(N-1);
	else T=((N-1)/2)*N;
	assert(K<=T);
}

vector<vector<double>> res;
void *calc_sim_n2(void* in)
{
	ull idx=(ull)in;
	
	ull r=idx;
	rep(c, r+1, N)
	{
		double js;
		set<string> common;
		set_intersection(records[r].attr.begin(), records[r].attr.end(), records[c].attr.begin(), 
							records[c].attr.end(), inserter(common, common.begin()));
		js=(double)common.size()/(records[r].attr.size()+records[c].attr.size()-common.size());
		res[r][c]=js;
	}
	r=N-idx-1;
	if(idx!=N/2)
	rep(c, r+1, N)
	{
		double js;
		set<string> common;
		set_intersection(records[r].attr.begin(), records[r].attr.end(), records[c].attr.begin(), 
							records[c].attr.end(), inserter(common, common.begin()));
		js=(double)common.size()/(records[r].attr.size()+records[c].attr.size()-common.size());
		res[r][c]=js;
	}
	pthread_exit(NULL);	
}

void solve()
{
	vector<pthread_t> thread_vc;
	ull NUMTHREAD=ceil(N/2);
	thread_vc.resize(NUMTHREAD);
	
	res.resize(N);
	fill(res.begin(), res.end(), vector<double>(N, 0.0));
	rep(i, 0, NUMTHREAD)
	{
		ull thread_id=i;
		auto rc=pthread_create(&thread_vc[i], NULL, calc_sim_n2, (void*)thread_id);
		if(rc)
		{
			cout<<"Error: Unable to create thread, "<<rc<<endl;
			exit(-1);
		}
	}
	
	rep(i, 0, NUMTHREAD)
		pthread_join(thread_vc[i], NULL);
	
	rep(i, 0, N)
	rep(j, i+1, N)
		sims.push(Similar{i, j, res[i][j]});						
}

int main()
{
	cout<<"Reading Data..."<<endl;
	input();
	cout<<"Reading Data Complete!"<<endl;
	
	cout<<"Processing Data..."<<endl;
	auto begin=chrono::high_resolution_clock::now();
	solve();
	auto end=chrono::high_resolution_clock::now();
	cout<<"Processing Complete!"<<endl;
	
	cout<<"\nTop "<<K<<"-Similar: "<<endl;
	rep(i, 0, K)
	{
		Similar s=sims.top();
		cout<<records[s.id1].obj<<" "<<records[s.id2].obj<<": "<<s.js<<endl;
		sims.pop();
	}
	
	cout<<"\nTime in processing: "<<fixed<<chrono::duration_cast<chrono::microseconds>(end-begin).count()/1e6<<"s";
	return 0;
}

/*
	0 1 2 3 4
0	-
1	  -
2	    -	
3         - 
4           -
*/
