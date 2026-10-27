#include <cmath>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <x86intrin.h>

using namespace std;

template <typename T> void fillVector(vector<T> &V)
{
	// 檢查 cin 的緩衝區裡面有沒有殘留換行符號，有的話才清除
	if (cin.peek() == '\n') {
		cin.ignore();
	}
	//int length = V.size;
	// cout<<"Put all value in same line and divide them by <space>."<<endl;
	string line;

	getline(cin, line);
	stringstream ssline(line);
	T num;
	int i;
	V.clear();
	while (ssline >> num) {
		V.push_back(num);
	}
	line = "";
}

long long int factorial(int n)
{
	long long int ans = 1;
	for (int i = 2; i < n + 1; i++) {
		ans *= i;
	}
	return ans;
}

double sinTaylorExpansionWithNormal(double x, int n)
{
	double ans = 0.0;
	int a = 1;
	double temp = 0;
	for (int i = 0; i < n + 1; i++) {
		a = (i % 2 == 0) ? 1 : -1;
		temp = pow(x, 2 * i + 1) / factorial(2 * i + 1);
		ans += a * temp;
	}
	return ans;
}

double sinTaylorExpansionWithHonor(double x, int n)
{
	double ans = x;
	double temp = x;
	for (int i = 1; i < n + 1; i++) {
		temp = -1 * temp * x * x / ((2 * i) * (2 * i + 1));
		ans += temp;
	}
	return ans;
}

double cosTaylorExpansionWithNormal(double x, int n)
{
	double ans = 0.0;
	int a = 1;
	double temp = 0;
	for (int i = 0; i < n + 1; i++) {
		a = (i % 2 == 0) ? 1 : -1;
		temp = pow(x, 2 * i) / factorial(2 * i);
		ans += a * temp;
	}
	return ans;
}

double cosTaylorExpansionWithHonor(double x, int n)
{
	double ans = 1.0;
	double temp = 1.0;
	for (int i = 1; i < n + 1; i++) {
		temp = -1 * temp * x * x / ((2 * i - 1) * (2 * i));
		ans += temp;
	}
	return ans;
}

double nornalWayPolynomial(const vector<double> coff, const double x)
{
	double ans = 0;
	double temp_x = 1;
	int size = coff.size();
	for (int i = 0; i < size; i++) {
		temp_x = pow(x, i);
		ans += temp_x * coff[i];
	}
	return ans;
}

template <typename T> T honorWayPolynomial(const vector<T> coff, const T x)
{
	int size = coff.size();
	T ans = coff[size - 1];
	for (int i = size - 2; i > (-1); i--) {
		ans *= x;
		ans += coff[i];
	}
	return ans;
}

template <typename T> bool isTwoValueSame(const T a, const T b)
{
	return a == b;
}

void FindPolynomialValue()
{
	cout << "Give the coefficient of the polynomial." << endl;
	vector<double> coff;

	fillVector(coff);

	cout << "Give a x." << endl;
	double x;
	cin >> x;

	volatile double ans_a = nornalWayPolynomial(coff, x);
	cout << "Normal way:" << endl;
	cout << "Result= " << fixed << setprecision(16) << ans_a << endl;

	volatile double ans_b = honorWayPolynomial(coff, x);
	cout << "Honor way:" << endl;
	cout << "Result= " << fixed << setprecision(16) << ans_b << endl;

	cout << endl;

	if (isTwoValueSame(ans_a, ans_b)) {
		cout << "Same." << endl;
	} else {
		cout << "Not the same." << endl;
	}
}

void complexPolynomial()
{
	vector<complex<double> > coff;
	cout << "Give the coefficient:" << endl;
	fillVector(coff);
	cout << "Give a x:" << endl;
	complex<double> x;
	cin >> x;
	complex<double> ans = honorWayPolynomial(coff, x);
	cout << "Result= " << ans.real() << " + " << ans.imag() << " i" << endl;
}

void compareTwoWaysWithTaylorExpasion()
{
	cout << "Taylor expansion of sin(x) when x=pi/4 and n=1~8." << endl;
	double x = M_PI / 4;
	cout << "Normal way:" << endl;
	for (int i = 1; i < 9; i++) {
		cout << setw(10) << sinTaylorExpansionWithNormal(x, i) << endl;
	}
	cout << endl;
	cout << "Honor way:" << endl;
	for (int i = 1; i < 9; i++) {
		cout << setw(10) << sinTaylorExpansionWithHonor(x, i) << endl;
	}
	cout << endl;

	cout << "Taylor expansion of cos(x) when x=pi/4 and x=1~8." << endl;
	cout << "Normal way:" << endl;
	for (int i = 1; i < 9; i++) {
		cout << setw(10) << cosTaylorExpansionWithNormal(x, i) << endl;
	}
	cout << endl;
	cout << "Honor way:" << endl;
	for (int i = 1; i < 9; i++) {
		cout << setw(10) << cosTaylorExpansionWithHonor(x, i) << endl;
	}
	cout << endl;
}

int main()
{
	cout << "Choose the domain of x." << endl;
	cout << "1. Real number\n2. Complex number" << endl;
	int mode = 0;
	cin >> mode;
	switch (mode) {
	case 1: {
		FindPolynomialValue();
		break;
	}
	case 2: {
		complexPolynomial();
		break;
	}
	default: {
		cout << "Invalid mode!" << endl;
		break;
	}
	}
	compareTwoWaysWithTaylorExpasion();
	return 0;
}
