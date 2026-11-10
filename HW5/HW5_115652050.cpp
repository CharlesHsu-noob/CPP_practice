#include <iostream>
#include <iomanip>
#include <cmath>

#define THEOVALUE 0.54030230586

double f(double x)
{
	return std::sin(x);
}

double forwardDiff(double x, double h)
{
	return (f(x + h) - f(x)) / h;
}

double backwardDiff(double x, double h)
{
	return (f(x) - f(x - h)) / h;
}

double centralDiff(double x, double h)
{
	return (f(x + h) - f(x - h)) / (2 * h);
}

double secondCentralDiff(double x, double h)
{
	return (f(x + h) - 2 * f(x) + f(x - h)) / (h * h);
}

void numericalDifferentiation()
{
	double result[3][15];
	double initx = 1.0;
	double h = 1.0;

	for (int i = 0; i < 15; i++) {
		result[0][i] = forwardDiff(initx, h);
		result[1][i] = backwardDiff(initx, h);
		result[2][i] = centralDiff(initx, h);
		h /= 10;
	}
	//output:
	//h, error of forward, error of backward, error of central
	std::cout << "Numerical differentiation" << std::endl;
	std::cout << "h\terror of forward\terror of backward\terror of central" << std::endl;
	h = 1.0;
	for (int i = 0; i < 15; i++) {
		std::cout << std::setw(6) << h << std::setw(18)
			  << std::fabs(result[0][i] - THEOVALUE) << std::setw(25)
			  << std::fabs(result[1][i] - THEOVALUE) << std::setw(23)
			  << std::fabs(result[2][i] - THEOVALUE) << std::endl;
		h /= 10.0;
	}
	std::cout << "the best step h:" << std::endl;
	int min_idx = 0;
	double min_err = 0;
	double error = 0;
	double best_step[3];
	for (int i = 0; i < 3; i++) {
		min_idx = 0;
		min_err = std::fabs(result[i][0] - THEOVALUE);
		for (int j = 0; j < 15; j++) {
			error = std::fabs(result[i][j] - THEOVALUE);
			if (error < min_err) {
				min_idx = j;
				min_err = error;
			}
		}
		std::cout << std::setw(19 + i * 3) << "1e-" << min_idx;
		best_step[i] = pow(10, min_idx * (-1));
		min_idx = 0;
	}
	std::cout << std::endl;

	//extend Q1
	std::cout
		<< "the theoretical value of best step length of central differentiation is (machine_epsilon)^(1/3) =10^(-5.33), "
		<< "and the best step length found by code is " << best_step[2]
		<< ".It's the same order of magnitude." << std::endl;

	//extend Q2
	h = 1.0;
	double theoValue_second = (-1) * 0.8414709848; //-sin(1)
	std::cout << "Second differentiation error:" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << std::fabs(secondCentralDiff(initx, h) - theoValue_second) << std::endl;
		h /= 10.0;
	}
}

int main()
{
	numericalDifferentiation();
	return 0;
}
