#include <cmath>
#include <cstdio> //printf("%.n f");
#include <iostream>
#include <limits>  //Mechine Epsilon
//#include <utility> //For pair<T1,T2>,already included in <iostream>
using namespace std;

//Q1:Machine Epsilon

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

int main() {
  MachineEpsilon();
  extendQ1_1();
  //cout << "Fuck You Microsoft." << endl;
  return 0;
}
