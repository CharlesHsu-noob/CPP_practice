#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <x86intrin.h>
using namespace std;

template <typename T> struct complex {
	T real;
	T imag;
	//constructor
	complex(T rl = 0, T im = 0)
		: real(rl)
		, imag(im)
	{
	}

	//operator
	//addition
	complex<T> operator+=(const complex<T> &other)
	{
		this->real += other.real;
		this->imag += other.imag;
		return *this;
	}
	complex<T> operator+=(const T &other)
	{
		this->real += other;
		return *this;
	}
	friend complex<T> operator+(const complex<T> x, const complex<T> &y)
	{
		complex<T> r = x;
		r += y;
		return r;
	}
	friend complex<T> operator+(const complex<T> x, const T &y)
	{
		complex<T> r = x;
		r += y;
		return r;
	}
	friend complex<T> operator+(const T &x, const complex<T> &y)
	{
		complex<T> r = y;
		r += x;
		return r;
	}
	//substraction
	complex<T> operator-=(const complex<T> &other)
	{
		this->real -= other.real;
		this->imag -= other.imag;
		return *this;
	}
	complex<T> operator-=(const T &other)
	{
		this->real -= other;
		return *this;
	}
	friend complex<T> operator-(const complex<T> x, const complex<T> &y)
	{
		complex<T> r = x;
		r -= y;
		return r;
	}
	friend complex<T> operator-(const complex<T> x, const T &y)
	{
		complex<T> r = x;
		r -= y;
		return r;
	}
	friend complex<T> operator-(const T &x, const complex<T> &y)
	{
		complex<T> r = y;
		r -= x;
		return r;
	}
	//multiplication
	complex<T> operator*=(const complex<T> &other)
	{
		const T org_real = this->real;
		const T org_imag = this->imag;
		this->real = org_real * other.real - org_imag * other.imag;
		this->imag = org_real * other.imag + org_imag * other.real;
		return *this;
	}
	complex<T> operator*=(const T &other)
	{
		this->real *= other;
		this->imag *= other;
		return *this;
	}
	friend complex<T> operator*(const complex<T> x, const complex<T> &y)
	{
		complex<T> r = x;
		r *= y;
		return r;
	}
	friend complex<T> operator*(const complex<T> x, const T &y)
	{
		complex<T> r = x;
		r *= y;
		return r;
	}
	friend complex<T> operator*(const T &x, const complex<T> &y)
	{
		complex<T> r = y;
		r *= x;
		return r;
	}
	//division
	complex<T> operator/=(const complex<T> &other)
	{
		const T n = (other.real * other.real) + (other.imag * other.imag);
		const T r = (this->real * other.real) + (this->imag * other.imag);
		const T i = (this->imag * other.real) - (this->real * other.imag);
		this->real = r / n;
		this->imag = i / n;
		return *this;
	}
	complex<T> operator/=(const T &other)
	{
		this->real /= other;
		this->imag /= other;
		return *this;
	}
	friend complex<T> operator/(const complex<T> x, const complex<T> &y)
	{
		complex<T> r = x;
		r /= y;
		return r;
	}
	friend complex<T> operator/(const complex<T> x, const T &y)
	{
		complex<T> r = x;
		r /= y;
		return r;
	}
	friend complex<T> operator/(const T &x, const complex<T> &y)
	{
		complex<T> r = x;
		r /= y;
		return r;
	}
	//os stream
	//output <<
	template <typename _CharT, typename _Traits>
	friend basic_ostream<_CharT, _Traits> &operator<<(basic_ostream<_CharT, _Traits> &__os,
							  const complex<T> &__x)
	{
		basic_ostringstream<_CharT, _Traits> __s;
		__s.flags(__os.flags());
		__s.imbue(__os.getloc());
		__s.precision(__os.precision());
		__s << '(' << __x.real << ',' << __x.imag << ')';
		return __os << __s.str();
	}

	//input >>
	friend std::istream &operator>>(std::istream &is, complex<T> &z)
	{
		T u = 0, v = 0;
		char ch = 0;
		bool fail = true;

		if (is >> ch) {
			if (ch == '(') {
				//(a,b) 或 (a)
				if (is >> u >> ch) {
					if (ch == ')') {
						//(a)
						z = complex<T>(u, 0);
						fail = false;
					} else if (ch == ',') {
						//(a,b)
						if (is >> v >> ch) {
							if (ch == ')') {
								z = complex<T>(u, v);
								fail = false;
							} else {
								is.putback(ch); // 字元不是 ')'
							}
						}
					} else {
						is.putback(ch);
					}
				}
			} else {
				//a
				is.putback(ch); //把剛才讀到的第一個非 ( 字元吐回串流
				if (is >> u) {
					z = complex<T>(u, 0);
					fail = false;
				}
			}
		}

		if (fail) {
			is.setstate(std::ios_base::failbit);
		}

		return is;
	}
	//function
	complex<T> conj() const
	{
		return complex<T>(this->real, -1 * this->imag);
	}
	//reference /usr/include/c++/16/complex.hpp
	T abs() const
	{
		T x = this->real;
		T y = this->imag;
		const T scalar = std::max(std::abs(x), std::abs(y));
		if (scalar == T()) { //通用的初始化值
			return scalar;
		}
		x /= scalar;
		y /= scalar;
		return scalar * sqrt(x * x + y * y);
	}
};

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

