#include <cmath>
#include <iostream>
#include "my_vector.hpp"

using namespace std;

void LbackSub(const xdd::matrix<double> &A, const xdd::vector<double> &b,
	      xdd::vector<double> &value)
{ // Back substitution
	// LU version
	//A同時包含L和U
	int row = b.size;
	int col = A[0].size;
	// 理論上col=row
	double temp_b;
	for (int i = 0; i < row; i++) { // 由上往下
		temp_b = b[i];
		for (int j = 0; j < i; j++) {
			temp_b -= A[i][j] * value[j];
		}
		//L的對角線是1所以不需要除
		value[i] = temp_b;
	}
}

void UbackSub(const xdd::matrix<double> &A, const xdd::vector<double> &b,
	      xdd::vector<double> &value)
{ // Back substitution
	// LU version
	//A同時包含L和U
	int row = b.size;
	int col = A[0].size;
	// 理論上col=row
	double temp_b;
	for (int i = row - 1; i > (-1); i--) { // 由下往上
		temp_b = b[i];
		for (int j = i + 1; j < col; j++) { //起點是第i row第i+1 col
			temp_b -= A[i][j] * value[j];
		}
		temp_b /= A[i][i];
		value[i] = temp_b;
	}
}

void decomposeLU(xdd::matrix<double> &A, xdd::vector<int> &P_idx)
{
	int row = A.size;
	int col = A[0].size;
	int GE_level;
	if (row < col) {
		GE_level = row;
	} else {
		GE_level = col;
	}
	for (int i = 0; i < GE_level; i++) {
		/*
         *這個矩陣消去完後就會變成U
         *而U是一個上三角矩陣,所有下半部元素都是0(包含對角線),
         *所以可以直接把下半部拿來儲存L,因為L是下三角矩陣(對角線上的數字都為1所以也不需要額外存)
         *所以最後A會變成一個合體矩陣
         * */
		double pivot = A[i][i];
		int count = i;
		while (pivot == 0 && (count + 1) < row) {
			count++;
			pivot = A[count][i];
		}
		if (count == row) {
			continue;
		} // 如果這個col全部都是0那就直接換下一個col作消去
		if (count != i) { // 代表要作row change
			swap(A[i], A[count]);
			swap(P_idx[i], P_idx[count]);
		}
		for (int j = i + 1 /*當前的row不用作消去*/; j < row; j++) {
			double elim = A[j][i];
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= {row_pivot/pivot} * {row_j第一個非0元素}
			double multiplier = elim / pivot;
			if (elim == 0) {
				continue;
			}
			for (int k = i + 1; k < col; k++) {
				// 當pivot那一行對應col的元素*倍率
				A[j][k] -= A[i][k] * multiplier;
			}
			A[j][i] =
				multiplier; //因為這個地方永遠都會是0,所以直接把他拿來儲存L矩陣裡面的數值,不用重新宣告一個變數
		}
	}
}

void solInverseVector(const xdd::matrix<double> &LU, const xdd::vector<int> &P_idx,
		      xdd::matrix<double> &Ainv)
{
	//此時A上半部是U,下半部是L
	//接下來要求U^-1,L^-1
	//Let PAx=LUx=Pe,[x1,x2,x3...]=A^-1 ; [e1,e2,e3...]=I.
	//let Ux=y, then Ly=Pe.=>Pe_i=P[i]
	int row = LU.size;
	int col = LU[0].size;
	xdd::vector<double> y(row, 0.0);
	xdd::vector<double> x(row, 0.0);
	xdd::vector<double> e(row, 0.0);
	for (int i = 0; i < col; i++) {
		//重置e
		for (int j = 0; j < row; j++) {
			e[j] = 0;
		}
		//找出1應該要放在e的哪一個位置
		for (int j = 0; j < row; j++) {
			if (P_idx[j] == i) {
				e[j] = 1.0;
				break;
			}
		}

		LbackSub(LU, e, y);
		UbackSub(LU, y, x);
		for (int j = 0; j < row; j++) {
			Ainv[j][i] = x[j];
		}
	}
}

void inverse(xdd::matrix<double> A_copy, xdd::matrix<double> &Ainv)
{
	int row;
	row = A_copy.size;

	xdd::vector<int> P_idx(row);
	for (int i = 0; i < row; i++) {
		P_idx[i] = i;
	}

	decomposeLU(A_copy, P_idx);

	solInverseVector(A_copy, P_idx, Ainv);
}

void findMatrixInverse()
{
	cout << "----Find Matrix Inverse----" << endl;
	int row, col;
	cout << "The size of matrix A.(row,column)" << endl;
	cin >> row >> col;
	xdd::matrix<double> A(row, xdd::vector<double>(col, 0));
	cout << "Put in the value of the matrix A." << endl;
	fillMatrix(A);

	xdd::matrix<double> Ainv(row, xdd::vector<double>(col, NAN));
	inverse(A, Ainv);

	cout << "----Result----\n" << "A inverse=>" << endl;
	printMatrix(Ainv);
}

int main()
{
	findMatrixInverse();
	return 0;
}
