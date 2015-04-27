#include <iostream>
#include <cstdio>
#include <stack>
#include <list>
#include <map>
#include <string>
#include <cmath>

using namespace std;

struct element
{
	bool isnumber;
	double number;
	int op;
};

void Infix2Postfix(string& expression, list<element>& Elem,map<int,string>&);

double readDouble(string&,int&);

int readFunc(string&,int&);

int readOp(string&,int&,bool);

bool hasHigherPrecendence(int&,int&);

void printOperatorStack(list<int>&,map<int,string>&);

void printPostfixList(list<element>&,map<int,string>&);

void processPostfix(list<element>&);

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
	string expression;
	list<element> Elem;
	map<int,string> opMap;
	opMap[5] = "(";
	opMap[15] = ")";
	opMap[4] = "sin";
	opMap[14] = "cos";
	opMap[24] = "exp";
	opMap[34] = "log";
	opMap[44] = "pow";
	opMap[54] = "sqrt";
	opMap[64] = "fabs";
	opMap[3] = "p";
	opMap[13] = "m";
	opMap[2] = "*";
	opMap[1] = "+";
	opMap[11] = "-";
	opMap[0] = ",";

	while(getline(cin, expression)){
		Infix2Postfix(expression, Elem, opMap);
		expression = "";
		processPostfix(Elem);
	}
	return 0;
}

void Infix2Postfix(string& expression, list<element>& Elem, map<int,string>& opMap){
	int i=0;
	int op;
	bool leftIsNumber = false;
	element container;
	list<int> S;

	cout << "*************** PROCESS START ***************" << endl;
	cout << "Infix expression: " << expression << endl << endl;
	
	while(i < expression.length()){
		
		// ignore white-space
		if (expression[i] == ' '){
			i++;
			continue;
		}

		else if (isdigit(expression[i]) || expression[i]=='.'){
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			container.isnumber = true;
			container.number = readDouble(expression,i);
			Elem.push_back(container);
			leftIsNumber = true;
			cout << "\tpush Double: " << container.number << endl;
		}

		else if (expression[i] == '('){
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			S.push_back(LEFT_PARENTHESIS);
			leftIsNumber = false;
			i++;
			cout << "\tpush left parenthesis into operator stack" << endl;
		}

		else if (expression[i] == ')'){
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			cout << "\treach right parenthesis" << endl;
			cout << "\tprocess operator in the operator stack......" << endl;
			while(!S.empty() && S.back()!=LEFT_PARENTHESIS){
				container.isnumber = false;
				container.op = S.back();
				Elem.push_back(container);
				S.pop_back();
				cout << "\tpop out [" << opMap[container.op] << "] from operator stack into postfix list" << endl;
			}
			S.pop_back();
			leftIsNumber = true;
			i++;
		}

		else if (isalpha(expression[i])){
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			op = readFunc(expression,i);
			while(!S.empty() && S.back()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.back(), op)){
				container.isnumber = false;
				container.op = S.back();
				Elem.push_back(container);
				S.pop_back();
				cout << "\tpop out [" << opMap[container.op] << "] from operator stack into postfix list" << endl;
			}
			S.push_back(op);
			leftIsNumber = false;
			cout << "\tpush [" << opMap[op] << "] into operator stack" << endl;
		}

		else{
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			op = readOp(expression,i,leftIsNumber);
			while(!S.empty() && S.back()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.back(), op)){
				container.isnumber = false;
				container.op = S.back();
				Elem.push_back(container);
				S.pop_back();
				cout << "\tpop out [" << opMap[container.op] << "] from operator stack into postfix list" << endl;
			}
			S.push_back(op);
			leftIsNumber = false;
			cout << "\tpush [" << opMap[op] << "] into operator stack" << endl;
		}
		cout << endl;
	}

	while(!S.empty()){
		printOperatorStack(S,opMap);
		printPostfixList(Elem,opMap);
		container.isnumber = false;
		container.op = S.back();
		Elem.push_back(container);
		S.pop_back();
		cout << "\tpop out [" << opMap[container.op] << "] from operator stack into postfix list" << endl;
	}
	cout << "Finally," << endl;
	printPostfixList(Elem,opMap);
	cout << "**************** PROCESS END ****************" << endl;
}

