#ifndef MY_COMPLEX_HPP
#define MY_COMPLEX_HPP

#ifndef MY_COMPLEX_H
#include "my_complex.h"
#endif

namespace xdd
{
//constructor
template <typename T>
complex<T>::complex(T rl, T im)
	: real(rl)
	, imag(im)
{
}
//in-sturct operator
template <typename T> complex<T> &complex<T>::operator+=(const complex<T> &other)
{
	this->real += other.real;
	this->imag += other.imag;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator+=(const T &other)
{
	this->real += other;
	return *this;
}

template <typename T> complex<T> &complex<T>::operator-=(const complex<T> &other)
{
	this->real -= other.real;
	this->imag -= other.imag;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator-=(const T &other)
{
	this->real -= other;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator*=(const complex<T> &other)
{
	const T org_real = this->real;
	const T org_imag = this->imag;
	this->real = org_real * other.real - org_imag * other.imag;
	this->imag = org_real * other.imag + org_imag * other.real;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator*=(const T &other)
{
	this->real *= other;
	this->imag *= other;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator/=(const complex<T> &other)
{
	const T n = (other.real * other.real) + (other.imag * other.imag);
	const T r = (this->real * other.real) + (this->imag * other.imag);
	const T i = (this->imag * other.real) - (this->real * other.imag);
	this->real = r / n;
	this->imag = i / n;
	return *this;
}
template <typename T> complex<T> &complex<T>::operator/=(const T &other)
{
	this->real /= other;
	this->imag /= other;
	return *this;
}
//in sturct function
template <typename T> complex<T> complex<T>::conj() const
{
	return complex<T>(this->real, -1 * this->imag);
}
template <typename T> T complex<T>::abs() const
{
	T x = this->real;
	T y = this->imag;
	const T scalar = std::max(std::abs(x), std::abs(y));
	if (scalar == T()) { //通用的初始化值
		return scalar;
	}
	x /= scalar;
	y /= scalar;
	return scalar * std::sqrt(x * x + y * y);
}

//friend operator
//addition
template <typename T> complex<T> operator+(const complex<T> &x, const complex<T> &y)
{
	complex<T> r = x;
	r += y;
	return r;
}
template <typename T> complex<T> operator+(const complex<T> &x, const T &y)
{
	complex<T> r = x;
	r += y;
	return r;
}
template <typename T> complex<T> operator+(const T &x, const complex<T> &y)
{
	complex<T> r = y;
	r += x;
	return r;
}
//subtraction
template <typename T> complex<T> operator-(const complex<T> &x, const complex<T> &y)
{
	complex<T> r = x;
	r -= y;
	return r;
}
template <typename T> complex<T> operator-(const complex<T> &x, const T &y)
{
	complex<T> r = x;
	r -= y;
	return r;
}
template <typename T> complex<T> operator-(const T &x, const complex<T> &y)
{
	complex<T> r = y;
	r -= x;
	return r;
}
//multiplication
template <typename T> complex<T> operator*(const complex<T> &x, const complex<T> &y)
{
	complex<T> r = x;
	r *= y;
	return r;
}
template <typename T> complex<T> operator*(const complex<T> &x, const T &y)
{
	complex<T> r = x;
	r *= y;
	return r;
}
template <typename T> complex<T> operator*(const T &x, const complex<T> &y)
{
	complex<T> r = y;
	r *= x;
	return r;
}
//division
template <typename T> complex<T> operator/(const complex<T> &x, const complex<T> &y)
{
	complex<T> r = x;
	r /= y;
	return r;
}
template <typename T> complex<T> operator/(const complex<T> &x, const T &y)
{
	complex<T> r = x;
	r /= y;
	return r;
}
template <typename T> complex<T> operator/(const T &x, const complex<T> &y)
{
	complex<T> r = x;
	r /= y;
	return r;
}
//ostream
template <typename T, typename _CharT, typename _Traits>
std::basic_ostream<_CharT, _Traits> &operator<<(std::basic_ostream<_CharT, _Traits> &__os,
						const complex<T> &__x)
{
	std::basic_ostringstream<_CharT, _Traits> __s;
	__s.flags(__os.flags());
	__s.imbue(__os.getloc());
	__s.precision(__os.precision());
	__s << '(' << __x.real << ',' << __x.imag << ')';
	return __os << __s.str();
}
//istream
template <typename T> std::istream &operator>>(std::istream &is, complex<T> &z)
{
	T u = 0, v = 0;
	char ch = 0;
	bool fail = true;

	if (is >> ch) {
		if (ch == '(') {
			//(a,b) 或 (a)
			if (is >> u >> ch) {
				if (ch == ')') {
					//(a)
					z = complex<T>(u, 0);
					fail = false;
				} else if (ch == ',') {
					//(a,b)
					if (is >> v >> ch) {
						if (ch == ')') {
							z = complex<T>(u, v);
							fail = false;
						} else {
							is.putback(ch); // 字元不是 ')'
						}
					}
				} else {
					is.putback(ch);
				}
			}
		} else {
			//a
			is.putback(ch); //把剛才讀到的第一個非 ( 字元吐回串流
			if (is >> u) {
				z = complex<T>(u, 0);
				fail = false;
			}
		}
	}

	if (fail) {
		is.setstate(std::ios_base::failbit);
	}

	return is;
}

}

#endif
