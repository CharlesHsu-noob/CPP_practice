#include <iostream>
#include <cmath>
#include <complex>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>
#include <x86intrin.h>

using namespace std;

template <typename T> void pushBack(T value, T *&array, int &capacity, int &index)
{
	if (index == capacity) {
		if (capacity == 0) {
			capacity = 1;
		} else {
			capacity *= 2;
		}
		T *newArray = new T[capacity];
		for (int i = 0; i < index; i++) {
			newArray[i] = array[i];
		}
		delete[] array;
		array = newArray;
	}
	array[index] = value;
	index++;
}

template <typename T> void fillArray(T *&array, int &capacity)
{
	if (cin.peek() == '\n') {
		cin.ignore();
	}

	string line;
	getline(cin, line);
	stringstream ssline(line);
	double num;
	int index = 0;
	for (int i = 0; i < capacity; i++) {
		array[i] = NAN;
	}
	while (ssline >> num) {
		pushBack(num, array, capacity, index);
	}
	line = "";
}

double nornalWayPolynomial(const double coff[], const int capacity, const double x)
{
	double ans = 0;
	double temp_x = 1;
	for (int i = 0; i < capacity; i++) {
		//calculate x^n
		for (int j = 0; j < i; j++) {
			temp_x *= x;
		}
		ans += temp_x * coff[i];
		temp_x = 1;
	}
	return ans;
}

template <typename T> T honorWayPolynomial(const T coff[], const int capacity, const T x)
{
	T ans = coff[capacity - 1];
	for (int i = capacity - 2; i > (-1); i--) {
		ans *= x;
		ans += coff[i];
	}
	return ans;
}

template <typename T> T horner(const std::vector<T> &a, T x)
{
	T result = a.back();
	for (int k = (int)a.size() - 2; k >= 0; --k)
		result = result * x + a[k];
	return result;
}

template <typename T> bool isTwoValueSame(const T a, const T b)
{
	return a == b;
}

void FindPolynomialValue()
{
	cout << "Give the coefficient of the polynomial." << endl;
	int capacity = 2;
	double *coff = new double[capacity];

	fillArray(coff, capacity);

	cout << "Give a x." << endl;
	double x;
	cin >> x;

	volatile double ans_a = nornalWayPolynomial(coff, capacity, x);
	cout << "Normal way:" << endl;
	cout << "Result= " << fixed << setprecision(16) << ans_a << endl;

	volatile double ans_b = honorWayPolynomial(coff, capacity, x);
	cout << "Honor way:" << endl;
	cout << "Result= " << fixed << setprecision(16) << ans_b << endl;

	cout << endl;

	if (isTwoValueSame(ans_a, ans_b)) {
		cout << "Same." << endl;
	} else {
		cout << "Not the same." << endl;
	}

	delete[] coff;
	coff = nullptr;
}

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

void complexPolynomial()
{
	vector<complex<double> > coff;
	cout << "Give the coefficient:" << endl;
	fillVector(coff);
	cout << "Give a x:" << endl;
	complex<double> x;
	cin >> x;
	complex<double> ans = horner(coff, x);
	cout << "Result= " << ans.real() << " + " << ans.imag() << " i" << endl;
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
	return 0;
}
