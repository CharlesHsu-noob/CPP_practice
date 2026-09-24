#include <cmath>
#include <cstdio> //printf("%.n f");
#include <iostream>
#include <limits>  //Mechine Epsilon
#include <utility> //For pair<T1,T2>
using namespace std;

float machine_eps_flt() {
  float eps = 1.0f;
  while (1.0f + (eps / 2.0f) > 1.0f) {
    eps /= 2.0f;
  }
  return eps;
}

double machine_eps_dub() {
  double eps = 1.0;
  while (1 + (eps / 2) > 1.0) {
    eps /= 2.0;
  }
  return eps;
}

void extendQ1_1() {
  double eps = numeric_limits<double>::epsilon() / 2;
  double a = (1.0 + eps) + eps;
  double b = 1.0 + (eps + eps);
  cout << "----Extend Question 1-1.----" << endl;
  cout << "(1.0+eps)+eps=";
  printf("%.20f\n", a);
  cout << "1.0+(eps+eps)=";
  printf("%.20f\n", b);
  cout << "Is a equals to b? No. b>a=1." << endl;
  cout << "Does this contradict the associative property of addition?\n"
          "Yes, it does. Mathematically, (a+b)+c = a+(b+c), while in "
          "floating-point calculations, due to Machine Epsilon, the order of "
          "addition will affect the result. Hence, floating-point addition "
          "does not satisfy the associative property."
       << endl;
}

void MachineEpsilon() {
  float flt_eps = machine_eps_flt();
  double dub_eps = machine_eps_dub();
  cout << "----Question 1:Machine Epsilon.----" << endl;
  cout << "Machine Epsilon calculated manually:\n"
       << "Float:" << flt_eps << "\tDouble:" << dub_eps << endl;
  float limit_eps_flt = numeric_limits<float>::epsilon();
  double limit_eps_dub = numeric_limits<double>::epsilon();
  cout << "Nachine Epsilon from limits.h:\n"
       << "Float:" << limit_eps_flt << "\tDouble:" << limit_eps_dub << endl;
  cout << "The machine epsilon defined in <limit> matches the value calculated "
          "manually."
       << endl;
}

// Bisection Method residual:1e-8
// Newton's Method residual:1e-11

double f(double x) { return pow(x, 2) - 4 * sin(x); }

double *getBisecInput() {
  double *usrin = new double[3];
  cout
      << "Give the boundary and the target.\n"
      << "a:left boundary\tb:right boundary  target:the value you want to find."
      << endl;
  cin >> usrin[0] >> usrin[1] >> usrin[2];
  return usrin;
}

pair<double, int> bisectionMethod(double a, double b, double target) {
  // a is the left boundary and b is the right boundary.
  double fa_diff = f(a) - target;
  double fb_diff = f(b) - target;
  if (fa_diff * fb_diff > 0) {
    cout << "ERROR:Wrong interval range or invalid value.(f(a)*f(b) should be "
            "negative.)"
         << endl;
    return {NAN, 0};
  } else if (fa_diff == 0) {
    return {a, 0};
  } else if (fb_diff == 0) {
    return {b, 0};
  }

  const double rsdl = 1e-8; // rsdl->residual
  const int max_iter = 100; // iter->iterations迭代
  double ans = (a + b) / 2.0;
  double fans_diff = f(ans) - target;
  int i = 0;
  while (abs(fans_diff) > rsdl && i < max_iter) {
    ans = (a + b) / 2.0;
    fans_diff = f(ans) - target;
    if (abs(fans_diff) <= rsdl) {
      break;
    }
    if (fa_diff * fans_diff > 0) {
      a = ans;
      fa_diff = fans_diff;
    } else {
      b = ans;
      fb_diff = fans_diff; // 可省略
                           // 判斷式只會用到fa_diff,這個變數永遠不會被調用
    }
    i++;
  }
  return {ans, i};
}

void FindRoot() {
  int method = 0;
  cout << "----Find root.----\nChoose a method.\n1:Bisection method.\n2:Secant "
          "method.\n3:Newton's method."
       << endl;
  cin >> method;
  switch (method) {
  case 1: {
    cout << "Use bisection method." << endl;
    double *usrin = getBisecInput();
    pair<double, int> ans = bisectionMethod(usrin[0], usrin[1], usrin[2]);
    if (isnan(ans.first)) {
      delete[] usrin;
      break;
    }
    printf("Find answer: %.8f in %i steps.\n", ans.first, ans.second);
    delete[] usrin;
    break;
  }
  case 2: {
    cout << "Use secant method." << endl;
    break;
  }
  case 3: {
    cout << "Use Newton's method." << endl;
    break;
  }
  default: {
    cout << "Invalid method." << endl;
    break;
  }
  }
}

int main() {
  // MachineEpsilon();
  // extendQ1_1();
  FindRoot();
  cout << "Fuck You Microsoft." << endl;
  return 0;
}
