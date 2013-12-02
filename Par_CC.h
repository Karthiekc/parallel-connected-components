#ifndef _PAR_CC_H
#define _PAR_CC_H

#include<vector>
#include<cilk.h>
#include <map>
#include <algorithm>

#define HEAD true
#define TAIL false

using namespace std;

struct Edge{
	int u,v;
};

void Par_Prefix_Sum(vector<int> &arr, vector<int> &out){
	int len = arr.size();

	if(len == 1){
		out[0] = arr[0];	
		return;
	}
	
	vector<int> y(len/2);
	vector<int> z(len/2);
	
	cilk_for(int i=0;i<len/2;i++){
		y[i] = arr[2*i]+arr[2*i+1];	
	}

	Par_Prefix_Sum(y,z);

	cilk_for(int i=0;i<len;i++){
		if(i==0)
			out[i] = arr[i];
		else if(i%2 != 0)
			out[i] = z[i/2];
		else
			out[i] = z[(i-1)/2] + arr[i];
		
	}
}

void get_new_edge_set(vector<Edge> &E, vector<int> &L, vector<Edge> &F){
	int m = E.size();
	vector<int> S(m);
	vector<int> Sout(m);
	cilk_for(int i=0;i< m;i++){
		if(L[E[i].u] != L[E[i].v])
			S[i] = 1;
		else
			S[i] = 0;
	}
	Par_Prefix_Sum(S, Sout);
	vector<int>().swap(S);//free
	
	int new_e_size = Sout[m-1];
	F.resize(new_e_size);

	cilk_for(int i=0;i<m;i++){
		if(L[E[i].u] != L[E[i].v]){
			F[Sout[i]-1].u = L[E[i].u];
			F[Sout[i]-1].v = L[E[i].v];
		}
	}
	vector<int>().swap(Sout);//free	
}

void get_new_vertex_set(vector<int> &V, vector<int> &L, vector<int> &V1){
	int n = V.size();
	vector<int> Vs(n), Vsout(n);
	cilk_for(int i=0;i< n;i++){
		if(L[V[i]] == V[i])
			Vs[i] = 1;
		else
			Vs[i] = 0;
	}

	Par_Prefix_Sum(Vs, Vsout);
	
	vector<int>().swap(Vs);//free

	int new_v_size = Vsout[n-1];
	V1.resize(new_v_size);

	cilk_for(int i=0;i< n;i++){
		if(L[V[i]] == V[i]){
			V1[Vsout[i]-1] = V[i];
		}
	}

	vector<int>().swap(Vsout);//free
}

bool compare_func(int i, int j){
	return i>j;
}

void print_results(vector<int> &L){
	map<int, int> CC;
	vector<int> result;
	for(int k=1;k<L.size();k++)
		CC[L[k]]++;
	
	for(map<int,int>::iterator m_iter = CC.begin();m_iter != CC.end();++m_iter){
		result.push_back(m_iter->second);
	}

	sort(result.begin(), result.end(), compare_func);	
	cout<<result.size()<<endl;
	
	for(int i=0;i<result.size();i++)
		cout<<result[i]<<endl;
}

#endif
