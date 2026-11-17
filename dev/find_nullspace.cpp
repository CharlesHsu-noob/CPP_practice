#include <iostream>
#include <cmath>
#include "my_vector.hpp"

using namespace std;

//include from CR_decompose.cpp
pair<xdd::matrix<double>, xdd::matrix<double> > CR_decompose(xdd::matrix<double> &A);
//end include

xdd::vector<int> getPivotCol(const xdd::matrix<double> &M)
{
	double tolerance = 1e-9;
	//require M is in R_0 type
	int row = M.size;
	int col = M[0].size;
	xdd::vector<int> pivotCol;

	for (int i = 0; i < row; i++) {
		for (int j = i; j < col; j++) {
			if (std::fabs(M[i][j]) > tolerance) {
				pivotCol.push_back(j);
				break;
			}
		}
	}
	return pivotCol;
}

/*
xdd::vector<int> getFreeCol(const xdd::matrix<double> &M)
{
	xdd::vector<int> freeCol;
	for (int i = 0; i < M[0].size; i++) {
		freeCol[i] = i;
	}
	xdd::vector<int> pivotCol = getPivotCol(M);
	xdd::removeByValue(freeCol, pivotCol);
	return freeCol;
}*/

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
			for (int j = 0; j < M.size; j++) {
				swap(M[j][i], M[j][pivotCol[i]]);
			}
			swap(P[pivotCol[i]], P[i]);
		}
	}
	return P;
}
xdd::vector<xdd::vector<double> > nullspace(xdd::matrix<double> &A)
{
	xdd::matrix<double> R = CR_decompose(A).second;

	//處理零矩陣
	if (R.size == 0) {
		int col = A[0].size;
		xdd::vector<xdd::vector<double> > basis(col, xdd::vector<double>(col, 0));
		for (int i = 0; i < col; i++) {
			basis[i][i] = 1.0;
		}
		return basis;
	}

	//std::tie(C, R) = CR_decompose(A);
	//C因為不會用到所以可以直接不接收
	xdd::vector<int> pivotCol = getPivotCol(R);

	if (R[0].size == pivotCol.size) {
		return xdd::vector<xdd::vector<double> >(0);
	}
	xdd::vector<int> P = colChange(R, pivotCol);

	//x_n.size=freeCol.size=R[0].size-pivotCol.size
	//here x_n is wrong sort, need resort later
	xdd::matrix<double> x_n(R[0].size - pivotCol.size, xdd::vector<double>(R[0].size, 0));

	//xdd::vector<double> b(pivotCol.size + 1, 0);
	//b[pivotCol.size] = 1;
	for (int i = 0; i < x_n.size; i++) {
		/*
                *為什麼這裡可以省略建構 temp_A 與呼叫 backSub？
                * 1. CR_decompose 所得出的 R 是簡約列梯形矩陣 (RREF)。
                * 2. 經過 colChange 將所有 pivot column 集中到左側後，R 的非零列會呈現分塊形式 [I_r | F]，
                *    其中左半部 I_r 為 r*r 的單位矩陣 (r = pivotCol.size)，右半部 F 為自由變數對應的係數矩陣。
                * 3. 求解齊次方程組 [I_r | F] * [x_p ; x_f] = 0 時，展開即為 I_r * x_p + F * x_f = 0，
                *    移項可得主變數 x_p = -F * x_f。
                * 4. 當我們將第 i 個自由變數設為 1 (其餘自由變數設為 0) 來求第 i 個基底向量時，
                *    主變數 x_p 的解就剛好是 F 的第 i 個 column 直接取負號 (即 -R[j][pivotCol.size + i])。
                * 5. 因為左半部已經是對角線為 1、其餘為 0 的單位矩陣 I_r，無須再透過高斯回代 (backSub) 消去，
                *    直接讀取並取負號即可，時間複雜度由 O(r^2) 降至 O(r)，同時省去配置 temp_A 的記憶體。
                *
		//可以優化(using M.block or M.view)
		xdd::matrix<double> temp_A(b.size, xdd::vector<double>(b.size, 0));
		for (int j = 0; j < temp_A.size - 1; j++) {
			for (int k = j; k < temp_A.size - 1; k++) {
				temp_A[j][k] = R[j][k];
			}
			//b.size-1=pivotCol.size
			temp_A[j][pivotCol.size] = R[j][pivotCol.size + i];

		//b.size-1=pivotCol.size
		temp_A[pivotCol.size][pivotCol.size] = 1;
		xdd::vector<double> temp_x = backSub(temp_A, b);
		}*/

		for (int j = 0; j < pivotCol.size; j++) {
			x_n[i][j] = -R[j][pivotCol.size + i]; // 直接取負號，免去 backSub
		}
		x_n[i][pivotCol.size + i] = 1.0;
	}

	//resort x_n
	for (int i = 0; i < x_n.size; i++) {
		//總共有i個row要檢查 i代表row index
		xdd::resortVector(x_n[i], P);
	}

	return x_n;
}

int main()
{
	cout << "----Find nullspace----" << endl;
	xdd::matrix<double> A = xdd::readMatrixFromIstream<double>();

	xdd::matrix<double> nullspace_basis = nullspace(A);

	cout << "Basis of null space of A:" << endl;

	if (nullspace_basis.size == 0) {
		cout << "{0}" << endl;
		return 0;
	}
	for (int i = 0; i < nullspace_basis.size; i++) {
		cout << "[";
		xdd::printVector(nullspace_basis[i]);
		cout << "]" << endl;
	}

	return 0;
}
