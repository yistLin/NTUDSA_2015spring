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

#define MAX_FEATURE (10000)
#define MAX_EXAMPLE (1000000)
#define ERROR 0.000001

struct DL
{
	double data;
	int label;
};

struct TreeNode
{
	int id, decision;
	double threshold;
	bool isLeaf;
	TreeNode *right;
	TreeNode *left;
};

void processFile(char const* filename, int*, vector<double*>&, int&, set<int>&);
void randomShuffle(vector<double*>&, int*, int);
double confusion(double, double);
double totalConfusion(double, double, double, double);
double computeThreshold(vector<DL>&, double&);
bool comp(DL, DL);
TreeNode* buildTree(double,int*,vector<double*>&, int*, int, set<int>);
void printTree(TreeNode*, int);

int main(int argc, char const *argv[]) {
	double epsilon = 0.0;
	int N = atoi(argv[2]);
	int* labels = new int[MAX_EXAMPLE];
	vector<double*> dataset;
	set<int> allID;
	int totalExample = 0;	// the amount of examples

	srand(time(NULL));

	processFile(argv[1], labels, dataset, totalExample, allID);

	int partExample = totalExample / 3;
	int *datasetMask = new int[partExample];
	TreeNode *decisionTree;

	printf("int forest_predict(double *attr){\n");
	printf("    int Y = 0, N = 0;\n");

	for(int n=0; n<N; ++n){
		for(int i=0; i<partExample; i++)
			datasetMask[i] = 1;

		randomShuffle(dataset, labels, totalExample);
		decisionTree = buildTree(epsilon, labels, dataset, datasetMask, partExample, allID);
		printf("tree%d_predict:\n", n+1);
		printTree(decisionTree, 0);
		printf("\n");
	}

	printf("voting:\n");
	printf("    return (Y > N) ? 1 : -1;\n");
	printf("}\n");

	/* return back memory got from "new" */
	for(unsigned int i=0; i < dataset.size(); ++i)
		delete [] dataset[i];
	delete [] labels;
	delete [] datasetMask;

	return 0;
}

void processFile(char const* filename, int* labels, vector<double*>& dataset, int& totalExample, set<int>& allID){
	int label, cnt = 0, id;
	char *tmp;
	ifstream fin;
	string istr;
	fin.open(filename);

	while (getline(fin, istr)){
		double* data = new double[MAX_FEATURE];
		char *cstr = new char[istr.size() + 1];
		strncpy(cstr, istr.c_str(), istr.size()+1);

		/* token label from string */
		tmp =  strtok(cstr, ": ");
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

		/* label and data refreshed */
		labels[cnt++] = label;
		dataset.push_back(data);
	}
	totalExample = cnt;
	fin.close();
}

void randomShuffle(vector<double*>& dataset, int* labels, int totalExample){
	int k = 0;
	double* tmp_d;
	int tmp_l;
	for(int i=totalExample-1; i>0; --i){
		k = rand()%(i+1);
		if(k != i){
			tmp_d = dataset[i];
			dataset[i] = dataset[k];
			dataset[k] = tmp_d;

			tmp_l = labels[i];
			labels[i] = labels[k];
			labels[k] = tmp_l;
		}
	}
}

double confusion(double m, double n){
	if(m == 0 && n == 0)
		return 1.0;
	else
		return 1.0 - ((m/(m+n)) * (m/(m+n))) - ((n/(m+n)) * (n/(m+n)));
}

double totalConfusion(double a, double b, double c, double d){
	if((a == 0 && b == 0) || (c == 0 && d == 0))
		return 1.0;

	return ((a+b) * confusion(a,b)) / (a+b+c+d) + ((c+d) * confusion(c,d)) / (a+b+c+d);
}

double computeThreshold(vector<DL>& dl, double& confusion){
	int pivot=0;
	int preY=0, preN=0, totalY=0, totalN=0;
	double tmp, smallestConfusion = 1.0;

	/* for some protect */
	if(dl.size() == 1){
		confusion = 1.0;
		return dl[0].data-1;
	}

	/* sort according to the value of data */
	sort(dl.begin(), dl.end(), comp);

	for(unsigned int i=0; i<dl.size(); ++i){
		if(dl[i].label == 1)
			totalY++;
		else
			totalN++;
	}

	for(unsigned int i=0; i<dl.size()-1; ++i){
		if(dl[i].label == 1)
			preY++;
		else
			preN++;

		if(dl[i].data != dl[i+1].data){
			tmp = totalConfusion(preY, preN, totalY-preY, totalN-preN);
			if(tmp < smallestConfusion){
				smallestConfusion = tmp;
				pivot = i;
			}
		}
	}
	confusion = smallestConfusion;

	double threshold;

	threshold = (dl[pivot].data + dl[pivot+1].data) / 2;

	return threshold;
}

