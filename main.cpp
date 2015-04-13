#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "mymethod.h"
using namespace std;

#define MAXUSERID 24000000
#define MAXADID 24000000

int main(int argc, const char *argv[])
{
	// input data type
	int c, i, a, ad, dep, p, q, k, t, des, u;
	unsigned long long int url;
	ContainerA ca;
	ContainerB cb;

	// vector with UserID as key
	vector<ContainerA>* v = new vector<ContainerA>[MAXUSERID];
	vector<ContainerB>* v2 = new vector<ContainerB>[MAXADID];

	// open input file
	FILE* fin = fopen(argv[1], "r");
	if(!fin){
		printf("Cannot open file.\n");
		exit(1);
	}

	// read input file
	while(fscanf(fin, "%d", &c) != EOF){
		fscanf(fin, "%d%llu%d%d%d%d%d%d%d%d%d",&i,&url,&a,&ad,&dep,&p,&q,&k,&t,&des,&u);
		// reset the content of container
		ca.set(a,q,p,dep,c,i);
		v[u].push_back(ca);
		cb.set(url,ad,k,t,des,u);
		v2[a].push_back(cb);
	}

	// close input file
	fclose(fin);

	// after read in the data, change to command-line interactive mode
	string command;
	int u1, u2;
	double theta;
	cin >> command;
	while(command != "quit"){
	// input "quit" to end the program
		if(command == "get"){
			cin >> u >> a >> q >> p >> dep;
			get(v, u, a, q, p, dep);
		}
		else if(command == "clicked"){
			cin >> u;
			clicked(v, u);
		}
		else if(command == "impressed"){
			cin >> u1 >> u2;
			impressed(v,v2,u1,u2);
		}
		else if(command == "profit"){
			cin >> a >> theta;
			profit(v,v2,a,theta);
		}
		cin >> command;
	}

	// delete the vector array
	delete [] v;
	delete [] v2;

	return 0;
}