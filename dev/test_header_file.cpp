#include <iostream>
#include <stdexcept>
#include "my_vector.hpp"

using namespace std;
using namespace xdd;

// 輔助函式：將靜態陣列轉為 xdd::matrix
xdd::matrix<double> makeMatrix(int row, int col, const double *raw)
{
	xdd::matrix<double> M(row, xdd::vector<double>(col, 0));
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			M[i][j] = raw[i * col + j];
		}
	}
	return M;
}

// 輔助函式：將靜態陣列轉為 xdd::vector
xdd::vector<double> makeVector(int len, const double *raw)
{
	xdd::vector<double> V(len, 0);
	for (int i = 0; i < len; i++) {
		V[i] = raw[i];
	}
	return V;
}

int main()
{
	cout << "==== Test 1: 2x3 Matrix * 3x1 Vector (Expected: 2x1 Vector) ====" << endl;
	double rawM1[] = { 1, 2, 3, 4, 5, 6 };
	double rawV1[] = { 10, 20, 30 };

	xdd::matrix<double> M1 = makeMatrix(2, 3, rawM1);
	xdd::vector<double> V1 = makeVector(3, rawV1);

	cout << "M1 (2x3):" << endl;
	xdd::printMatrix(M1);
	cout << "V1 (3x1):" << endl;
	xdd::printVector(V1);

	xdd::vector<double> res1 = matrixMulti(M1, V1);
	cout << "Result M1 * V1 (Expected: [140, 320]):" << endl;
	xdd::printVector(res1);
	cout << endl;

	cout << "==== Test 2: Verify Null Space Basis (A * x_n = 0) ====" << endl;
	double rawA[] = { 2, 4, 2, 0, 4, 4, 0, 8, 8 };
	// 先前算出的 R = [1, 0, -1; 0, 1, 1]，對應的 nullspace 基底為 [1, -1, 1]
	double rawXn[] = { 1, -1, 1 };

	xdd::matrix<double> A = makeMatrix(3, 3, rawA);
	xdd::vector<double> x_n = makeVector(3, rawXn);

	xdd::vector<double> res2 = matrixMulti(A, x_n);
	cout << "Result A * x_n (Expected: [0, 0, 0]):" << endl;
	xdd::printVector(res2);
	cout << endl;

	cout << "==== Test 3: Dimension Mismatch Exception (2x3 * 2x1) ====" << endl;
	double rawV_bad[] = { 1, 2 };
	xdd::vector<double> V_bad = makeVector(2, rawV_bad);

	try {
		xdd::vector<double> res_err = matrixMulti(M1, V_bad);
	} catch (const std::invalid_argument &e) {
		cout << "Successfully caught exception: " << e.what() << endl;
	}

	return 0;
}
