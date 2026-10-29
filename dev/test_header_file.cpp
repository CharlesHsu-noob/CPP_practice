#include <iostream>
// 假設你的檔案叫做 my_complex.h。如果在 include 資料夾內，請改成 "include/my_complex.h"
#include "my_complex.h"

using namespace std;
using namespace xdd; // 引用你自訂的命名空間

int main()
{
	// 測試建構子
	complex<double> c1(1.5, 2.0);
	complex<double> c2(3.0, -1.0);

	// 測試輸出運算子 (<<)
	cout << "c1 = " << c1 << "\n";
	cout << "c2 = " << c2 << "\n";

	// 測試加法與乘法
	cout << "c1 + c2 = " << (c1 + c2) << "\n";
	cout << "c1 * c2 = " << (c1 * c2) << "\n";

	// 測試絕對值函式
	cout << "abs(c1) = " << c1.abs() << "\n";

	// 測試輸入運算子 (>>)
	complex<double> c3;
	cout << "請輸入一個複數 (格式如 (1,2) 或 3): ";
	if (cin >> c3) {
		cout << "你輸入的是: " << c3 << "\n";
	} else {
		cout << "輸入格式錯誤！\n";
	}

	return 0;
}
