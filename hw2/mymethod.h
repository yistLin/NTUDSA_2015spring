#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <set>
#include <algorithm>
using namespace std;

class ContainerA {
public:
	int adid, queryid, position, depth, click, impression;
	ContainerA();
	~ContainerA();
	void set(const int&,const int&,const int&,const int&,const int&,const int&);
};

class ContainerB {
public:
	unsigned long long int url;
	int advertiserid, keywordid, titleid, descriptionid, userid;
	ContainerB();
	~ContainerB();
	void set(const unsigned long long int&,const int&,const int&,const int&,const int&,const int&);
};

struct SmallContainer
{
	int adid, queryid;
	bool operator<(const SmallContainer& a) const;
};

void get(vector<ContainerA>*,const int&,const int&,const int&,const int&,const int&);
void clicked(vector<ContainerA>*,const int&);
void impressed(vector<ContainerA>*,vector<ContainerB>*,const int&,const int&);
void profit(vector<ContainerA>*,vector<ContainerB>*,const int&,const double&);