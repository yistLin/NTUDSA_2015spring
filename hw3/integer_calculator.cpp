#include <iostream>
#include <stack>
#include <list>
#include <map>
#include <string>

using namespace std;

struct element
{
	bool isnumber;
	int content;
};

void Infix2Postfix(string& expression, list<element>& Elem, map<int,string>& opMap);

int readInt(string&,int&);

int readOp(string&,int&,bool);

bool hasHigherPrecendence(int&,int&);

void printOperatorStack(list<int>&,map<int,string>&);

void printPostfixList(list<element>&,map<int,string>&);

void processPostfix(list<element>&);

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
	list<element> Elem;
	map<int,string> opMap;
	opMap[0] = "||";
	opMap[1] = "&&";
	opMap[2] = "|";
	opMap[3] = "^";
	opMap[4] = "&";
	opMap[5] = "<<";
	opMap[15] = ">>";
	opMap[6] = "+";
	opMap[16] = "-";
	opMap[7] = "*";
	opMap[17] = "/";
	opMap[27] = "%";
	opMap[8] = "!";
	opMap[18] = "~";
	opMap[28] = "p";
	opMap[38] = "m";
	opMap[9] = "(";

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

		else if (isdigit(expression[i])){
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			container.isnumber = true;
			container.content = readInt(expression,i);
			Elem.push_back(container);
			leftIsNumber = true;
			cout << "\tpush Integer: " << container.content << endl;
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
				container.content = S.back();
				Elem.push_back(container);
				S.pop_back();
				cout << "\tpop out [" << opMap[container.content] << "] from operator stack into postfix list" << endl;
			}
			S.pop_back();
			leftIsNumber = true;
			i++;
		}

		else{
			printOperatorStack(S,opMap);
			printPostfixList(Elem,opMap);
			op = readOp(expression,i,leftIsNumber);
			while(!S.empty() && S.back()!=LEFT_PARENTHESIS && hasHigherPrecendence(S.back(), op)){
				container.isnumber = false;
				container.content = S.back();
				Elem.push_back(container);
				S.pop_back();
				cout << "\tpop out [" << opMap[container.content] << "] from operator stack into postfix list" << endl;
			}
			S.push_back(op);
			leftIsNumber = false;
			cout << "\tpush [" << opMap[op] << "] into operator stack" << endl;
		}
		// seperate with the next step
		cout << endl;
	}

	while(!S.empty()){
		printOperatorStack(S,opMap);
		printPostfixList(Elem,opMap);
		container.isnumber = false;
		container.content = S.back();
		Elem.push_back(container);
		S.pop_back();
		cout << "\tpop out [" << opMap[container.content] << "] from operator stack into postfix list" << endl;
		cout << endl;
	}
	cout << "Finally," << endl;
	printPostfixList(Elem,opMap);
	cout << "**************** PROCESS END ****************" << endl;
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
			cout << "[" << (*it).content << "]" << " ";
		else
			cout << "[" << opMap[(*it).content] << "]" << " ";
	}
	cout << endl;
}

void processPostfix(list<element>& Q){
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
		Q.pop_front();
	}
	cout << endl;
	cout << "RESULT: " << N.top() << endl << endl;
}