double readDouble(string& line,int& i){
	double d = 0;
	int decimal_level;
	while(isdigit(line[i])){
		d *= 10;
		d += (int)line[i] - 48;
		i++;
	}
	// if input format is a float number
	if(line[i] == '.'){
		i++;	// pass the dot
		decimal_level = -1;
		while(isdigit(line[i])){
			d += ((int)line[i] - 48) * pow(10,decimal_level);
			i++;
			decimal_level--;
		}
	}
	return d;
}

int readFunc(string& line,int& i){
	switch(line[i]){
		case 's':
			if(line[i+1] == 'i'){
				// read-in sin()
				i += 3;
				return SIN;
			}
			else{
				// read-in sqrt()
				i += 4;
				return SQRT;
			}
			break;
		case 'c':
			// read-in cos()
			i += 3;
			return COS;
			break;
		case 'e':
			// read-in exp()
			i += 3;
			return EXP;
			break;
		case 'l':
			// read-in log()
			i += 3;
			return LOG;
			break;
		case 'p':
			// read-in pow()
			i += 3;
			return POW;
			break;
		case 'f':
			// read-in fabs()
			i += 4;
			return FABS;
			break;
	}
	return -1;
}

int readOp(string& line, int& i,bool leftIsNumber){
	switch(line[i]){
		case '+':
			i++;
			return leftIsNumber ? ADD : PLUS;
			break;
		case '-':
			i++;
			return leftIsNumber ? SUBTRACT : MINUS;
			break;
		case '*':
			i++;
			return MULTIPLY;
			break;
		case ',':
			i++;
			return COMMA;
			break;
	}
	return -1;
}

bool hasHigherPrecendence(int& a, int& b){
	if((a%10)==3 && (b%10)==3){
		// both a and b are right associated
		return false;
	}
	else if((a%10) >= (b%10))
		return true;
	else
		return false;
}

void printOperatorStack(list<int>& S,map<int,string>& opMap){
	cout << "Operator Stack: ";
	for(list<int>::iterator it = S.begin(); it!=S.end(); ++it){
		cout << "[" << opMap[(*it)] << "] ";
	}
	cout << endl;
}

void printPostfixList(list<element>& Elem,map<int,string>& opMap){
	cout << "Postfix List: ";
	for(list<element>::iterator it = Elem.begin(); it!=Elem.end(); ++it){
		if((*it).isnumber)
			cout << "[" << (*it).number << "]" << " ";
		else
			cout << "[" << opMap[(*it).op] << "]" << " ";
	}
	cout << endl;
}

void processPostfix(list<element>& Q){
	stack<double> N;
	double a, b;
	printf("Postfix Exp: ");
	while(!Q.empty()){
		if(Q.front().isnumber){
			printf("%.6f ", Q.front().number);
			N.push(Q.front().number);
		}
		else{
			switch(Q.front().op){
				case ADD:
					printf("+ ");
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a + b;
					N.push(a);
					break;
				case SUBTRACT:
					printf("- ");
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a - b;
					N.push(a);
					break;
				case MULTIPLY:
					printf("* ");
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a * b;
					N.push(a);
					break;
				case PLUS:
					printf("+ ");
					a = N.top();
					N.pop();
					a = +a;
					N.push(a);
					break;
				case MINUS:
					printf("- ");
					a = N.top();
					N.pop();
					a = -a;
					N.push(a);
					break;
				case SIN:
					printf("sin ");
					a = N.top();
					N.pop();
					a = sin(a);
					N.push(a);
					break;
				case COS:
					printf("cos ");
					a = N.top();
					N.pop();
					a = cos(a);
					N.push(a);
					break;
				case EXP:
					printf("exp ");
					a = N.top();
					N.pop();
					a = exp(a);
					N.push(a);
					break;
				case LOG:
					printf("log ");
					a = N.top();
					N.pop();
					a = log(a);
					N.push(a);
					break;
				case FABS:
					printf("fabs ");
					a = N.top();
					N.pop();
					a = fabs(a);
					N.push(a);
					break;
				case SQRT:
					printf("sqrt ");
					a = N.top();
					N.pop();
					a = sqrt(a);
					N.push(a);
					break;
				case POW:
					printf("pow ");
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = pow(a,b);
					N.push(a);
					break;
				default:
					break;
			}
		}
		Q.pop_front();
	}
	printf("\n");
	printf("RESULT: %.6f\n", N.top());
	cout << endl;
}