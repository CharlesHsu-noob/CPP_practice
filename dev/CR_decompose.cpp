#include <iostream>
#include "my_vector.hpp"

using namespace std;

void GE(xdd::matrix<double> &A, xdd::matrix<int> &P)
{
	/*CR分解不需要row change*/
	int row, col;
	row = A.size;
	col = A[0].size;
	int GE_level;
	if (row < col) {
		GE_level = row;
	} else {
		GE_level = col;
	}
	bool nonElimination;
	for (int i = 0; i < GE_level; i++) {
		nonElimination = false;
		double pivot = A[i][i];
		int count = i;
		while (pivot == 0 && (count + 1) < row) {
			count++;
			pivot = A[count][i];
		}

		if (count == row) {
			nonElimination = true;
			continue;
		} // 如果這個col全部都是0那就直接換下一個col作消去

		if (count != i) { // 代表要作row change
			swap(A[i], A[count]);
			swap(P[i], P[count]);
		}

		for (int j = i + 1 /*當前的row不用作消去*/; j < row; j++) {
			double elim = A[j][i];
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
			double multiplier = elim / pivot;
			if (elim == 0) {
				continue;
			}
			for (int k = i + 1 /*因為A[j][i]最後一定會變成0,所以直接從下一個開始*/;
			     k < col; k++) {
				double coff =
					A[i][k] * multiplier; // 當pivot那一row對應col的元素*倍率
				A[j][k] = A[j][k] - coff;
			}
			A[j][i] = 0.0; //手動賦值
		}
	}
}

xdd::vector<pair<int, int> > backGE(xdd::matrix<double> &A, xdd::matrix<int> &P)
{
	xdd::vector<pair<int, int> > pivotCol;
	int row, col;
	pair<int, int> pos;
	row = A.size;
	col = A[0].size;
	int GE_level;
	if (row < col) {
		GE_level = row;
	} else {
		GE_level = col;
	}
	for (int i = GE_level - 1; i > (-1); i--) {
		double pivot = A[i][i];
		int count = i;
		while (pivot == 0 && count > (-1)) {
			count--;
			pivot = A[count][i];
		}

		if (count == 0) {
			continue;
		} // 如果這個col全部都是0那就直接換下一個col作消去

		if (count != i) { // 代表要作row change
			swap(A[i], A[count]);
			swap(P[i], P[count]);
		}

		for (int j = i - 1 /*當前的row不用作消去*/; j > (-1); j--) {
			double elim = A[j][i];
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
			double multiplier = elim / pivot;
			if (elim == 0) {
				continue;
			}
			for (int k = i + 1 /*因為A[j][i]最後一定會變成0,所以直接從下一個開始*/;
			     k < col; k++) {
				double coff =
					A[i][k] * multiplier; // 當pivot那一row對應col的元素*倍率
				A[j][k] = A[j][k] - coff;
			}
			A[j][i] = 0.0; //手動賦值
			xdd::printMatrix(A);
		}
		pos.first = count;
		pos.second = i;
		pivotCol.push_back(pos);
	}
	return pivotCol;
}

void rref(xdd::matrix<double> &M, xdd::vector<pair<int, int> > pivotCol)
{
	int numOfPivot = pivotCol.size;
	int col = M[0].size;
	int pivot_row = 0;
	int pivot_col = 0;
	double pivot = 0;
	for (int i = 0; i < numOfPivot; i++) {
		pivot = M[pivot_row][pivot_col];
		for (int j = pivot_col; j < col; j++) {
			M[i][j] /= M[pivot_row][pivot_col];
		}
	}
}

void CR_decompose()
{
	cout << "----CR decompose----" << endl;
	int row, col;
	cout << "The size of matrix A.(row,column)" << endl;
	cin >> row >> col;

	xdd::matrix<double> A(row, xdd::vector<double>(col, 0));
	cout << "Put in the value of the matrix A." << endl;
	xdd::fillMatrix(A);
	xdd::matrix<int> P(row, xdd::vector<int>(row, 0));
	for (int i = 0; i < row; i++) {
		P[i][i] = 1;
	}

	GE(A, P);
	xdd::printMatrix(A);
	xdd::vector<pair<int, int> > pivotCol;
	/*pivotCol裡面紀錄的方式:第n個元素代表第n個pivot在哪一個row,col
         * e.g. pivotCol=[<1,1>,<3,4>,<5,5>]
         * 代表pivot在(1,1),(3,4),(5,5)
         * col2 是free col*/
	pivotCol = backGE(A, P);
	//xdd::printMatrix(A);

	//rref(A, pivotCol);
	//xdd::printMatrix(A);
}

int main()
{
	CR_decompose();
	return 0;
}