int combination(int n, int k)
{
	if (k > n) {
		return 0;
	}
	if (k > (n - k)) {
		k = n - k;
	}
	int ans = 1;
	for (int i = 1; i < k + 1; i++) { //在第i個迴圈時分子有i個連序正整數,一定可以被i整除
		ans *= (n - i + 1);
		ans /= i;
	}
	return ans;
}

double pNormal(double x, int n)
{
	double ans = 0.0;
	int a = -1;
	double temp = 0;
	for (int i = 0; i < n + 1; i++) {
		a *= -1;
		temp = combination(n, i);
		temp *= pow(x, n - i);
		ans += a * temp;
	}
	return ans;
}

double pHonor(double x, int n)
{
	double ans = 1;
	double a = 1;
	for (int i = 1; i < n + 1; i++) {
		ans *= x;
		a *= -1;
		ans += a * combination(n, i);
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

complex<double> expComplex(const complex<double> &z, int n)
{
	complex<double> ans = { 1, 0 };
	complex<double> temp = { 1, 0 };
	for (int i = 1; i < n + 1; i++) {
		temp *= z / (i * 1.0);
		ans += temp;
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
	cout << "Result= " << ans.real << " + " << ans.imag << " i" << endl;
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

void expansionOfBinaryTheorom()
{
	cout << "The expansion of (x-1)^n where x=1.1 and n=1~20." << endl;
	cout << "Normal way:" << endl;
	for (int i = 1; i < 21; i++) {
		cout << setw(10) << pNormal(1.1, i) << endl;
	}
	cout << endl;
	cout << "Honor way:" << endl;
	for (int i = 1; i < 21; i++) {
		cout << setw(10) << pHonor(1.1, i) << endl;
	}
	cout << endl;

	cout << "The expansion of (x-1)^n where x=1.01 and n=1~20." << endl;
	cout << "Normal way:" << endl;
	for (int i = 1; i < 21; i++) {
		cout << setw(10) << pNormal(1.01, i) << endl;
	}
	cout << endl;
	cout << "Honor way:" << endl;
	for (int i = 1; i < 21; i++) {
		cout << setw(10) << pHonor(1.01, i) << endl;
	}
	cout << endl;
}

void powerOfExponatial()
{
	cout << "Power of exponatial e^z, z=1 + (pi/4)i, n=1~20." << endl;
	complex<double> z = { 1, M_PI / 4.0 };
	for (int i = 1; i < 21; i++) {
		cout << setw(10) << expComplex(z, i) << endl;
	}
	cout << endl;
}

void polynomialCalculater()
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
}

int main()
{
	polynomialCalculater();
	compareTwoWaysWithTaylorExpasion();
	expansionOfBinaryTheorom();
	powerOfExponatial();
	return 0;
}
