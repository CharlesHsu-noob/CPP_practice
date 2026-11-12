#include <iostream>
#include <iomanip>
#include <cmath>
#include <vector>

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

#undef THEOVALUE

double g(double x)
{
	return std::exp((-1) * x * x);
}

double h(double x)
{
	return sin(1 / x);
}

double m(double x)
{
	return 5 * pow(x, 3) + 3 * x * x + 7 * x - 18;
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

bool isAccurateEnough(double L, double R, double S, double eps)
{
	return std::fabs(L + R - S) <= (15 * eps);
}

double adaptiveSimpson(double a, double b, double (*func)(double), double S,
		       std::vector<double> &partition, double eps = 1e-9)
{
	double mid = (a + b) / 2;
	double L = compositeSimpson(2, func, a, mid);
	double R = compositeSimpson(2, func, mid, b);
	if (isAccurateEnough(L, R, S, eps)) {
		return R + L + ((R + L - S) / 15.0);
	}
	//std::cout << "iterate" << std::endl;
	//not accurate enough, continue depper iteration.
	partition.push_back(mid);
	L = adaptiveSimpson(a, mid, func, L, partition, eps / 2);
	R = adaptiveSimpson(mid, b, func, R, partition, eps / 2);
	return L + R; //不需要加(L+R-S)/15 因為遞迴的子節點已經加過一次了
}

void adaptiveQuadrature()
{
	std::cout << "Extend Q2-1" << std::endl;
	double a = 0.5;
	double b = 1.5;
	std::cout << "Find the integration of sin(1/x) on [" << a << "," << b
		  << "] by using Adaptive Simpson's Rule." << std::endl;
	double originalS = compositeSimpson(2, h, a, b);
	std::vector<double> partition;
	double ans = adaptiveSimpson(a, b, h, originalS, partition);
	int min_idx = 0;
	for (int i = 0; i < partition.size(); i++) {
		min_idx = i;
		for (int j = i + 1; j < partition.size(); j++) {
			if (partition[j] < partition[min_idx]) {
				min_idx = j;
			}
		}
		std::swap(partition[i], partition[min_idx]);
	}
	std::cout << "Find the answer= " << std::setprecision(16) << ans << std::endl;
	std::cout << "Total number of adaptive nodes recorded: " << partition.size() << std::endl;
	std::cout << "Partition:" << std::endl;
	for (int i = 0; i < partition.size(); i += 8) {
		for (int j = i; j < std::min(i + 8, (int)partition.size()); j++) {
			std::cout << std::setprecision(6) << std::setw(10) << partition[j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Subinterval widths (dx_i = x_i - x_{i-1}, showing local step size density):"
		  << std::endl;
	for (int i = 1; i < partition.size(); i += 8) {
		for (int j = i; j < std::min(i + 8, (int)partition.size()); j++) {
			std::cout << std::setprecision(6) << std::setw(10)
				  << partition[j] - partition[j - 1];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout
		<< "The subinterval widths are much smaller near the left endpoint (where sin(1/x) oscillates more rapidly)"
		   "and become larger toward the right endpoint (where the function is smooth), "
		   "show that nodes automatically concentrate in rapidly oscillating regions."
		<< std::endl;
}

//node=2
double gaussianQuadrature(double a, double b, double (*func)(double))
{
	//mapping [a,b] to [-1,1]
	//x=(b-a)*t/2+(b+a)/2
	//t=1/root(3)
	double h = (b - a) / 2.0; //mapping_ratio
	double mid = (b + a) / 2.0;
	double inv_sqrt3 = 0.5773502691896257645;
	double h_sqrt3 = h * inv_sqrt3;
	return h * (func(mid - h_sqrt3) + func(mid + h_sqrt3));
}

void extendQ2_2()
{
	std::cout << "Extend Q2-2" << std::endl;
	double a = 1;
	double b = 10;
	std::cout << "Gaussian Quadrature of m(x)=5x^3+3x^2+7x-18 on [" << a << "," << b << "]."
		  << std::endl;
	double ans = gaussianQuadrature(a, b, m);
	std::cout << "Find ans= " << std::setprecision(9) << ans << std::endl;
	std::cout << "Exact analytical value: 13682.25" << std::endl;
	std::cout << "\nEfficiency Comparison:" << std::endl;
	std::cout
		<< "1. 2-Point Gaussian Quadrature only requires 2 function evaluations to achieve the exact result (0 error) for a cubic polynomial."
		   "\n2. In contrast, Composite Trapezoidal or Midpoint rules would require hundreds of nodes (h -> 0) to reach similar precision."
		   "\n3. This demonstrates the superior efficiency of Gaussian Quadrature for smooth polynomial functions by optimally placing nodes rather than using equidistant points."
		<< std::endl;
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
	numericalDifferentiation();
	numericalIntagration();
	adaptiveQuadrature(); //Extend Q2-1
	extendQ2_2();
	return 0;
}
