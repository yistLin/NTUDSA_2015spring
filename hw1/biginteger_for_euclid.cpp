#include <iostream>
#include "biginteger_for_euclid.h"

BigInteger::BigInteger(){
	top = 0;
	for(int i=0; i<512; i++)
		digit[i] = 0;
}

BigInteger::BigInteger(const int& n){
	top = 0;
	int m = n;
	while(m > 0){
		digit[top] = m % 10;
		m /= 10;
		top++;
	}
	top -= 1;
	for(int i=top+1; i<512; i++)
		digit[i] = 0;
}

BigInteger::BigInteger(const std::string& s){
	int len = s.length();
	for(top=0; top<len; top++){
		digit[top] = int(s[len-1-top]) - 48;
	}
	top -= 1;
	for(int i=top+1; i<512; i++)
		digit[i] = 0;
}

BigInteger::BigInteger(const BigInteger& b){
	top = b.top;
	for(int i=0; i<512; i++){
		digit[i] = b.digit[i];
	}
}

BigInteger::~BigInteger(){}


bool BigInteger::operator<(const BigInteger& b) const{
	if(top < b.top)
		return true;
	else if(top == b.top){
		for(int i=b.top; i>=0; i--){
			if(digit[i] < b.digit[i])
				return true;
			else if(digit[i] > b.digit[i])
				return false;
		}
		return false;
	}
	else
		return false;
}

bool BigInteger::operator==(const BigInteger& b) const{
	int flag = 1;
	if(top == b.top){
		for(int i=top; i>=0; i--){
			if(digit[i] != b.digit[i]){
				flag = 0;
				break;
			}
		}
		return (flag==1)?true:false;
	}
	else
		return false;
}

BigInteger BigInteger::operator%(const BigInteger& b) const{
	BigInteger tmp, c(*this), t(b);
	for(int i=(c.top - t.top); i>=0; i--){
		tmp = t.shift(i);
		while(tmp < c || tmp == c){
			c = c - tmp;
		}
	}
	for(int i=511; i>=0; i--){
		if(c.digit[i] != 0){
			c.top = i;
			break;
		}
	}
	return c;
}

const BigInteger BigInteger::shift(const int& n){
	BigInteger tmp;
	for(int i=0; i<=top; i++){
		tmp.digit[i+n] = digit[i];
	}
	tmp.top = top + n;
	return tmp;
}

const BigInteger BigInteger::operator-(const BigInteger& b) const{
	BigInteger n(*this);
	for(int i=0; i<=n.top; i++){
		if(n.digit[i] < b.digit[i]){
			n.digit[i] += 10;
			n.digit[i+1]--;
			n.digit[i] -= b.digit[i];
		}
		else{
			n.digit[i] -= b.digit[i];
		}
	}
	for(int i=n.top; i>0; i--){
		if(n.digit[i] == 0)
			n.top--;
		else
			break;
	}
	return n;
}

BigInteger& BigInteger::operator=(const BigInteger& b){
	this->top = b.top;	
	for(int i=0; i<512; i++){
		this->digit[i] = b.digit[i];
	}
	return *this;
}

bool BigInteger::iszero(){
	return (top==0 && digit[0]==0)?true:false;
}

std::ostream& operator<<(std::ostream& out, const BigInteger& b){
	for(int i=b.top; i>=0; i--)
		out << b.digit[i];
	return out;
}
