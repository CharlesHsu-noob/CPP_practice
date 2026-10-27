#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

bool checkMatrixSize(int columnA, int rowB) { return columnA == rowB; }

void fillMatrix(vector<vector<int>> &M) {
  // 檢查 cin 的緩衝區裡面有沒有殘留換行符號，有的話才清除
  if (cin.peek() == '\n') {
    cin.ignore();
  }
  int row = M.size();
  int column = M[0].size();
  cout << "Put the element here(by row):\n";
  string line;

  for (int i = 0; i < row; i++) {
    getline(cin, line);
    stringstream ssline(line);
    int num;
    M[i].clear();
    while (ssline >> num) {
      M[i].push_back(num);
    }
    line = "";
  }
}

vector<vector<int>> transMatrix(const vector<vector<int>> &M) {
  int rowM = M.size();
  int colM = M[0].size();
  vector<vector<int>> T(colM, vector<int>(rowM, 0));
  for (int i = 0; i < rowM; i++) {
    for (int j = 0; j < colM; j++) {
      T[j][i] = M[i][j];
    }
  }
  return T;
}

vector<vector<int>> matrix_multi(const vector<vector<int>> &A,
                                 const vector<vector<int>> &BT) {
  int num = 0;
  int rowA = A.size();
  int colA = A[0].size();
  int colB = BT.size();
  int colBT = BT[0].size();
  // colA=colBT=rowB
  // colBT=rowB
  vector<vector<int>> result(rowA, vector<int>(colB, 0));
  for (int i = 0; i < rowA; i++) {
    for (int j = 0; j < colB; j++) {
      for (int k = 0; k < colA; k++) {
        num += A[i][k] * BT[j][k];
      }
      result[i][j] = num;
      num = 0;
    }
  }
  return result;
}

int main() {
  bool inputCorrect = false;
  int matrix_size[2][2] = {{0, 0}, {0, 0}};
  while (!inputCorrect) {
    cout << "The size of matrix A(row,column):";
    cin >> matrix_size[0][0] >> matrix_size[0][1];
    // 總共有len(column)=row個vector,每個vector有len(row)=column個變量
    cout << "The size of matrix B(row,column):";
    cin >> matrix_size[1][0] >> matrix_size[1][1];
    if (checkMatrixSize(matrix_size[0][1], matrix_size[1][0])) {
      inputCorrect = true;
    } else {
      cout << "Wrong matirx size! Can't calculate multiplication." << endl;
    }
  }
  vector<vector<int>> A(matrix_size[0][0], vector<int>(matrix_size[0][1], 0));
  vector<vector<int>> B(matrix_size[1][0], vector<int>(matrix_size[1][1], 0));
  cout << "Matrix A\n";
  fillMatrix(A);
  cout << "Matrix B\n";
  fillMatrix(B);

  vector<vector<int>> ans = matrix_multi(A, transMatrix(B));

  for (int i = 0; i < ans.size(); i++) {
    for (int j = 0; j < ans[0].size(); j++) {
      cout << ans[i][j] << "\t";
    }
    cout << "\n";
  }

  return 0;
}
