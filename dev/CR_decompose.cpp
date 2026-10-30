#include <iostream>
#include "my_vector.hpp"

using namespace std;

//return free column's posision
xdd::vector<int> GE(xdd::matrix<double> &A, xdd::matrix<int> &P)
{
	/*CR分解不需要row change*/
	int row, col;
	row = A.size;
	col = A[0].size;
	xdd::vector<int> freeCol;
	int freeColCount = 0;
	int GE_level;
	if (row > col) {
		GE_level = row;
	} else {
		GE_level = col;
	}

	for (int i = 0; i < GE_level; i++) {
		//每一個col都從 [當前row-已經有的free col數] 開始找
		int currentRow = i - freeColCount;
		if (currentRow > row) {
			break;
		}
		double pivot = A[currentRow][i];
		cout << "pivot=A[" << currentRow << "][" << i << "]" << endl;
		int count = currentRow;
		while (pivot == 0 && (count + 1) < row) {
			count++;
			pivot = A[count][i];
		}

		if (pivot == 0) {
			cout << "column " << i << " is free column, pass." << endl;
			freeCol.push_back(i);
			freeColCount++;
			continue;
		} // 如果這個col全部都是0那就直接換下一個col作消去

		cout << "count:" << count << ", row:" << row << endl;
		cout << "pivot of column " << i << " is " << pivot << endl;

		if (count != currentRow) { // 代表要作row change
			swap(A[currentRow], A[count]);
			swap(P[currentRow], P[count]);
		}

		for (int j = currentRow + 1 /*當前的row不用作消去*/; j < row; j++) {
			double elim = A[j][i];
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
			double multiplier = elim / pivot;
			if (elim == 0) {
				continue;
			}
			for (int k = i + 1 /*因為A[j][i]最後一定會變成0,所以直接從下一個開始*/;
			     k < col; k++) {
				// 當pivot那一row對應col的元素*倍率
				double coff = A[currentRow][k] * multiplier;
				A[j][k] = A[j][k] - coff;
			}
			A[j][i] = 0.0; //手動賦值
			//xdd::printMatrix(A);
			//cout << endl;
		}
	}
	return freeCol;
}
//return pivot column
xdd::vector<int> backGE(xdd::matrix<double> &A, xdd::vector<int> &freeCol)
{
	/*CR分解不需要row change*/
	int row, col;
	row = A.size;
	col = A[0].size;

	xdd::vector<int> pivotCol(col, 0);
	for (int i = 0; i < col; i++) {
		pivotCol[i] = i;
	}
	//只需要對pivot col作消去 free column不用管他
	xdd::removeBlacklist(pivotCol, freeCol);
	int GE_level = pivotCol.size;

	int currentRow = row - 1;
	for (int i = GE_level - 1; i > (-1); i--) {
		cout << "now elim col " << pivotCol[i] << endl;
		int currentCol = pivotCol[i];
		double pivot = A[currentRow][currentCol];
		int count = currentRow;
		while (pivot == 0 && (count - 1) > (-1)) {
			count--;
			pivot = A[count][currentCol];
		}
		currentRow = count;

		//cout << "pivot of column " << currentCol << " is " << pivot << endl;

		//因為GE已經確保他是上三角矩陣了所以不需要row change

		for (int j = currentRow - 1 /*當前的row不用作消去*/; j > (-1); j--) {
			double elim = A[j][currentCol];
			if (elim == 0) {
				continue;
			}
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
			double multiplier = elim / pivot;
			for (int k = currentCol +
				     1 /*因為A[j][i]最後一定會變成0,所以直接從下一個開始*/;
			     k < col; k++) {
				// 當pivot那一row對應col的元素*倍率
				double coff = A[currentRow][k] * multiplier;
				A[j][k] = A[j][k] - coff;
			}
			A[j][currentCol] = 0.0; //手動賦值
			//xdd::printMatrix(A);
			//cout << endl;
		}
		currentRow--;
	}
	return pivotCol;
}

//return C ,R use A's memory space
xdd::matrix<double> rref(xdd::matrix<double> &A)
{
	int row = A.size;
	int col = A[0].size;
	bool isZeroRow = true;
	xdd::vector<int> zeroRow;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			if (A[i][j] != 0) {
				isZeroRow = false;
				break;
			}
		}
		if (isZeroRow) {
			zeroRow.push_back(i);
		}
		isZeroRow = true;
	}
	xdd::removeByIndex(A, zeroRow);
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
	xdd::vector<int> freeCol, pivotCol;
	freeCol = GE(A, P);

	pivotCol = backGE(A, freeCol);

	xdd::resortMatrix(A, P);
}

int main()
{
	CR_decompose();
	return 0;
}
