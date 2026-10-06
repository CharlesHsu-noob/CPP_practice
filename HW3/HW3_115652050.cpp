#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

// 每次在用的時候都要在函式上方重新宣告
template <typename T> struct vector {
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

template <typename T> void printVector(const vector<T> &V) {
  int digits = 4;
  digits *= -1;
  double precision = pow(10, digits);
  int len = V.size;
  for (int i = 0; i < len; i++) {
    if (abs(V[i]) < precision) {
      cout << setw(10) << 0;
    } else {
      cout << setw(10) << fixed << setprecision(digits * (-1)) << V[i];
    }
  }
  cout << endl;
}

template <typename T> void printMatrix(const vector<vector<T>> &M) {
  int row = M.size;
  for (int i = 0; i < row; i++) {
    printVector(M[i]);
  }
}

template <typename T> void fillMatrix(vector<vector<T>> &M) {
  int row = M.size;
  cout << "Put the element here(by row):\n";
  cout << "Put all value in the same row in same line and divide them by "
          "<space> and press <Enter> after finished a row."
       << endl;
  for (int i = 0; i < row; i++) {
    fillVector(M[i]);
  }
}

template <typename T> void fillVector(vector<T> &V) {
  // 檢查 cin 的緩衝區裡面有沒有殘留換行符號，有的話才清除
  if (cin.peek() == '\n') {
    cin.ignore();
  }
  int length = V.size;
  // cout<<"Put all value in same line and divide them by <space>."<<endl;
  string line;

  getline(cin, line);
  stringstream ssline(line);
  T num;
  int i;
  V.clear();
  while (ssline >> num) {
    V.push_back(num);
  }
  line = "";
}

void extendedMatrixGE(vector<vector<double>> &A, vector<vector<int>> &P,
                      vector<double> &b) {
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
      swap(A[i], A[count]);
      swap(P[i], P[count]);
      swap(b[i], b[count]);
    }

    for (int j = i + 1 /*當前的row不用作消去*/; j < row; j++) {
      double elim = A[j][i];
      // 這一個row的所有元素都會用到這個元素，但這個元素在地一個迴圈就會被消去，所以要額外寫一個變數保留他
      // 即row_j -= row_pivot * {row_j第一個非0元素}/pivot
      if (elim == 0) {
        continue;
      }
      for (int k = i /*這代表當前的col座標所以不用加1*/; k < col; k++) {
        double coff = A[i][k] * elim; // 當pivot那一row對應col的元素*倍率
        coff /= pivot;
        A[j][k] = A[j][k] - coff;
      }
      double coff_b = elim / pivot;
      b[j] -= b[i] * coff_b;
    }
  }
}

vector<double> backSub(const vector<vector<double>> &A,
                       const vector<vector<int>> &P,
                       const vector<double> &b) { // Back substitution
  int row = b.size;
  int col = A[0].size;
  // 理論上col=row
  int index = 0;
  double temp_b;
  vector<double> value(
      row, NAN); // 這是用來算出數值的向量 沒有處理因為row change造成的錯位
  vector<double> x(row, NAN);            // 這是要回傳的向量 需要重新排序
  for (int i = row - 1; i > (-1); i--) { // 由下往上
    temp_b = b[i];
    for (int j = 0; j < (col - i + 1); j++) {
      temp_b -= A[i][j] * x[j];
    }
    temp_b /= A[i][i];
    value[i] = temp_b;
    // 處理x的順序因為row change而亂掉的問題
  }
}

void solLinearSys() {
  cout << "----Solve Linear System----" << endl;
  int row, col;
  cout << "The size of matrix A.(row,column)" << endl;
  cin >> row >> col;
  vector<vector<double>> A(row, vector<double>(col, 0));
  cout << "Put in the value of the matrix A." << endl;
  fillMatrix(A);
  vector<vector<int>> P(row, vector<int>(row, 0));
  for (int i = 0; i < row; i++) {
    P[i][i] = 1;
  }

  cout << "Put in the value of the vector b." << endl;
  vector<double> b(row, 0);
  fillVector(b);

  extendedMatrixGE(A, P, b);
}

int main() {
  solLinearSys();
  return 0;
}
