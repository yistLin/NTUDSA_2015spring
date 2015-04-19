#include <iostream>
using namespace std;

void output(const int a, const int b, string s, const int out, const int iter){
	cout << "Case (" << a << ", " << b << "): " << s << " = " << out << ", taking " << iter << " iterations" << endl;
	return;
}

void swap(int &a, int &b){
	int tmp = a;
	a = b;
	b = tmp;
}

int min(const int a, const int b){return (a < b)?a:b;}

int max(const int a, const int b){return (a > b)?a:b;}

void gcd_by_reverse_search(const int a, const int b){
	int iter = 0;
	for (int i=min(a,b); i > 0; i--) {
		iter++;
		if((a % i == 0) && (b % i == 0)){
			output(a, b, "GCD-By-Reverse-Search", i, iter);
			return;
		}
	}
}

int gcd_by_filter_internal(const int a, const int b, int &iter){
	for (int i=2; i <= min(a,b); i++){
		iter++;
		if((a % i == 0) && (b % i == 0)){
			return i * gcd_by_filter_internal(a/i, b/i, iter);
		}
	}
	return 1;
}

void gcd_by_filter(const int a, const int b){
	int iter=0;
	int out = gcd_by_filter_internal(a, b, iter);
	output(a, b, "GCD-By-Filter", out, iter);
	return;
}

int gcd_by_filter_faster_internal(const int a, const int b, const int s, int &iter){
	for (int i=s; i <= min(a,b); i++){
		iter++;
		if((a % i == 0) && (b % i == 0)){
			return i * gcd_by_filter_faster_internal(a/i, b/i, i, iter);
		}
	}
	return 1;
}

void gcd_by_filter_faster(const int a, const int b){
	int iter=0;
	int out = gcd_by_filter_faster_internal(a, b, 2, iter);
	output(a, b, "GCD-By-Filter-Faster", out, iter);
	return;
}

void gcd_by_binary(const int a, const int b){
	int n = min(a,b), m = max(a,b), ans=1, iter=0;
	while (n!=0 && m!=0){
		if(n%2==0 && m%2==0){
			ans *= 2;
			n /= 2;
			m /= 2;
		}
		else if(n%2==0 && m%2!=0){
			n /= 2;
		}
		else if(n%2!=0 && m%2==0){
			m /= 2;
		}
		if(n > m) swap(n,m);
		m -= n;
		iter++;
	}
	output(a, b, "GCD-By-Binary", n*ans, iter);
	return;
}

void gcd_by_euclid(const int a, const int b){
	int n=min(a,b), m=max(a,b), tmp=0, iter=0;
	while(m % n != 0){
		tmp = n;
		n = m % n;
		m = tmp;
		iter++;
	}
	output(a, b, "GCD-By-Euclid", n, iter);
	return;
}

int main()
{
	int a, b;
	cin >> a;
	while (a != 0){
		cin >> b;
		gcd_by_reverse_search(a, b);
        gcd_by_filter(a, b);
		gcd_by_filter_faster(a, b);
        gcd_by_binary(a, b);
        gcd_by_euclid(a, b);
		cin >> a;
	}
	return 0;
}