bool comp(DL a, DL b){
	if(a.data < b.data)
		return true;
	else
		return false;
}

TreeNode* buildTree(double epsilon, int* labels, vector<double*>& dataset, int* datasetMask, int totalExample, set<int> currentID){
	/* compute confusion in the examples first... */
	double smallestConfusion = 1.0;
	double *threshold = new double[MAX_FEATURE];
	int pivot = 0;
	int preY = 0, preN = 0;
	int totalY = 0, totalN = 0;

	for(int i=0; i<totalExample; ++i){
		if(datasetMask[i] == 1){
			if(labels[i] == 1)
				totalY++;
			else
				totalN++;
		}
	}

	TreeNode *root = new TreeNode;

	if(confusion(totalY, totalN) <= epsilon){
		root->isLeaf = true;
		if(totalY > totalN)
			root->decision = 1;
		else if(totalY == totalN)
			root->decision = (rand()%2)?1:-1;
		else
			root->decision = -1;
	}
	else{
		vector<DL> dl;
		dl.reserve(totalExample);

		DL tmp;
		double* confusions = new double[MAX_FEATURE];
		pivot = *(currentID.begin());
		for(set<int>::iterator id=currentID.begin(); id!=currentID.end(); ++id){
			/* loop for all dataset */
			for(int j=0; j<totalExample; ++j){
				if(datasetMask[j] == 1){
					tmp.data = dataset[j][*id];
					tmp.label = labels[j];
					dl.push_back(tmp);
				}
			}
			/* compute the threshold for every feature */
			threshold[*id] = computeThreshold(dl,confusions[*id]);
			if(confusions[*id] < smallestConfusion){
				smallestConfusion = confusions[*id];
				pivot = *id;
			}
			dl.clear();
		}

		for(int i=0; i<totalExample; ++i){
			if(datasetMask[i] == 1){
				if(dataset[i][pivot] <= threshold[pivot]){
					if(labels[i] == 1)
						preY++;
					else
						preN++;
				}
			}
		}
		if((preY == 0) && (preN == 0)){
			root->isLeaf = true;
			if(totalY > totalN)
				root->decision = 1;
			else if(totalY == totalN)
				root->decision = (rand()%2)?1:-1;
			else
				root->decision = -1;
		}
		else if((totalY == preY) &&  (totalN == preN)){
			root->isLeaf = true;
			if(preY > preN)
				root->decision = 1;
			else if(preY == preN)
				root->decision = (rand()%2)?1:-1;
			else
				root->decision = -1;
		}
		else{
			root->isLeaf = false;
			root->id = pivot;
			root->threshold = threshold[pivot];
			int *tmp_datasetMask = new int[totalExample];
			for(int i=0; i<totalExample; ++i)
				tmp_datasetMask[i] = datasetMask[i];

			for(int i=0; i<totalExample; ++i){
				if(datasetMask[i] == 1){
					if(dataset[i][pivot] > (root->threshold)){
						tmp_datasetMask[i] = 0;
					}
				}
			}
			root->left = buildTree(epsilon, labels, dataset, tmp_datasetMask, totalExample, currentID);

			for(int i=0; i<totalExample; ++i)
				tmp_datasetMask[i] = datasetMask[i];

			for(int i=0; i<totalExample; ++i){
				if(datasetMask[i] == 1){
					if(dataset[i][pivot] <= (root->threshold)){
						tmp_datasetMask[i] = 0;
					}
				}
			}
			root->right = buildTree(epsilon, labels, dataset, tmp_datasetMask, totalExample, currentID);

			delete [] tmp_datasetMask;
		}
	}

	delete [] threshold;
	return root;
}

void printTree(TreeNode* root, int depth){
	if(root->isLeaf){
		for(int i=0; i<=depth; i++)
			printf("    ");
		if(root->decision == 1)
			printf("Y++;\n");
		else
			printf("N++;\n");
	}
	else{
		for(int i=0; i<=depth; i++)
			printf("    ");
		printf("if(attr[%d] <= %f){\n", root->id, root->threshold);
		
		printTree(root->left, depth+1);
		
		for(int i=0; i<=depth; i++)
			printf("    ");
		printf("}\n");

		for(int i=0; i<=depth; i++)
			printf("    ");
		printf("else{\n");

		printTree(root->right, depth+1);

		for(int i=0; i<=depth; i++)
			printf("    ");
		printf("}\n");
	}
}