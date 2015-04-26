#include <iostream>
#include <cstdio>
#include <stack>
#include <queue>
#include <string>
#include <cmath>

using namespace std;

struct element
{
	bool isnumber;
	double number;
	int op;
};

void Infix2Postfix(string& expression, queue<element>& Elem);

double readDouble(string&,int&);

int readFunc(string&,int&);

int readOp(string&,int&,bool);

bool hasHigherPrecendence(int&,int&);

void processPostfix(queue<element>&);

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
	queue<element> Elem;

	while(getline(cin, expression)){
		Infix2Postfix(expression, Elem);
		expression = "";
		processPostfix(Elem);
	}
	return 0;
}

void Infix2Postfix(string& expression, queue<element>& Elem){
	int i=0;
	int op;
	bool leftIsNumber = false;
	element container;
	stack<int> S;
	
	while(i < expression.length()){
		
		// ignore white-space
		if (expression[i] == ' '){
			i++;
			continue;
		}

		else if (isdigit(expression[i]) || expression[i]=='.'){
			container.isnumber = true;
			container.number = readDouble(expression,i);
			Elem.push(container);
			leftIsNumber = true;
		}

		else if (expression[i] == '('){
			S.push(LEFT_PARENTHESIS);
			leftIsNumber = false;
			i++;
		}

		else if (expression[i] == ')'){
			while(!S.empty() && S.top()!=LEFT_PARENTHESIS){
				container.isnumber = false;
				container.op = S.top();
				Elem.push(container);
				S.pop();
			}
			S.pop();
			leftIsNumber = true;
			i++;
		}

		else if (isalpha(expression[i])){
			op = readFunc(expression,i);
			while(!S.empty() && S.top()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.top(), op)){
				container.isnumber = false;
				container.op = S.top();
				Elem.push(container);
				S.pop();
			}
			S.push(op);
			leftIsNumber = false;
		}

		else{
			op = readOp(expression,i,leftIsNumber);
			while(!S.empty() && S.top()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.top(), op)){
				container.isnumber = false;
				container.op = S.top();
				Elem.push(container);
				S.pop();
			}
			S.push(op);
			leftIsNumber = false;
		}
	}

	while(!S.empty()){
		container.isnumber = false;
		container.op = S.top();
		Elem.push(container);
		S.pop();
	}
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

void processPostfix(queue<element>& Q){
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
		Q.pop();
	}
	printf("\n");
	printf("RESULT: %.6f\n", N.top());
}