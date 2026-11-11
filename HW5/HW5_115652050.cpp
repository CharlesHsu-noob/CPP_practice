#include <iostream>
#include <iomanip>
#include <cmath>

#define THEOVALUE 0.54030230586

double f(double x)
{
	return std::sin(x);
}

double forwardDiff(double x, double h, double (*func)(double))
{
	return (func(x + h) - func(x)) / h;
}

double backwardDiff(double x, double h, double (*func)(double))
{
	return (func(x) - func(x - h)) / h;
}

double centralDiff(double x, double h, double (*func)(double))
{
	return (func(x + h) - func(x - h)) / (2 * h);
}

double richardsonExtra(double x, double h, double (*func)(double))
{
	return (8.0 * (func(x + h) - func(x - h)) - func(x + 2 * h) + func(x - 2 * h)) / (12 * h);
}

double secondCentralDiff(double x, double h, double (*func)(double))
{
	return (func(x + h) - 2 * func(x) + func(x - h)) / (h * h);
}

void numericalDifferentiation()
{
	double result[3][15];
	double initx = 1.0;
	double h = 1.0;

	for (int i = 0; i < 15; i++) {
		result[0][i] = forwardDiff(initx, h, f);
		result[1][i] = backwardDiff(initx, h, f);
		result[2][i] = centralDiff(initx, h, f);
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
	std::cout << "Extend Q1-1" << std::endl;
	std::cout
		<< "the theoretical value of best step length of central differentiation is (machine_epsilon)^(1/3) =10^(-5.33), "
		<< "and the best step length found by code is " << best_step[2]
		<< ".It's the same order of magnitude." << std::endl;

	//extend Q2
	std::cout << "Extend Q1-2" << std::endl;
	h = 1.0;
	double theoValue_second = (-1) * 0.8414709848; //-sin(1)
	std::cout << "Second differentiation error:" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << std::fabs(secondCentralDiff(initx, h, f) - theoValue_second)
			  << std::endl;
		h /= 10.0;
	}

	//extend Q3
	std::cout << "Extend Q1-3" << std::endl;
	h = 1.0;
	std::cout << "Richardson extrapolation(step:h, h/2)" << std::endl;
	std::cout << std::setw(6) << "h" << std::setw(12) << "error" << std::endl;
	for (int i = 0; i < 15; i++) {
		std::cout << std::setw(6) << h << std::setw(12)
			  << std::fabs(richardsonExtra(initx, h, f) - THEOVALUE) << std::endl;
		h /= 10.0;
	}
	std::cout << "Get error of 1e-12 order at h=1e-3, "
		     "which is bigger than the best step h in central differentiation."
		  << std::endl;
}

double g(double x)
{
	return std::exp((-1) * x * x);
}

double compositeTrapezoidal(int n, double (*func)(double), double a = 0.0, double b = 1.0)
{
	double delta = (b - a) / (1.0 * n);
	double ans = func(a) + func(b);
	ans /= 2;
	for (int i = 1; i < n; i++) {
		ans += func(a + i * delta);
	}
	ans *= delta;
	return ans;
}

double compositeMidpoint(int n, double (*func)(double), double a = 0.0, double b = 1.0)
{
	double delta = (b - a) / (1.0 * n);
	a -= 0.5 * delta;
	double ans = 0;
	for (int i = 1; i < (n + 1); i++) {
		ans += func(a + i * delta);
	}
	ans *= delta;
	return ans;
}

double compositeSimpson(int n, double (*func)(double), double a = 0.0, double b = 1.0)
{
	n = (n % 2 == 0) ? n : (n + 1); //prove that n is even number
	double delta = (b - a) / (1.0 * n);
	double ans = 0;

	//n/2->*4   (n/2)-1->*2
	for (int i = 1; i < n; i += 2) { //奇數項
		ans += func(a + i * delta);
	}
	ans *= 2;
	for (int i = 2; i < n; i += 2) { //偶數項
		ans += func(a + i * delta);
	}
	ans *= 2;

	ans += func(a);
	ans += func(b);

	ans /= 3;
	ans *= delta;
	return ans;
}

#define THEOVALUE 0.7468241328124270

void numericalIntagration()
{
	double result[3][10];
	int n = 1;
	for (int i = 0; i < 10; i++) {
		n *= 2;
		result[0][i] = compositeTrapezoidal(n, g);
		result[1][i] = compositeMidpoint(n, g);
		result[2][i] = compositeSimpson(n, g);
	}

	//output
	//h, error of Trapezoidal, error of Midpoint, error of Simpson
	std::cout << "Numerical integration" << std::endl;
	std::cout << "n\terror of Trapezoidal\terror of Midpoint\terror of Simpson" << std::endl;
	n = 1;
	double error[3][10];
	for (int i = 0; i < 10; i++) {
		n *= 2;
		for (int j = 0; j < 3; j++) {
			error[j][i] = std::fabs(result[j][i] - THEOVALUE);
		}
		std::cout << std::setw(6) << n << std::setw(18) << error[0][i] << std::setw(25)
			  << error[1][i] << std::setw(23) << error[2][i] << std::endl;
	}
	std::cout << std::endl;

	double ratio[3][9];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			ratio[i][j] = error[i][j] / error[i][j + 1];
		}
	}
	std::cout << "Relative ratio of error" << std::endl;
	std::cout << "Trapezoidal" << std::setw(14) << "Midpoint" << std::setw(14) << "Simpson"
		  << std::endl;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 3; j++) {
			std::cout << std::setw(14) << ratio[j][i];
		}
		std::cout << std::endl;
	}
}

int main()
{
	//numericalDifferentiation();
	numericalIntagration();
	return 0;
}
