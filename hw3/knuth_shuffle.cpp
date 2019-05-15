#include <iostream>
#include <cstdlib>
#include <algorithm>
#include <iterator>
#include <ctime>
using namespace std;

template<typename RandomAccessIterator>
void knuthShuffle(RandomAccessIterator begin, RandomAccessIterator end) {
	for(unsigned int n = end - begin - 1; n >= 1; --n) {
		unsigned int k = rand() % (n + 1);
		if(k != n) {
			std::iter_swap(begin + k, begin + n);
		}
	}
}

int main(int argc, char const *argv[])
{
	int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	cout << "origin array is: ";
	for(int i=0; i<10; i++)
		cout << array[i] << " ";
	cout << endl;

	srand(time(NULL));
	knuthShuffle(array, array+10);
	
	cout << "after shuffle is: ";
	for(int i=0; i<10; i++)
		cout << array[i] << " ";
	cout << endl;
	return 0;
}