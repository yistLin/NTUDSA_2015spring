#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#define MAX_FEATURE (1024+1)
#define MAX_DATASET 200

struct DL{
	double data;
	int label;
};

void processFile(char const* filename, int*, vector<double*>&);
void processInputString(string, int&, double*);
double confusion(int, int);
double totalConfusion(int, int, int, int);
void computeAllThreshold(int*, vector<double*>&, double*);
double computeThreshold(int*, double*);
bool comp(DL, DL);
void buildTree(double*,double,int*,vector<double*>&);

int main(int argc, char const *argv[]) {
	double epsilon = atof(argv[2]);
	double* threshold = new double[MAX_FEATURE];
	int* labels = new int[MAX_DATASET];
	vector<double*> dataset;

	processFile(argv[1], labels, dataset);

	computeAllThreshold(labels, dataset, threshold);
	
	for(int i=0; i<dataset[0][0]; ++i)
		printf("threshold = %.6f\n", threshold[i]);

	/* start to build tree... */
	printf("int tree_predict(double *attr){\n");
	buildTree(threshold, epsilon, labels, dataset);
	printf("}\n");

	/* return back memory got from "new" */
	for(int i=0; i<dataset.size(); ++i)
		delete [] dataset[i];
	delete [] threshold;

	return 0;
}

void processFile(char const* filename, int* labels, vector<double*>& dataset){
	int label, cnt = 0;
	ifstream fin;
	string istr;
	fin.open(filename);

	while (getline(fin, istr)){
		double* data = new double[MAX_FEATURE];
		processInputString(istr, label, data);
		/* label and data refreshed */
		labels[cnt++] = label;
		dataset.push_back(data);
	}
}

void processInputString(string istr, int& label, double* data){
	int id;
	int sizeOfData = 0;
	char *cstr = new char[istr.size() + 1];
	strncpy(cstr, istr.c_str(), istr.size()+1);

	/* token label from string */
	char *tmp =  strtok(cstr, ": ");
	label = atoi(tmp);
	tmp = strtok(NULL, ": ");

	while(tmp != NULL) {
		/* token data of every feature from string */
		id = atoi(tmp);
		tmp = strtok(NULL, ": ");
		data[id] = atof(tmp);
		tmp = strtok(NULL, ": ");
		sizeOfData++;
	}
	data[0] = sizeOfData;

	delete[] cstr;
}

double confusion(double m, double n){
	return 1.0 - (m/(m+n)) * (m/(m+n)) - (n/(m+n)) * (n/(m+n));
}

double totalConfusion(int e, int f, int g, int h){
	double a = e, b = f, c = g, d = h;
	return (a+b) * confusion(a,b) / (a+b+c+d) + (c+d) * confusion(c,d) / (a+b+c+d);
}

void computeAllThreshold(int* labels, vector<double*>& dataset, double* threshold){
	double* data = new double[MAX_DATASET];
	/* loop for all features */
	for(int id=1; id<=dataset[0][0]; ++id){
		/* loop for all dataset */
		for(int j=0; j<MAX_DATASET; ++j)
			data[j] = dataset[j][id];
		/* compute the threshold for every feature */
		threshold[id-1] = computeThreshold(labels, data);
	}
	/*
	for(int i=0; i<=dataset[0][0]; ++i)
		printf("threshold[%d] = %.6f\n", i, threshold[i]);
	*/
	delete [] data;
}

double computeThreshold(int* labels, double* data){
	int pivot=0;
	int preY=0, preN=0, totalY=0, totalN=0;
	double tmp, smallestConfusion = 1.0;

	DL* dl = new DL[MAX_DATASET];
	for(int i=0; i<MAX_DATASET; ++i){
		dl[i].data = data[i];
		dl[i].label = labels[i];
		if(labels[i] == 1)
			totalY++;
		else
			totalN++;
	}
	/* sort according to the value of data */
	sort(dl, dl+MAX_DATASET, comp);

	tmp = totalConfusion(preY,preN,totalY,totalN);
	if(tmp <= smallestConfusion){
		smallestConfusion = tmp;
		pivot = 0;
	}
	for(int i=0; i<MAX_DATASET; ++i){
		if(dl[i].label == 1)
			preY++;
		else
			preN++;

		tmp = totalConfusion(preY, preN, totalY-preY, totalN-preN);
		if(tmp <= smallestConfusion){
			smallestConfusion = tmp;
			pivot = i+1;
		}
	}

	double threshold;
	
	if(pivot == 0)
		threshold = dl[0].data - 1;
	else if(pivot == MAX_DATASET)
		threshold = dl[MAX_DATASET].data + 1;
	else
		threshold = (dl[pivot].data + dl[pivot+1].data) / 2;

	delete [] dl;
	return threshold;
}

bool comp(DL a, DL b){
	return a.data < b.data;
}

void buildTree(double* , double epsilon, int* labels, vector<double*>& dataset){
	/* work need to be done... */
}