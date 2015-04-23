#include <iostream>
#include <stack>
#include <string>

#define MAXLENGTH 1000000

using namespace std;

int main(int argc, char const *argv[])
{
	stack<int> stack_n;
	stack<char> stack_o;

	char entireLine[MAXLENGTH];
	int int_buf;
	int it;
	bool processingInt;
	
	// process all the input from stdin
	while(cin.getline(entireLine, MAXLENGTH)){
		it = 0;
		int_buf = 0;
		processingInt = false;
		
		// process input from line
		while(entireLine[it] != '\0'){
			if(isdigit(entireLine[it])){
				if(processingInt){
					int_buf *= 10;
					int_buf += (int)entireLine[it] - 48;
				}
				else{
					processingInt = true;
					int_buf = (int)entireLine[it] - 48;
				}
			}
			else{
				if(processingInt){
					processingInt = false;
					stack_n.push(int_buf);
					int_buf = 0;
				}
				if(entireLine[it] != ' ')
					stack_o.push(entireLine[it]);
			}
			it++;
		}
		// if an Integer is at the end of line
		if(processingInt){
			stack_n.push(int_buf);
		}

		/*
		// output from the stack
		cout << "content of stack_n: ";
		while(!stack_n.empty()){
			cout << "[" << stack_n.top() << "] ";
			stack_n.pop();
		}
		cout << endl;
		cout << "content of stack_o: ";
		while(!stack_o.empty()){
			cout << "[" << stack_o.top() << "] ";
			stack_o.pop();
		}
		cout << endl;
		*/
	}

	return 0;
}