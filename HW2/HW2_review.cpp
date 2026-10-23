#include <cmath>
#include <iostream>

using namespace std;

const double RESIDUAL = 1e-11;
const double ERROR = 1e-8;

double g(double x){
	double res=1.0;
	for(int i=0;i<10;i++){
		res*=x;
	}
	return res;
}

double dg(double x)
{
	double res=1.0;
	for(int i=0;i<9;i++){
		res*=x;
	}
	return 10*res;
}

bool checkResidual(double gx)
{
	return std::fabs(gx) < RESIDUAL;
}

bool checkAbsError(double currentX, double preX) {
    return std::fabs(currentX - preX) < ERROR;
}

bool converge(double current_x, double prev_x) {
    if (checkAbsError(current_x, prev_x))
        return true;
    return false;
}

pair<double, int> newtonsMethod(double init, double target)
{
	double finit_diff = g(init) - target;
	if (finit_diff == 0) {
		return { init, 0 };
	}
	int max_iter = 100;
	int i = 0;
	double ans = init;
    double prev_ans=ans;
	double fans_diff = finit_diff;
	double df_ans = dg(ans);
	if (checkResidual(fans_diff)) {
		return { ans, i };
	}
	while (i < max_iter) {
		if (df_ans == 0) {
			cout << "ERROR: derivative is zero." << endl;
			return { NAN, 0 };
		}
        prev_ans=ans;
		ans = ans - (fans_diff / df_ans);
		fans_diff = g(ans) - target;
		df_ans = dg(ans);
		if (converge(ans,prev_ans)) {
			break;
		}
		i++;
	}
	return { ans, i };
}

double residual(double x, double target, double (*func)(double))
{
	// double (*func)(double) is a function pointer, which points to a function
	// that takes a double and returns a double. first double is the return type
	// of "func",second double is the parameter type of "func".
	return std::fabs(func(x) - target);
}

void FindRoot()
{
	cout << "Newton's method." << endl;
	double init, target;
	cout << "Give the initial value and the target value(target:g(ans)=target)." << endl;
	cin >> init >> target;
	pair<double, int> ans = newtonsMethod(init, target);
	if (isnan(ans.first)) {
		cout<<"NAN"<<endl;
	}
	if (ans.second == 100) {
		cout << "Warning: Method didn't converge in 100 steps." << endl;
	}
	double res = residual(ans.first, target, g);
	printf("Find answer: %.11f in %i steps.\n", ans.first, ans.second);
	printf("Residual: %.8e\n", res);
}

int main()
{
	FindRoot();
	return 0;
}