#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <time.h>

using namespace std;

#define MAX_FEATURE (1024+1)
#define MAX_EXAMPLE (100000)
#define ERROR 0.000001

struct DL
{
	double data;
	int label;
};

struct TreeNode
{
	int id, decision;
	bool isLeaf;
	TreeNode *right;
	TreeNode *left;
};

void processFile(char const* filename, int*, vector<double*>&, int&, set<int>&);
void processInputString(string, int&, double*, set<int>&);
double confusion(double, double);
double totalConfusion(double, double, double, double);
void computeAllThreshold(int*, vector<double*>&, double*, int, set<int>&);
double computeThreshold(int*, double*, int);
bool comp(DL, DL);
TreeNode* buildTree(double*,double,int*,vector<double*>&, int*, int, set<int>);
void printTree(TreeNode*, int, double*);
void randomShuffle(vector<double*>&,int*,int);


int main(int argc, char const *argv[]) {
	double epsilon = 0.0;
	double* threshold = new double[MAX_FEATURE];
	int* labels = new int[MAX_EXAMPLE];
	vector<double*> dataset;
	set<int> allID;
	int totalExample = 0;	// the amount of examples
	int N = atoi(argv[2]);
	int partExample = N / 3;

	processFile(argv[1], labels, dataset, totalExample, allID);

	/* create shuffle array */
	srand(time(NULL));

	int *datasetMask = new int[partExample];

	printf("int forest_predict(double *attr){\n");
	printf("\tint Y=0, N=0;\n");
	for(int i=0; i<N; i++){
		randomShuffle(dataset, labels, totalExample);
		
		computeAllThreshold(labels, dataset, threshold, partExample, allID);
		
		for(int j=0; j<partExample; j++)
			datasetMask[j] = 1;
		
		/* start to build tree... */
		TreeNode *decisionTree = new TreeNode;
		decisionTree = buildTree(threshold, epsilon, labels, dataset, datasetMask, partExample, allID);

		printTree(decisionTree, 0, threshold);
		printf("\n");
	}
	printf("\treturn (Y>N) ? 1 : -1;\n");
	printf("}\n");

	/* return back memory got from "new" */
	for(unsigned int i=0; i < dataset.size(); ++i)
		delete [] dataset[i];
	delete [] threshold;

	return 0;
}

void processFile(char const* filename, int* labels, vector<double*>& dataset, int& totalExample, set<int>& allID){
	int label, cnt = 0;
	ifstream fin;
	string istr;
	fin.open(filename);

	while (getline(fin, istr)){
		double* data = new double[MAX_FEATURE];
		processInputString(istr, label, data, allID);
		/* label and data refreshed */
		labels[cnt++] = label;
		dataset.push_back(data);
		totalExample++;
	}
	fin.close();
}

void processInputString(string istr, int& label, double* data, set<int>& allID){
	int id;
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
		allID.insert(id);
	}

	delete[] cstr;
}

double confusion(double m, double n){
	if(fabs(m) < ERROR || fabs(n) < ERROR)
		return 0.0;
	else
		return 1.0 - ((m/(m+n)) * (m/(m+n))) - ((n/(m+n)) * (n/(m+n)));
}

double totalConfusion(double a, double b, double c, double d){
	return ((a+b) * confusion(a,b)) / (a+b+c+d) + ((c+d) * confusion(c,d)) / (a+b+c+d);
}

void computeAllThreshold(int* labels, vector<double*>& dataset, double* threshold, int totalExample, set<int>& allID){
	double* data = new double[totalExample];
	/* loop for all features */
	for(set<int>::iterator id=allID.begin(); id!=allID.end(); ++id){
		/* loop for all dataset */
		for(int j=0; j<totalExample; ++j)
			data[j] = dataset[j][*id];
		/* compute the threshold for every feature */
		threshold[*id] = computeThreshold(labels, data, totalExample);
	}
	delete [] data;
}

double computeThreshold(int* labels, double* data, int totalExample){
	int pivot=0;
	int preY=0, preN=0, totalY=0, totalN=0;
	double tmp, smallestConfusion = 1.0;

	DL* dl = new DL[totalExample];
	for(int i=0; i<totalExample; ++i){
		dl[i].data = data[i];
		dl[i].label = labels[i];
		if(labels[i] == 1)
			totalY++;
		else
			totalN++;
	}
	/* sort according to the value of data */
	sort(dl, dl + totalExample, comp);

	double preData = dl[0].data-1;

	tmp = totalConfusion(preY,preN,totalY,totalN);
	if(tmp < smallestConfusion && fabs(tmp - smallestConfusion) > ERROR){
		smallestConfusion = tmp;
		pivot = 0;
	}
	for(int i=0; i<totalExample; ++i){
		if(dl[i].label == 1)
			preY++;
		else
			preN++;

		tmp = totalConfusion(preY, preN, totalY-preY, totalN-preN);
		if(tmp < smallestConfusion && dl[i].data != preData){
			smallestConfusion = tmp;
			pivot = i+1;
		}
		preData = dl[i].data;
	}

	double threshold;
	
	if(pivot == 0)
		threshold = dl[0].data - 1;
	else if(pivot == totalExample)
		threshold = dl[totalExample].data + 1;
	else
		threshold = (dl[pivot-1].data + dl[pivot].data) / 2;

	delete [] dl;
	return threshold;
}

