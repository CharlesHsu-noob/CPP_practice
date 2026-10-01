#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

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

void printMatrix(const vector<vector<double>> &M) {
  int row = M.size;
  int col = M[0].size;
  for (int i = 0; i < row; i++) {
    for (int j = 0; j < col; j++) {
      if (abs(M[i][j]) < 1e-4) {
        cout << "\t" << 0;
      } else {
        cout << "\t" << fixed << setprecision(4) << M[i][j];
      }
    }
    cout << endl;
  }
}

void fillMatrix(vector<vector<double>> &M) {
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

void GE(vector<vector<double>> &A, vector<vector<double>> &P) {
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
    while (pivot == 0 && (count + 1) < row) {
      count++;
      pivot = A[count][i];
    }
    if (count == row) {
      continue;
    } // 如果這個col全部都是0那就直接換下一個col作消去
    if (count != i) { // 代表要作row change
      vector<double> temp = A[i];
      A[i] = A[count];
      A[count] = temp;
      temp = P[i];
      P[i] = P[count];
      P[count] = temp;
    }
    for (int j = i + 1 /*當前的row不用作消去*/; j < row; j++) {
      double elim = A
          [j]
          [i]; // 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
               // 即row_j -= {row_pivot/pivot} * {row_j第一個非0元素}
      if (elim == 0) {
        continue;
      }
      for (int k = i /*這代表當前的col座標所以不用加1*/; k < col; k++) {
        double coff = A[i][k] * elim; // 當pivot那一行對應col的元素*倍率
        coff /= pivot;
        A[j][k] = A[j][k] - coff;
      }
    }
  }
}

void solLinearSys() {
  cout << "----Solve Linear System----" << endl;
  int row, col;
  cout << "The size of matrix.(row,column)" << endl;
  cin >> row >> col;
  vector<vector<double>> A(row, vector<double>(col, 0));
  cout << "Put in the value of the matrix." << endl;
  fillMatrix(A);
  vector<vector<double>> P(row, vector<double>(row, 0));
  for (int i = 0; i < row; i++) {
    P[i][i] = 1;
  }
  GE(A, P);

  cout << "----Result----\n" << "A=>" << endl;
  printMatrix(A);
  cout << "P=" << endl;
  printMatrix(P);
}

int main() {
  solLinearSys();
  return 0;
}
