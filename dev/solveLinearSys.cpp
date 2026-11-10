#include <cmath>
#include <iostream>
#include "my_vector.hpp"
using namespace std;
using namespace xdd;

bool isLinearSysSolvable(const vector<vector<double> > &M)
{
	int a = 0;
	int row = M.size;
	int col = M[0].size;
	//在GE後執行
	//從最後一個row開始檢查可以最小化計算時間
	//此時M是upper triangular,只需要從M[i][i]開始檢查
	for (int i = row - 1; i > (-1); i--) {
		a = -1; //先預設無解
		for (int j = i; j < col; j++) {
			if (std::abs(M[i][j]) > 1e-9) {
				a = 1;
				break;
			}
		}
		if (a == -1) {
			return false;
		}
	}
	return true;
}

void extendedMatrixGE(vector<vector<double> > &A, vector<vector<int> > &P, vector<double> &b)
{
	int row, col;
	row = A.size;
	col = A[0].size;
	int GE_level;
	if (row < col) {
		GE_level = row;
	} else {
		GE_level = col;
	}
	for (int i = 0; i < GE_level; i++) {
		double pivot = A[i][i];
		int count = i;
		while (std::fabs(pivot) < 1e-9 && (count + 1) < row) {
			A[count][i] = 0; //has a vaey small pivot be 0
			count++;
			pivot = A[count][i];
		}

		if (count != i) { // 代表要作row change
			swap(A[i], A[count]);
			swap(P[i], P[count]);
			swap(b[i], b[count]);
		}

		for (int j = i + 1 /*當前的row不用作消去*/; j < row; j++) {
			double elim = A[j][i];
			// 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
			// 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
			double multiplier = elim / pivot;

			if (elim == 0) {
				continue;
			}
			for (int k = i + 1 /*因為A[j][i]最後一定會變成0,所以直接從下一個開始*/
			     ;
			     k < col; k++) {
				double coff =
					A[i][k] * multiplier; // 當pivot那一row對應col的元素*倍率
				A[j][k] = A[j][k] - coff;
			}
			A[j][i] = 0.0; //手動賦值

			b[j] -= b[i] * multiplier;
		}
	}
}

vector<double> backSub(const vector<vector<double> > &A, const vector<vector<int> > &P,
		       const vector<double> &b)
{ // Back substitution
	int row = b.size;
	int col = A[0].size;
	// 理論上col=row
	double temp_b;
	vector<double> value(row,
			     NAN); // 這是用來算出數值的向量 沒有處理因為row change造成的錯位
	vector<double> x(row, NAN); // 這是要回傳的向量 需要重新排序
	for (int i = row - 1; i > (-1); i--) { // 由下往上
		temp_b = b[i];
		for (int j = i + 1; j < col; j++) { //起點是第i row第i+1 col
			temp_b -= A[i][j] * value[j];
			//要用value不能用x因為這裡都是有經過row change的
		}
		temp_b /= A[i][i];
		value[i] = temp_b;

		// 處理x的順序因為row change而亂掉的問題
		for (int j = 0; j < col; j++) {
			if (P[i][j] == 1) {
				x[j] = temp_b;
				//cout << "value[" << i << "]=x[" << j << "]." << endl;
				//debugInfo
				break;
			}
		}
	}
	return x;
}

void debugInfo(const vector<vector<double> > &A, const vector<vector<int> > &P,
	       const vector<double> &b)
{
	cout << "A after GE:" << endl;
	printMatrix(A);
	cout << "P:" << endl;
	printMatrix(P);
	cout << "b after GE:" << endl;
	printVector(b);
}

vector<double> solLinearSys(matrix<double> &A, vector<double> &b)
{
	int row = A.size;
	int col = A[0].size;

	vector<vector<int> > P(row, vector<int>(row, 0));
	for (int i = 0; i < row; i++) {
		P[i][i] = 1;
	}

	extendedMatrixGE(A, P, b);

	if (!isLinearSysSolvable(A)) {
		cout << "This matrix is not solvable or has infinite solution!" << endl;
		cout << "R_0 of A:" << endl;
		printMatrix(A);
		cout << "b:" << endl;
		printVector(b);
		return NAN;
	}

	vector<double> x = backSub(A, P, b);

	return x;
}

int main()
{
	cout << "----Solve Linear System----" << endl;
	int row, col;
	cout << "The size of matrix A.(row,column)" << endl;
	cin >> row >> col;

	if (row != col) {
		cout << "Sorry, this program only support square matrix.";
		return 0;
	}

	vector<vector<double> > A(row, vector<double>(col, 0));
	cout << "Put in the value of the matrix A." << endl;
	fillMatrix(A);

	cout << "Put in the value of the vector b." << endl;
	vector<double> b(row, 0);
	fillVector(b);

	vector<double> x = solLinearSys(A, b);

	cout << "result of Ax=b,x=" << endl;
	printVector(x);
	return 0;
}
