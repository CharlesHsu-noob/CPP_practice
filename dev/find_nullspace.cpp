#include <iostream>
#include <tuple>
#include "my_vector.hpp"

using namespace std;

//include from CR_decompose.cpp
pair<xdd::matrix<double>, xdd::matrix<double> >
CR_decompose(xdd::matrix<double> &A);
//end include

//include from solLinearSys.cpp
xdd::vector<double> solLinearSys(xdd::matrix<double> &A,
				 xdd::vector<double> &b);
//end include

xdd::vector<int> getPivotCol(const xdd::matrix<double> &M)
{
	//require M is in R_0 type
	int row = M.size;
	int col = M[0].size;
	xdd::vector<int> pivotCol;

	for (int i = 0; i < row; i++) {
		for (int j = i; j < col; j++) {
			if (std::fabs(M[i][j]) > 1e-9) {
				pivotCol.push_back(j);
				break;
			}
		}
	}
	return pivotCol;
}

xdd::vector<int> getFreeCol(const xdd::matrix<double> &M)
{
	xdd::vector<int> freeCol;
	for (int i = 0; i < M[0].size; i++) {
		freeCol[i] = i;
	}
	xdd::vector<int> pivotCol = getPivotCol(M);
	xdd::removeByValue(freeCol, pivotCol);
	return freeCol;
}

//return permutation of col change in vector type
//P[i]=j means that in M which M be column changed,
//original M's j-th column be change to i-th col in M_colChange.
xdd::vector<int> colChange(xdd::matrix<double> &M, xdd::vector<int> pivotCol)
{
	//change free columns to the right

	xdd::vector<int> P(M[0].size);
	for (int i = 0; i < M[0].size; i++) {
		P[i] = i;
	}

	for (int i = 0; i < pivotCol.size; i++) {
		if (pivotCol[i] != i) {
			swap(M[pivotCol[i]], M[i]);
			swap(P[pivotCol[i]], P[i]);
		}
	}
	return P;
}
xdd::vector<xdd::vector<double> > nullspace(xdd::matrix<double> &A)
{
	xdd::matrix<double> C, R;
	std::tie(C, R) = CR_decompose(A);
	xdd::vector<int> pivotCol = getPivotCol(R);
	xdd::vector<int> P = colChange(R, pivotCol);
	//x_n.size=freeCol.size=R[0].size-pivotCol.size
	//here x_n is wrong sort, need resort later
	xdd::matrix<double> x_n(R[0].size - pivotCol.size,
				xdd::vector<double>(R[0].size, 0));
	xdd::vector<double> b(pivotCol.size + 1, 0);
	b[pivotCol.size] = 1;
	for (int i = 0; i < x_n.size; i++) {
		//可以優化(using M.block or M.view)
		xdd::matrix<double> temp_A(b.size,
					   xdd::vector<double>(b.size, 0));
		for (int j = 0; j < temp_A.size - 1; j++) {
			for (int k = j; k < temp_A.size - 1; k++) {
				temp_A[j][k] = R[j][k];
			}
			//b.size-1=pivotCol.size
			temp_A[j][pivotCol.size] = R[j][pivotCol.size];
		}

		//b.size-1=pivotCol.size
		temp_A[pivotCol.size][pivotCol.size] = 1;
		xdd::vector<double> temp_x = solLinearSys(temp_A, b);

		for (int j = 0; j < pivotCol.size; j++) {
			x_n[i][j] = temp_x[j];
		}
		x_n[i][pivotCol.size + i] = 1;
	}

	//resort x_n
	for (int i = 0; i < x_n[0].size; i++) {
		//總共有i個col要檢查 i代表column index
		if (i == P[i]) {
			continue;
		}
		//colum change
		for (int j = 0; j < x_n.size; j++) {
			swap(x_n[j][i], x_n[j][P[i]]);
		}
	}

	return x_n;
}

int main()
{
	cout << "----Find nullspace----" << endl;
	xdd::matrix<double> A = xdd::readMatrixFromIstream<double>();
	/*
	xdd::matrix<double> C, R;
	std::tie(C, R) = CR_decompose(A);
	xdd::vector<int> pivotCol = getPivotCol(R);

	xdd::printVector(pivotCol);
	return 0;*/

	xdd::matrix<double> nullspace_basis = nullspace(A);

	cout << "Basis of null space of A:" << endl;

	for (int i = 0; i < nullspace_basis.size; i++) {
		xdd::printVector(nullspace_basis[i]);
		cout << "\n" << endl;
	}

	return 0;
}