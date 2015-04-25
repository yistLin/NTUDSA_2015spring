#include <iostream>
#include <string>
#include <stack>
#include <map>

#define MAXLENGTH 1000000

using namespace std;

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

int readInt(const char* line, int& iter){
	int res = 0;
	while(isdigit(line[iter])){
		res *= 10;
		res += (int)line[iter] - 48;
		iter++;
	}
	return res;
}

int readOp(map<char,int>& m, const char* line, int& iter, bool& behindNumber){
	if(!behindNumber){
		if(m[line[iter]] == ADD){
			iter++;
			return UNARY_PLUS;
		}
		else if(m[line[iter]] == SUBTRACT){
			iter++;
			return UNARY_MINUS;
		}
	}
	switch(m[line[iter]]){
		case LEFT_SHIFT:
			iter += 2;
			return LEFT_SHIFT;
			break;
		case RIGHT_SHIFT:
			iter += 2;
			return RIGHT_SHIFT;
			break;
		case BINARY_AND:
			if (m[line[iter+1]] == BINARY_AND){
				iter += 2;
				return LOGICAL_AND;
			}
			else{
				iter++;
				return BINARY_AND;
			}
			break;
		case BINARY_OR:
			if (m[line[iter+1]] == BINARY_OR){
				iter += 2;
				return LOGICAL_OR;
			}
			else{
				iter++;
				return BINARY_OR;
			}
			break;
		default:
			iter++;
			return m[line[iter-1]];
			break;
	}
}

void processOp(stack<int>& n, stack<int>& o){
	int a, b;
	int op = o.top();
	o.pop();
	switch(op){
		case LOGICAL_NOT:
			cout << "! ";
			a = n.top();
			n.pop();
			a = !a;
			n.push(a);
			break;
		case BINARY_NOT:
			cout << "~ ";
			a = n.top();
			n.pop();
			a = ~a;
			n.push(a);
			break;
		case MULTIPLY:
			cout << "* ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a * b;
			n.push(a);
			break;
		case DIVIDE:
			cout << "/ ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a / b;
			n.push(a);
			break;
		case REMAIND:
			cout << "% ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a % b;
			n.push(a);
			break;
		case ADD:
			cout << "+ ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a + b;
			n.push(a);
			break;
		case SUBTRACT:
			cout << "- ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a - b;
			n.push(a);
			break;
		case LEFT_SHIFT:
			cout << "<< ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a << b;
			n.push(a);
			break;
		case RIGHT_SHIFT:
			cout << ">> ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a >> b;
			n.push(a);
			break;
		case BINARY_AND:
			cout << "& ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a & b;
			n.push(a);
			break;
		case BINARY_XOR:
			cout << "^ ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a ^ b;
			n.push(a);
			break;
		case BINARY_OR:
			cout << "| ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a | b;
			n.push(a);
			break;
		case LOGICAL_AND:
			cout << "&& ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a && b;
			n.push(a);
			break;
		case LOGICAL_OR:
			cout << "|| ";
			b = n.top();
			n.pop();
			a = n.top();
			n.pop();
			a = a || b;
			n.push(a);
			break;
		case UNARY_PLUS:
			cout << "+ ";
			a = n.top();
			n.pop();
			a = +a;
			n.push(a);
			break;
		case UNARY_MINUS:
			cout << "- ";
			a = n.top();
			n.pop();
			a = -a;
			n.push(a);
			break;
	}
}

void processInputOp(stack<int>& n, stack<int>& o, int& op_buf){
	if(o.empty()){
		o.push(op_buf);
	}
	else if(op_buf == RIGHT_PARENTHESIS){
		while(o.top() != LEFT_PARENTHESIS){
			processOp(n, o);
		}
		// pop out LEFT_PARENTHESIS
		o.pop();
	}
	else if(((o.top() % 10) >= (op_buf % 10)) && (o.top() != LEFT_PARENTHESIS)){
		while(!o.empty() && ((o.top() % 10) >= (op_buf % 10)) && (o.top() != LEFT_PARENTHESIS)){
			processOp(n, o);
		}
		// after process input op, push op_buf into stack_o
		o.push(op_buf);
	}
	else{
		// if precedence greater than previous one, just push
		o.push(op_buf);
	}
}

int main(int argc, char const *argv[])
{
	stack<int> stack_n;
	stack<int> stack_o;
	map<char,int> op_map;
	op_map.insert(pair<char,int>('(',LEFT_PARENTHESIS));
	op_map.insert(pair<char,int>(')',RIGHT_PARENTHESIS));
	op_map.insert(pair<char,int>('!',LOGICAL_NOT));
	op_map.insert(pair<char,int>('~',BINARY_NOT));
	op_map.insert(pair<char,int>('+',ADD));
	op_map.insert(pair<char,int>('-',SUBTRACT));
	op_map.insert(pair<char,int>('*',MULTIPLY));
	op_map.insert(pair<char,int>('/',DIVIDE));
	op_map.insert(pair<char,int>('%',REMAIND));
	op_map.insert(pair<char,int>('<',LEFT_SHIFT));
	op_map.insert(pair<char,int>('>',RIGHT_SHIFT));
	op_map.insert(pair<char,int>('&',BINARY_AND));
	op_map.insert(pair<char,int>('^',BINARY_XOR));
	op_map.insert(pair<char,int>('|',BINARY_OR));

	char entireLine[MAXLENGTH];
	bool behindNumber;
	int int_buf, op_buf;
	int it;
	
	// process all the input from stdin
	while(cin.getline(entireLine, MAXLENGTH)){
		it = 0;
		behindNumber = false;

		// output the line requested by 3.4 spec
		cout << "Postfix Exp: ";
		
		// process input from line
		while(entireLine[it] != '\0'){
			if(isdigit(entireLine[it])){
				int_buf = readInt(entireLine, it);
				stack_n.push(int_buf);
				cout << int_buf << " ";
				// if read-in is number, just output
				behindNumber = true;
			}
			else{
				if(entireLine[it] != ' '){
					op_buf = readOp(op_map,entireLine,it,behindNumber);
					// check the precedence of the operator
					processInputOp(stack_n, stack_o, op_buf);
					//cout << "op_buf = " << op_buf << endl;
					if(op_buf == RIGHT_PARENTHESIS || op_buf)
						behindNumber = true;
					else
						behindNumber = false;
				}
				else
					it++;
			}
		}
		while(!stack_o.empty()){
			processOp(stack_n, stack_o);
		}
		// translate to postfis ended
		cout << endl;

		// the last integer in stack_n is the answer
		cout << "RESULT: " << stack_n.top() << endl;
		stack_n.pop();
	}

	return 0;
}