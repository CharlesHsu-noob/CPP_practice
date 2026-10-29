
#ifndef MY_COMPLEX_H
#define MY_COMPLEX_H

#include <iostream>
#include <cmath>
#include <sstream>
#include <algorithm>
namespace xdd
{
template <typename T> struct complex {
	T real;
	T imag;

	complex(T rl = 0, T im = 0);
	complex<T> &operator+=(const complex<T> &other);
	complex<T> &operator+=(const T &other);
	complex<T> &operator-=(const complex<T> &other);
	complex<T> &operator-=(const T &other);
	complex<T> &operator*=(const complex<T> &other);
	complex<T> &operator*=(const T &other);
	complex<T> &operator/=(const complex<T> &other);
	complex<T> &operator/=(const T &other);

	complex<T> conj() const;
	T abs() const;
};

//external operator
template <typename T> complex<T> operator+(const complex<T> &x, const complex<T> &y);
template <typename T> complex<T> operator+(const complex<T> &x, const T &y);
template <typename T> complex<T> operator+(const T &x, const complex<T> &y);

template <typename T> complex<T> operator-(const complex<T> &x, const complex<T> &y);
template <typename T> complex<T> operator-(const complex<T> &x, const T &y);
template <typename T> complex<T> operator-(const T &x, const complex<T> &y);

template <typename T> complex<T> operator*(const complex<T> &x, const complex<T> &y);
template <typename T> complex<T> operator*(const complex<T> &x, const T &y);
template <typename T> complex<T> operator*(const T &x, const complex<T> &y);

template <typename T> complex<T> operator/(const complex<T> &x, const complex<T> &y);
template <typename T> complex<T> operator/(const complex<T> &x, const T &y);
template <typename T> complex<T> operator/(const T &x, const complex<T> &y);

//iostream
template <typename T, typename _CharT, class _Traits>
std::basic_ostream<_CharT, _Traits> &operator<<(std::basic_ostream<_CharT, _Traits> &__os,
						const complex<T> &__x);
template <typename T> std::istream &operator>>(std::istream &is, complex<T> &z);
}

#include "my_complex.hpp"

#endif
