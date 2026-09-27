#include <iostream>
#include<cmath>

using namespace std;

//Q2:Find root in different methods

// Bisection Method residual:1e-11
// Secant Method residual:1e-11
// Newton's Method residual:1e-11
//printf("&e",residual) to print the residual in scientific notation.
const double RESIDUAL=1e-11;

double f(double x) { return pow(x, 2) - 4 * sin(x); }

double df(double x) {return 2*x-4*cos(x);}

double *getABTInput() {
  /** 
  *  @brief user input for left boundary, right boundary, and target value.
  *  @param A:Left boundary
  *  @param B:Right boundary
  *  @param T:Target value
  *  @return double pointer to an array of size 3, containing A, B, and T.
  */
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

  const double rsdl = RESIDUAL; // rsdl->residual
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

pair<double,int> secantMethod(double a,double b,double target){
  double fa_diff = f(a) - target;
  double fb_diff = f(b) - target;

  if(fa_diff==0){
    return {a,0};
  }
  else if (fb_diff==0){
    return {b,0};
  }

  const double rsdl=RESIDUAL;
  const int max_iter=100;
  int i=0;
  double ans=b;
  double fans_diff=fb_diff;
  double m=0;
  while(abs(fans_diff)>rsdl && i<max_iter){
    if(fa_diff==fb_diff){
      cout<<"ERROR: f(a) and f(b) should not be equal."<<endl;
      return {NAN,0};
    }
    //main calculation
    m=(fb_diff-fa_diff)/(b-a);
    ans=b-(fb_diff/m);
    
    a=b;
    fa_diff=fb_diff;
    b=ans;
    fb_diff=f(b)-target;
    fans_diff=fb_diff;
    i++;
  }
  return {ans,i};
}

pair<double,int> newtonsMethod(double init,double target){
  double finit_diff=f(init)-target;
  if(finit_diff==0){return {init,0};}
  double rsdl=RESIDUAL;
  int max_iter=100;
  int i=0;
  double ans=init;
  double fans_diff=finit_diff;
  double df_ans=df(ans);
  while(abs(fans_diff)>rsdl && i<max_iter){
    if(df_ans==0){
      cout<<"ERROR: derivative is zero."<<endl;
      return {NAN,0};
    }
    ans=ans-(fans_diff/df_ans);
    fans_diff=f(ans)-target;
    df_ans=df(ans);
    i++;
  }
  return {ans, i};
}

double residual(double x,double target,double (*func)(double)){
  //double (*func)(double) is a function pointer, which points to a function that takes a double and returns a double.
  //first double is the return type of "func",second double is the parameter type of "func".
  return abs(func(x)-target);
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
    double *usrin = getABTInput();
    pair<double, int> ans = bisectionMethod(usrin[0], usrin[1], usrin[2]);
    if (isnan(ans.first)) {
      delete[] usrin;
      break;
    }
    double res=residual(ans.first,usrin[2],f);
    printf("Find answer: %.8f in %i steps.\n", ans.first, ans.second);
    printf("Residual: %.8e\n",res);
    delete[] usrin;
    break;
  }
  case 2: {
    cout << "Use secant method." << endl;
    double *usrin = getABTInput();
    pair<double,int> ans=secantMethod(usrin[0],usrin[1],usrin[2]);
    if(isnan(ans.first)){
      delete[] usrin;
      break;
    }
    double res=residual(ans.first,usrin[2],f);
    printf("Find answer: %.11f in %i steps.\n", ans.first, ans.second);
    printf("Residual: %.8e\n",res);
    delete[] usrin;
    break;
  }
  case 3: {
    cout << "Use Newton's method." << endl;
    double init,target;
    cout<<"Give the initial value and the target value."<<endl;
    cin>>init>>target;
    pair<double,int> ans=newtonsMethod(init,target);
    if(isnan(ans.first)){
      break;
    }
    if(ans.second==100){
      cout<<"Warning: Method didn't converge in 100 steps."<<endl;
    }
    double res=residual(ans.first,target,f);
    printf("Find answer: %.11f in %i steps.\n", ans.first, ans.second);
    printf("Residual: %.8e\n",res);
    break;
  }
  default: {
    cout << "Invalid method." << endl;
    break;
  }
  }
}

int main(){
    FindRoot();
    return 0;
}