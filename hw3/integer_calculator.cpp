#include <iostream>
#include <stack>
#include <queue>
#include <string>

using namespace std;

struct element
{
	bool isnumber;
	int content;
};

void Infix2Postfix(string& expression, queue<element>& Elem);

int readInt(string&,int&);

int readOp(string&,int&,bool);

bool hasHigherPrecendence(int&,int&);

void processPostfix(queue<element>&);

enum {
	// ( and ) are the highest level
	LEFT_PARENTHESIS = 9,
	RIGHT_PARENTHESIS = 19,
	// !, ~, (unary)+ and (unary)- are the second
	LOGICAL_NOT = 8,
	BINARY_NOT = 18,
	UNARY_PLUS = 28,
	UNARY_MINUS = 38,
	// *, / and % are third
	MULTIPLY = 7,
	DIVIDE = 17,
	REMAIND = 27,
	// (binary)+ and (binary)- are forth
	ADD = 6,
	SUBTRACT = 16,
	// >> and << are fifth
	LEFT_SHIFT = 5,
	RIGHT_SHIFT = 15,
	// the end, &, ^, |, && and ||
	BINARY_AND = 4,
	BINARY_XOR = 3,
	BINARY_OR = 2,
	LOGICAL_AND = 1,
	LOGICAL_OR = 0
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

		else if (isdigit(expression[i])){
			container.isnumber = true;
			container.content = readInt(expression,i);
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
				container.content = S.top();
				Elem.push(container);
				S.pop();
			}
			S.pop();
			leftIsNumber = true;
			i++;
		}

		else{
			op = readOp(expression,i,leftIsNumber);
			while(!S.empty() && S.top()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.top(), op)){
				container.isnumber = false;
				container.content = S.top();
				Elem.push(container);
				S.pop();
			}
			S.push(op);
			leftIsNumber = false;
		}
	}

	while(!S.empty()){
		container.isnumber = false;
		container.content = S.top();
		Elem.push(container);
		S.pop();
	}
}

int readInt(string& line, int& iter){
	int res = 0;
	while(isdigit(line[iter])){
		res *= 10;
		res += (int)line[iter] - 48;
		iter++;
	}
	return res;
}

int readOp(string& line, int& i,bool leftIsNumber){
	switch(line[i]){
		case '+':
			i++;
			return leftIsNumber ? ADD : UNARY_PLUS;
			break;
		case '-':
			i++;
			return leftIsNumber ? SUBTRACT : UNARY_MINUS;
			break;
		case '*':
			i++;
			return MULTIPLY;
			break;
		case '/':
			i++;
			return DIVIDE;
			break;
		case '%':
			i++;
			return REMAIND;
			break;
		case '&':
			if(line[i+1]=='&'){
				i+=2;
				return LOGICAL_AND;
			}
			else{
				i++;
				return BINARY_AND;
			}
			break;
		case '|':
			if(line[i+1]=='|'){
				i+=2;
				return LOGICAL_OR;
			}
			else{
				i++;
				return BINARY_OR;
			}
			break;
		case '>':
			i+=2;
			return RIGHT_SHIFT;
			break;
		case '<':
			i+=2;
			return LEFT_SHIFT;
			break;
		case '!':
			i++;
			return LOGICAL_NOT;
			break;
		case '~':
			i++;
			return BINARY_NOT;
			break;
		case '^':
			i++;
			return BINARY_XOR;
			break;
	}
	return -1;
}

bool hasHigherPrecendence(int& a, int& b){
	if((a%10)==8 && (b%10)==8){
		// both a and b are right associated
		return false;
	}
	else if((a%10) >= (b%10))
		return true;
	else
		return false;
}

void processPostfix(queue<element>& Q){
	stack<int> N;
	int a, b;
	cout << "Postfix Exp: ";
	while(!Q.empty()){
		if(Q.front().isnumber){
			cout << Q.front().content << " ";
			N.push(Q.front().content);
		}
		else{
			switch(Q.front().content){
				case ADD:
					cout << "+ ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a + b;
					N.push(a);
					break;
				case SUBTRACT:
					cout << "- ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a - b;
					N.push(a);
					break;
				case MULTIPLY:
					cout << "* ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a * b;
					N.push(a);
					break;
				case DIVIDE:
					cout << "/ ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a / b;
					N.push(a);
					break;
				case REMAIND:
					cout << "% ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a % b;
					N.push(a);
					break;
				case LOGICAL_AND:
					cout << "&& ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a && b;
					N.push(a);
					break;
				case LOGICAL_OR:
					cout << "|| ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a || b;
					N.push(a);
					break;
				case LOGICAL_NOT:
					cout << "! ";
					a = N.top();
					N.pop();
					a = !a;
					N.push(a);
					break;
				case BINARY_XOR:
					cout << "^ ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a ^ b;
					N.push(a);
					break;
				case BINARY_NOT:
					cout << "~ ";
					a = N.top();
					N.pop();
					a = ~a;
					N.push(a);
					break;
				case BINARY_OR:
					cout << "| ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a | b;
					N.push(a);
					break;
				case BINARY_AND:
					cout << "& ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a & b;
					N.push(a);
					break;
				case RIGHT_SHIFT:
					cout << ">> ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a >> b;
					N.push(a);
					break;
				case LEFT_SHIFT:
					cout << "<< ";
					b = N.top();
					N.pop();
					a = N.top();
					N.pop();
					a = a << b;
					N.push(a);
					break;
				case UNARY_PLUS:
					cout << "+ ";
					a = N.top();
					N.pop();
					a = +a;
					N.push(a);
					break;
				case UNARY_MINUS:
					cout << "- ";
					a = N.top();
					N.pop();
					a = -a;
					N.push(a);
					break;
			}
		}
		Q.pop();
	}
	cout << endl;
	cout << "RESULT: " << N.top() << endl;
}