bool comp(DL a, DL b){
	if(a.data < b.data)
		return true;
	else
		return false;
}

TreeNode* buildTree(double* threshold, double epsilon, int* labels, vector<double*>& dataset, int* datasetMask, int totalExample, set<int> currentID){
	/* compute confusion in the examples first... */
	double smallestConfusion = 1.0, tmpConfusion;
	int pivot = 0;
	int preY, preN, postY, postN;
	int pivot_preY=0, pivot_preN=0, pivot_postY=0, pivot_postN=0;
	for(set<int>::iterator id=currentID.begin(); id!=currentID.end(); ++id){
		preY = 0;
		preN = 0;
		postY = 0;
		postN = 0;
		for(int i=0; i<totalExample; ++i){
			if(datasetMask[i] == 1){
				if(dataset[i][*id] <= threshold[*id]){
					if(labels[i] == 1)
						preY++;
					else
						preN++;
				}
				else{
					if(labels[i] == 1)
						postY++;
					else
						postN++;
				}
			}
		}

		tmpConfusion = totalConfusion(double(preY), double(preN), double(postY), double(postN));
		if(tmpConfusion < smallestConfusion){
			smallestConfusion = tmpConfusion;
			pivot = *id;
			pivot_preN = preN;
			pivot_preY = preY;
			pivot_postN = postN;
			pivot_postY = postY;
		}
	}
	/* Now, we have the smallestConfusion and its pivot in the current dataset */

	TreeNode *root = new TreeNode;
	/* confusion smaller than epsilon or cannot branch anymore */
	if( (pivot_preN + pivot_postN) == 0 ){
		root->isLeaf = true;
		root->decision = 1;
	}
	else if( (pivot_preY + pivot_postY) == 0 ){
		root->isLeaf = true;
		root->decision = -1;
	}
	else if( (pivot_preY + pivot_preN) == 0 ){
		root->isLeaf = true;
		if(pivot_postY > pivot_postN)
			root->decision = 1;
		else
			root->decision = -1;
	}
	else if( (pivot_postY + pivot_postN) == 0 ){
		root->isLeaf = true;
		if(pivot_preY > pivot_preN)
			root->decision = 1;
		else
			root->decision = -1;
	}
	else if( (preY == 0 && postN == 0) || (preN == 0 && postY == 0) ){
		root->isLeaf = false;
		root->id = pivot;
		root->left = new TreeNode;
		root->right = new TreeNode;
		root->left->isLeaf = true;
		root->right->isLeaf = true;
		if( preY == 0 && postN == 0){
			root->right->decision = 1;
			root->left->decision = -1;
		}
		else{
			root->right->decision = -1;
			root->left->decision = 1;
		}
	}
	else if(smallestConfusion < epsilon || fabs(smallestConfusion - epsilon) < ERROR){
		root->isLeaf = true;
		if((pivot_preY + pivot_postY) > (pivot_preN + pivot_postN))
			root->decision = 1;
		else
			root->decision = -1;
	}
	else{
		/* dataset can be branched to two new tree */
		root->isLeaf = false;
		root->id = pivot;

		/* record the mask first */
		int *tmp_datasetMask = new int[totalExample];
		for(int i=0; i<totalExample; ++i)
			tmp_datasetMask[i] = datasetMask[i];

		for(int i=0; i<totalExample; ++i){
			if(tmp_datasetMask[i] == 1){
				/* if data smaller than threshold */
				if(dataset[i][pivot] > threshold[pivot])
					datasetMask[i] = 0;
			}
		}
		root->left = buildTree(threshold, epsilon, labels, dataset, datasetMask, totalExample, currentID);

		for(int i=0; i<totalExample; ++i)
			datasetMask[i] = tmp_datasetMask[i];

		for(int i=0; i<totalExample; ++i){
			if(tmp_datasetMask[i] == 1){
				/* if data smaller than threshold */
				if(dataset[i][pivot] <= threshold[pivot])
					datasetMask[i] = 0;
			}
		}
		root->right = buildTree(threshold, epsilon, labels, dataset, datasetMask, totalExample, currentID);
	}

	return root;
}

void printTree(TreeNode* root, int depth, double* threshold){
	if(root->isLeaf){
		for(int i=0; i<=depth; i++)
			printf("\t");
		
		if(root->decision == 1)
			printf("Y++;\n");
		else
			printf("N++;\n");
	}
	else{
		for(int i=0; i<=depth; i++)
			printf("\t");
		printf("if(attr[%d] <= %f){\n", root->id, threshold[root->id]);
		
		printTree(root->left, depth+1, threshold);
		
		for(int i=0; i<=depth; i++)
			printf("\t");
		printf("}\n");

		for(int i=0; i<=depth; i++)
			printf("\t");
		printf("else{\n");

		printTree(root->right, depth+1, threshold);

		for(int i=0; i<=depth; i++)
			printf("\t");
		printf("}\n");
	}
}

void randomShuffle(vector<double*>& dataset, int* labels, int totalExample){
	unsigned int k;
	double* tmp_d;
	int tmp_i;
	for(unsigned int i=totalExample-1; i>0; --i){
		k = rand() % (i+1);
		if(k != i){
			tmp_d = dataset[k];
			dataset[k] = dataset[i];
			dataset[i] = tmp_d;

			tmp_i = labels[k];
			labels[k] = labels[i];
			labels[i] = tmp_i;
		}
	}
}