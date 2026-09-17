#include <iostream>
#include <sstream>
#include <string>
// #include <vector>
using namespace std;

template <typename T>

struct vector {
  int size;
  T *data;
  int capacity;

  // constructor
  vector(int init_capacity = 2) {
    capacity = init_capacity;
    size = 0;
    data = new T[capacity];
  }

  vector(int init_size, const T &init_value) {
    size = init_size;
    capacity = init_size;
    data = new T[capacity];

    for (int i = 0; i < size; i++) {
      data[i] = init_value;
    }
  }
  // 拷貝建構子 copy constructor
  // 當全新的vector B要長的跟A一樣時(vector B=A;),
  // 執行Deep Copy 避免兩個vector共用同一塊Heap記憶體(Shellow Copy),
  // 防止程式結束時delete一塊記憶體兩次導致crash.
  vector(const vector &other) {
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
      data[i] = other.data[i];
    }
  }

  // 賦值運算子 copy assignment operator
  // 當兩個已經存在的vector要進行等號賦值時(B=A;)
  // 處理舊的記憶體 並支援連續賦值(A=B=C;)
  vector &operator=(const vector &other) {
    // 自我檢查(防止下一步刪掉自己的記憶體)
    if (this == &other) {
      return *this;
    }
    delete[] data;
    size = other.size;
    capacity = other.capacity;
    data = new T[capacity];
    for (int i = 0; i < size; i++) {
      data[i] = other.data[i];
    }
    // 回傳自身的參考 讓他支援連續等號
    return *this;
  }

  // destructor
  ~vector() {
    delete[] data;
    data = nullptr;
  }

  // 回傳＆才能真正修改資料值
  T &operator[](int index) { return data[index]; }

  const T &operator[](int index) const { return data[index]; }

  void push_back(T value) {
    if (size == capacity) {
      if (capacity == 0) {
        capacity = 1;
      } else {
        capacity *= 2;
      }
      T *newData = new T[capacity];
      for (int i = 0; i < size; i++) {
        newData[i] = data[i];
      }
      delete[] data;
      data = newData;
    }
    data[size] = value;
    size++;
  }

  void clear() { size = 0; }
};

bool checkMatrixSize(int columnA, int rowB) { return columnA == rowB; }

void fillMatrix(vector<vector<int>> &M) {
  // 檢查 cin 的緩衝區裡面有沒有殘留換行符號，有的話才清除
  if (cin.peek() == '\n') {
    cin.ignore();
  }
  int row = M.size;
  int column = M[0].size;
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
  int rowM = M.size;
  int colM = M[0].size;
  vector<vector<int>> Trans(colM, vector<int>(rowM, 0));
  for (int i = 0; i < rowM; i++) {
    for (int j = 0; j < colM; j++) {
      Trans[j][i] = M[i][j];
    }
  }
  return Trans;
}

vector<vector<int>> matrix_multi(const vector<vector<int>> &A,
                                 const vector<vector<int>> &BT) {
  int num = 0;
  int rowA = A.size;
  int colA = A[0].size;
  int colB = BT.size;
  int colBT = BT[0].size;
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

  for (int i = 0; i < ans.size; i++) {
    for (int j = 0; j < ans[0].size; j++) {
      cout << ans[i][j] << "\t";
    }
    cout << "\n";
  }

  return 0;
}
