#include <iostream>
#include <string>
#include <ctime>
#include "computer.h"
using namespace std;

int main(int argc, char const *argv[])
{
	clock_t t1, t2;
	t1 = clock();
	int c, w;
	int cm1, cm2;
	int max_priority;
	process ps;
	string cmd;
	cin >> c >> w;
	BinomialHeap *computers = new BinomialHeap[c];
	
	while (cin >> cmd) {
		if (cmd == "assign"){
			cin >> cm1 >> ps.id >> ps.p;
			computers[cm1].insert(ps);
			cout << "There are " << computers[cm1].getSize() << " tasks on computer " << cm1 << "." << endl;
		}
		else if (cmd == "execute") {
			cin >> cm1;
			int samePriCnt = 0;
			int *samePriQ = new int[computers[cm1].getSize()];
			max_priority = computers[cm1].top().p;
			do {
				samePriQ[samePriCnt++] = computers[cm1].pop().id;
			} while(computers[cm1].getSize() > 0 && computers[cm1].top().p == max_priority);

			sort(samePriQ, samePriQ+samePriCnt);
			for (int i=0; i<samePriCnt; i++){
				cout << "Computer " << cm1 << " executed task " << samePriQ[i] << "." << endl;
			}
			delete [] samePriQ;
		}
		else if (cmd == "merge") {
			cin >> cm1 >> cm2;
			if (computers[cm2].getSize() >= w){
				computers[cm1].merge(computers[cm2]);
				cout << "The largest priority number is now " << computers[cm1].top().p << " on " << cm1 << "." << endl;
			}
			else
				cout << "Merging request failed." << endl;
		}
		else {
			cout << "undeclared command, please ensure if you're typing wrong words" << endl;
		}
	}

	delete [] computers;
	t2 = clock();
	cout << "pass time: " << (t2-t1)/(double)CLOCKS_PER_SEC << endl;
	return 0;
}