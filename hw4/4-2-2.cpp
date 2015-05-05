#include <cstdlib>
#include <cstdio>
#include <cstr>
#include <fstream>
#include <vector>

using namespace std;

#define MAX_FEATURE (1024+1)

int main(int argc, const char* argv[]) {
	double* data = new double[MAX_FEATURE];

	ifstream fin;
	string istr;
	fin.open(argv[1]);

	double epsilon = atof(argv[2]);
	char *tmp;
	int label;

	printf("epsilon= %lf\n", epsilon);

	while (getline(fin, istr)) {
		memset(data, 0, sizeof(double) * MAX_FEATURE);

		char *cstr = new char[istr.size() + 1];
		strncpy(cstr, istr.c_str(), istr.size()+1);

		tmp =  strtok(cstr, ": ");
		label = atoi(tmp);
		tmp = strtok(NULL, ": ");

		printf("label= %d\n", label);

		while(tmp != NULL) {
			int id = atoi(tmp);
			tmp = strtok(NULL, ": ");
			data[id] = atof(tmp);
			tmp = strtok(NULL, ": ");
			printf("id= %d, data= %lf\n", id, data[id]);
		}

		delete[] cstr;
	}
	delete[] data;
}