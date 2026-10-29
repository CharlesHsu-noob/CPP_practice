#ifndef MY_VECTOR_HPP
#define MY_VECTOR_HPP

#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <algorithm>

namespace xdd
{
template <typename T> struct vector {
	int size;
	T *data;
	//因為strcut裡面包含了new 所以一定要自己寫rule of 5
	//否則讓編譯器自己生成會發生double free造成程式崩潰
	int capacity;

	// constructor
	vector(int init_size = 0)
	{
		size = init_size;
		capacity = (init_size > 0) ? init_size : 2;
		data = new T[capacity](); //()的作用是如果T是int或double編譯器會自動填滿0
	}

	vector(int init_size, const T &init_value)
	{
		size = init_size;
		capacity = init_size;
		data = new T[capacity];

		for (int i = 0; i < size; i++) {
			data[i] = init_value;
		}
	}
	// 拷貝建構子 copy constructor
	// 當全新的vector B要長的跟A一樣時(vector B=A;),
	// 執行Deep Copy 避免兩個vector共用同一塊Heap記憶體(Shallow Copy),
	// 防止程式結束時delete一塊記憶體兩次導致crash.
	vector(const vector &other)
	{
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
	vector &operator=(const vector &other)
	{
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

	/*Move constructor 和 Move Assignment operator
                是專門為了不必要的Deep Copy而生
                以A=B舉例
                當被賦予的值(B)是R-value時就會觸發Move Assignment operator
                如果B是L-value則會使用Copy Assignment operator
                如果是新建的變數vector<T> A = B; 
                會觸發 Copy Constructor；
                而如果是 vector<T> A = std::move(B); 或等號右邊是暫存結果，則會觸發 Move Constructor。*/

	//Move constructor移動建構子
	//在執行swap(vector<T>,vector<T>)的時候為了避免編譯器temp一份新的記憶體然後把東西全部複製過去
	//這個建構子使他只會交換記憶體位置(pointer)而不會造成時間複雜度過高
	//這個建構子適用於 vector<T> temp=std::move(A)
	vector(vector &&other) noexcept
	/*
                * &&other(右參照值):即將被刪除,可以搶走的物件(R-value)
                * noexcept:讓swap不要報錯*/
	{
		//這裡只交換記憶體位置而不是用for迴圈把所有資料值交換
		size = other.size;
		data = other.data;
		capacity = other.capacity;

		other.size = 0;
		other.capacity = 0;
		other.data = nullptr;
		//要把other的記憶體位置設為nullptr
		//不然other執行destructor的時候會把這個記憶體位置的資料清掉
	}

	//Move Assignment operator
	//防止自己賦值給自己
	//適用於 vector<T> A(a,x);
	//      vector<T> B(b,y);
	//此時A,B的大小並不一樣
	//B=std::move(A);
	//B需要先處理好自己的內容才能接收A的內容
	vector &operator=(vector &&other) noexcept
	{
		if (this != &other) {
			delete[] data;
			size = other.size;
			capacity = other.capacity;
			data = other.data;

			other.size = 0;
			other.capacity = 0;
			other.data = nullptr;
		}
		return *this;
	}

	// destructor
	~vector()
	{
		delete[] data;
		data = nullptr;
	}

	// 回傳＆才能真正修改資料值
	T &operator[](int index)
	{
		return data[index];
	}

	const T &operator[](int index) const
	{
		return data[index];
	}

	void push_back(T value)
	{
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

	void clear()
	{
		size = 0;
	}

	// ---- 迭代器定義與函式 ----

	// 定義迭代器別名，這讓外部能使用 xdd::vector<T>::iterator
	using iterator = T *;
	using const_iterator = const T *;

	// 1. 非 const 版本：允許透過迭代器修改裡面的值（例如：*v.begin() = 10;）
	iterator begin() noexcept
	{
		return data; // 指向第一個元素 data[0] 的記憶體位置
	}

	iterator end() noexcept
	{
		return data + size; // 指向最後一個元素的下一個位置（Past-the-end）
	}

	// 2. const 版本：當 vector 物件是 const 時（例如 printVector 函式內），必須呼叫此版本
	const_iterator begin() const noexcept
	{
		return data;
	}

	const_iterator end() const noexcept
	{
		return data + size;
	}
};

template <typename T> using matrix = vector<vector<T> >;

template <typename T> void printVector(const vector<T> &V)
{
	int digits = 4;
	digits *= -1;
	double precision = pow(10, digits);
	int len = V.size;
	for (int i = 0; i < len; i++) {
		if (std::fabs(V[i]) < precision) {
			std::cout << std::setw(10) << 0;
		} else {
			std::cout << std::setw(10) << std::fixed << std::setprecision(digits * (-1))
				  << V[i];
		}
	}
	std::cout << std::endl;
}

template <typename T> void printMatrix(const matrix<T> &M)
{
	int row = M.size;
	for (int i = 0; i < row; i++) {
		printVector(M[i]);
	}
}

template <typename T> void fillVector(vector<T> &V)
{
	// 檢查 cin 的緩衝區裡面有沒有殘留換行符號，有的話才清除
	if (std::cin.peek() == '\n') {
		std::cin.ignore();
	}
	//int length = V.size;
	// cout<<"Put all value in same line and divide them by <space>."<<endl;
	std::string line;

	std::getline(std::cin, line);
	std::stringstream ssline(line);
	T num;
	int i;
	V.clear();
	while (ssline >> num) {
		V.push_back(num);
	}
	line = "";
}

template <typename T> void fillMatrix(matrix<T> &M)
{
	int row = M.size;
	std::cout << "Put the element here(by row):\n";
	std::cout << "Put all value in the same row in same line and divide them by "
		     "<space> and press <Enter> after finished a row."
		  << std::endl;
	for (int i = 0; i < row; i++) {
		fillVector(M[i]);
	}
}

template <typename T> void removeBlacklist(vector<T> &V, const vector<T> &blacklist)
{
	auto new_end = std::remove_if(V.begin(), V.end(), [&blacklist](const T &x) {
		return std::find(blacklist.begin(), blacklist.end(), x) != blacklist.end();
	});
	V.size = new_end - V.begin();
}

}

#endif
