#include <iostream>
#include <vector>
#include <cilk.h>
#include <stdlib.h>
#include "Par_CC.h"

using namespace std;

int iter = 0;
bool *C;
int VCOUNT;
vector<int> L;

void Par_Randomized_CC1(vector<int> &V, vector<Edge> &E) {
	if(E.size() == 0)
		return;
	
	iter++;
	int n = V.size();
	int m = E.size();
	vector<int> S(m);

	cilk_for(int i=0;i<n;i++){
		double r = (double)rand()/RAND_MAX;
		C[V[i]] = (r>=0.5)?HEAD:TAIL;
	}	
	cilk_for(int i=0;i<m;i++){
		int u = E[i].u;
		int v = E[i].v;
		if(C[u] == TAIL && C[v] == HEAD)
			L[u] = L[v];
	}

	cilk_for(int i=0;i<m;i++){
		if(L[E[i].u] != L[E[i].v])
			S[i] = 1;
		else
			S[i] = 0;
	}

	vector<Edge> F;
	get_new_edge_set(E, L, F);

	Par_Randomized_CC1(V, F);

	vector<Edge>().swap(F);//free

	cilk_for(int i=0;i<m;i++){
		int u = E[i].u;
		int v = E[i].v;
		if(v == L[u])
			L[u] = L[v];
	}
}


int cilk_main(int argc, char* argv[]){
	int n, m;
	
	scanf("%d %d", &n, &m);
	VCOUNT = n;	
	vector<int> V(n);
	vector<Edge> E(m);
	
	L.resize(n+1);
	C = new bool[n+1];

	cilk_for(int i=0; i<n; i++){
		V[i] = i+1;
		L[i+1] = i+1;
	}
	
	for(int j=0;j<m;j++){
		scanf("%d %d", &(E[j].u), &(E[j].v));
	}
	
	Par_Randomized_CC1(V, E);
	
	delete[] C;

	print_results(L);
	
	return 0;
}
