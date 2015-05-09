#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <vector>
#include <set>
#include <algorithm>

using namespace std;

#define MAX_FEATURE (1024+1)
#define MAX_EXAMPLE (10000)

int buildTree_cnt;

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

int main(int argc, char const *argv[]) {
	double epsilon = atof(argv[2]);
	double* threshold = new double[MAX_FEATURE];
	int* labels = new int[MAX_EXAMPLE];
	vector<double*> dataset;
	set<int> allID;
	int totalExample = 0;	// the amount of examples

	processFile(argv[1], labels, dataset, totalExample, allID);

	computeAllThreshold(labels, dataset, threshold, totalExample, allID);
	/*
	for(set<int>::iterator i=allID.begin(); i!=allID.end(); ++i)
		printf("threshold = %.6f\n", threshold[*i]);
	printf("\n\n");
	*/
	int *datasetMask = new int[totalExample];
	for(int i=0; i<totalExample; i++)
		datasetMask[i] = 1;

	/* start to build tree... */
	buildTree_cnt = 0;
	TreeNode *decisionTree = buildTree(threshold, epsilon, labels, dataset, datasetMask, totalExample, allID);

	printf("int tree_predict(double *attr){\n");
	printTree(decisionTree, 0, threshold);
	printf("}\n");

	/* return back memory got from "new" */
	for(int i=0; i<dataset.size(); ++i)
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
	if((m+n) == 0)
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

	tmp = totalConfusion(preY,preN,totalY,totalN);
	if(tmp <= smallestConfusion){
		smallestConfusion = tmp;
		pivot = 0;
	}
	for(int i=0; i<totalExample; ++i){
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
	else if(pivot == totalExample)
		threshold = dl[totalExample].data + 1;
	else
		threshold = (dl[pivot].data + dl[pivot+1].data) / 2;

	delete [] dl;
	return threshold;
}

bool comp(DL a, DL b){
	return a.data < b.data;
}

TreeNode* buildTree(double* threshold, double epsilon, int* labels, vector<double*>& dataset, int* datasetMask, int totalExample, set<int> currentID){
	/* compute confusion in the examples first... */
	/*
	printf("run buildTree...\n");
	buildTree_cnt++;
	if(buildTree_cnt > 20){
		TreeNode *root = new TreeNode;
		root->isLeaf = true;
		root->decision = 10000;
		return root;
	}
	*/
	double smallestConfusion = 1.0, tmpConfusion;
	int pivot = 0;
	int preY, preN, postY, postN;
	int pivot_preY, pivot_preN, pivot_postY, pivot_postN;
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
		//printf("tmpConfusion = %.6f\n", tmpConfusion);
		//printf("preY=%d, preN=%d, postY=%d, postN=%d\n", preY,preN,postY,postN);
		if(tmpConfusion <= smallestConfusion){
			smallestConfusion = tmpConfusion;
			pivot = *id;
			pivot_preN = preN;
			pivot_preY = preY;
			pivot_postN = postN;
			pivot_postY = postY;
		}
	}
	/* Now, we have the smallestConfusion and its pivot in the current dataset */

	//printf("smallestConfusion = %.6f, pivot = %d, preY = %d, preN = %d, postY = %d, postN = %d\n", smallestConfusion, pivot, pivot_preY, pivot_preN, pivot_postY, pivot_postN);	

	TreeNode *root = new TreeNode;
	
	/* confusion smaller than epsilon or cannot branch anymore */
	if( (pivot_preY + pivot_preN) == 0 ){
		//printf("preY+preN = 0\n");
		root->isLeaf = true;
		if(pivot_postY > pivot_postN)
			root->decision = 1;
		else
			root->decision = -1;
	}
	else if( (pivot_postY + pivot_postN) == 0 ){
		//printf("postY+postN = 0\n");
		root->isLeaf = true;
		if(pivot_preY > pivot_preN)
			root->decision = 1;
		else
			root->decision = -1;
	}
	else if(smallestConfusion <= epsilon || currentID.size() <= 1){
		//printf("smallestConfusion <= epsilon or currentID.size() <= 1\n");
		root->isLeaf = true;
		if((pivot_preY + pivot_postY) > (pivot_preN + pivot_postN))
			root->decision = 1;
		else
			root->decision = -1;
	}
	else{
		/* dataset can be branched to two new tree */
		root->isLeaf = false;
		//printf("treenode isn't leaf, branching...\n");
		root->id = pivot;
		//currentID.erase(pivot);

		/* record the mask first */
		int *tmp_datasetMask = new int[totalExample];
		for(int i=0; i<totalExample; ++i)
			tmp_datasetMask[i] = datasetMask[i];

		for(int i=0; i<totalExample; ++i){
			if(tmp_datasetMask[i] == 1){
				/* if data smaller than threshold */
				if(dataset[i][pivot] <= threshold[pivot])
					datasetMask[i] = 0;
			}
		}

		root->left = buildTree(threshold, epsilon, labels, dataset, datasetMask, totalExample, currentID);

		for(int i=0; i<totalExample; ++i)
			datasetMask[i] = tmp_datasetMask[i];

		for(int i=0; i<totalExample; ++i){
			if(tmp_datasetMask[i] == 1){
				/* if data smaller than threshold */
				if(dataset[i][pivot] > threshold[pivot])
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
		printf("return %d;\n", root->decision);
	}
	else{
		for(int i=0; i<=depth; i++)
			printf("\t");
		printf("if(attr[%d] > %.6f){\n", root->id, threshold[root->id]);
		
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