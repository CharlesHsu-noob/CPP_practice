#include <cmath>
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

void GE(vector<vector<double>> A, vector<vector<double>> P) {}

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
    P[i][i] = 0;
  }
}

int main() {
  solLinearSys();
  return 0;
}
