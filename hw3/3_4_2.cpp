#include <iostream>
#include <stack>
#include <map>
#include <cstdio>
#include <cmath>
using namespace std;

#define MAXLENGTH 1000000

void readFloat(const char*,stack<double>&,int&);
void readFunc(const char*,stack<int>&,int&);
int readOp(int&,bool&);
void processInputOp(stack<double>&,stack<int>&,int);
void processOp(stack<double>&,stack<int>&);

// operator precedence
enum {
	// left and right parenthesis
	LEFT_PARENTHESIS = 5,
	RIGHT_PARENTHESIS = 15,
	// special functions
	SIN = 4,
	COS = 14,
	EXP = 24,
	LOG = 34,
	POW = 44,
	SQRT = 54,
	FABS = 64,
	// normal operators
	PLUS = 3,
	MINUS = 13,
	MULTIPLY = 2,
	ADD = 1,
	SUBTRACT = 11,
	COMMA = 0
};

int main(int argc, char const *argv[])
{
	stack<double> d;
	stack<int> o;

	map<char,int> m;
	m.insert(pair<char,int>('+',ADD));
	m.insert(pair<char,int>('-',SUBTRACT));
	m.insert(pair<char,int>('*',MULTIPLY));
	m.insert(pair<char,int>('(',LEFT_PARENTHESIS));
	m.insert(pair<char,int>(')',RIGHT_PARENTHESIS));
	m.insert(pair<char,int>(',',COMMA));

	int i;
	bool behindNumber;
	char entireLine[MAXLENGTH];

	while(cin.getline(entireLine, MAXLENGTH)){
		// set iterator to zero
		i = 0;
		behindNumber = false;

		cout << "Postfix Exp: ";
		while(entireLine[i] != '\0'){
			if(isdigit(entireLine[i]) || entireLine[i] == '.'){
				// input format is: line, stack, iterator
				readFloat(entireLine, d, i);
				behindNumber = true;
			}

			else if(isalpha(entireLine[i])){
				// input format is: line, stack, iterator
				readFunc(entireLine, o, i);
				behindNumber = false;
			}
			else if(entireLine[i] != ' '){
				// input format is: stack, stack, int(operator)
				processInputOp(d,o,readOp(m[entireLine[i]],behindNumber));
				
				// change behindNumber
				if(entireLine[i] == ')')
					behindNumber = true;
				else
					behindNumber = false;
				i++;
			}
			else	// if read-in is white-space, just pass it
				i++;
		}
		while(!o.empty()){
			processOp(d,o);
		}
		cout << endl;
		printf("RESULT: %.6f\n", d.top());
		d.pop();
	}
	return 0;
}

void readFloat(const char* line,stack<double>& d,int& i){
	double d_buf = 0;
	int decimal_level;
	while(isdigit(line[i])){
		d_buf *= 10;
		d_buf += (int)line[i] - 48;
		i++;
	}
	// if input format is a float number
	if(line[i] == '.'){
		i++;	// pass the dot
		decimal_level = -1;
		while(isdigit(line[i])){
			d_buf += ((int)line[i] - 48) * pow(10,decimal_level);
			i++;
			decimal_level--;
		}
	}
	printf("%.6f ", d_buf);
	d.push(d_buf);
}

void readFunc(const char* line,stack<int>& o,int& i){
	switch(line[i]){
		case 's':
			if(line[i+1] == 'i'){
				// read-in sin()
				//printf("sin()\n");
				o.push(SIN);
				i += 3;
			}
			else{
				// read-in sqrt()
				//printf("sqrt()\n");
				o.push(SQRT);
				i += 4;
			}
			break;
		case 'c':
			// read-in cos()
			//printf("cos()\n");
			o.push(COS);
			i += 3;
			break;
		case 'e':
			// read-in exp()
			//printf("exp()\n");
			o.push(EXP);
			i += 3;
			break;
		case 'l':
			// read-in log()
			//printf("log()\n");
			o.push(LOG);
			i += 3;
			break;
		case 'p':
			// read-in pow()
			//printf("pow()\n");
			o.push(POW);
			i += 3;
			break;
		case 'f':
			// read-in fabs()
			//printf("fabs()\n");
			o.push(FABS);
			i += 4;
			break;
	}
}

int readOp(int& op,bool& behindNumber){
	if(op == ADD)
		return behindNumber ? ADD : PLUS;
	else if(op == SUBTRACT)
		return behindNumber ? SUBTRACT : MINUS;
	else
		return op;
}

void processInputOp(stack<double>& d,stack<int>& o,int op){
	if(o.empty() || d.empty())
		o.push(op);
	else if(op == RIGHT_PARENTHESIS){
		while(!o.empty() && o.top() != LEFT_PARENTHESIS)
			processOp(d,o);
		o.pop();
	}
	else if((o.top() != LEFT_PARENTHESIS) && ((op%10) <= (o.top()%10)) ){
		while(!o.empty() && !d.empty() && (o.top() != LEFT_PARENTHESIS) && ((op%10) <= (o.top()%10)))
			processOp(d,o);
		o.push(op);
	}
	else{
		o.push(op);
	}
}

void processOp(stack<double>& d,stack<int>& o){
	if(o.empty())
		return;
	if(d.empty())
		return;

	double a, b;
	int op = o.top();
	o.pop();

	switch(op){
		case SIN:
			cout << "sin ";
			a = d.top();
			d.pop();
			a = sin(a);
			d.push(a);
			break;
		case COS:
			cout << "cos ";
			a = d.top();
			d.pop();
			a = cos(a);
			d.push(a);
			break;
		case EXP:
			cout << "exp ";
			a = d.top();
			d.pop();
			a = exp(a);
			d.push(a);
			break;
		case LOG:
			cout << "log ";
			a = d.top();
			d.pop();
			a = log(a);
			d.push(a);
			break;
		case POW:
			cout << "pow ";
			b = d.top();
			d.pop();
			a = d.top();
			d.pop();
			a = pow(a, b);
			d.push(a);
			break;
		case SQRT:
			cout << "sqrt ";
			a = d.top();
			d.pop();
			a = sqrt(a);
			d.push(a);
			break;
		case FABS:
			cout << "fabs ";
			a = d.top();
			d.pop();
			a = fabs(a);
			d.push(a);
			break;
		case ADD:
			cout << "+ ";
			b = d.top();
			d.pop();
			a = d.top();
			d.pop();
			a = a + b;
			d.push(a);
			break;
		case SUBTRACT:
			cout << "- ";
			b = d.top();
			d.pop();
			a = d.top();
			d.pop();
			a = a - b;
			d.push(a);
			break;
		case MULTIPLY:
			cout << "* ";
			b = d.top();
			d.pop();
			a = d.top();
			d.pop();
			a = a * b;
			d.push(a);
			break;
		case MINUS:
			cout << "- ";
			a = d.top();
			d.pop();
			a = -a;
			d.push(a);
			break;
		case PLUS:
			cout << "+ ";
			a = d.top();
			d.pop();
			a = +a;
			d.push(a);
			break;
		case COMMA:
			break;
	}
}