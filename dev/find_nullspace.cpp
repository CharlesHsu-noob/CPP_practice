#include <iostream>
#include <tuple>
#include "my_vector.hpp"

using namespace std;

//include from CR_decompose.cpp
pair<xdd::matrix<double>, xdd::matrix<double> >
CR_decompose(xdd::matrix<double> &A);
//end include

xdd::vector<xdd::vector<double> > nullspace(xdd::matrix<double> &A)
{
	xdd::matrix<double> C, R;
	std::tie(C, R) = CR_decompose(A);
}

int main()
{
	return 0;
}