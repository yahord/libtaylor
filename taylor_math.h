
// By Ulf Ekstrom March-June 2009.
// Elementary functions. For inclusion in taylor.h only!

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

//Taylor series of 1/(a+x)
template<class T,int N>
void inv_taylor(taylor<T,1,N>& t, const T &a)
{
  assert(a != T(0) && "1/(a+x) not analytic at a = 0");
  t[0] = 1/a;
  for (int i=1;i<=N;i++)
    t[i] = -t[i-1]/a;
}

template<class T,int Nvar, int Ndeg, class S>
taylor<T,Nvar, Ndeg> operator/(const S &x, const taylor<T,Nvar,Ndeg>& t)
{
  taylor<T,1,Ndeg> tmp;
  inv_taylor(tmp,t[0]);
  tmp*=x;
  taylor<T,Nvar, Ndeg> res;
  t.compose(res,tmp);
  return res;
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar, Ndeg> operator/(const taylor<T,Nvar,Ndeg>& t, const T &x)
{
  taylor<T,Nvar,Ndeg> tmp = t;
  tmp *= 1/x;
  return tmp;
}

template<class T,int Nvar, int Ndeg, class S>
taylor<T,Nvar, Ndeg> operator/(const taylor<T,Nvar,Ndeg>& t, const S &x)
{
  taylor<T,Nvar,Ndeg> tmp = t;
  tmp *= 1/T(x);
  return tmp;
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar, Ndeg> operator/(const taylor<T,Nvar,Ndeg>&t1, const taylor<T,Nvar,Ndeg>& t2)
{
  taylor<T,1,Ndeg> tmp;
  inv_taylor(tmp,t2[0]);
  taylor<T,Nvar, Ndeg> res;
  t2.compose(res,tmp);
  res*=t1;
  return res;
}

// Evaluate the taylor series of exp(x0+x)=exp(x0)*exp(x)
template<class T,int Ndeg>
void exp_taylor(taylor<T,1,Ndeg> &t, const T &x0)
{
  T ifac = 1;
  t[0] = exp(x0);
  for (int i=1;i<=Ndeg;i++)
    {
      ifac *= i;
      t[i] = t[0]/ifac;
    }
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> exp(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  exp_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}

// Log series log(a+x) = log(1+x/a) + log(a)
template<class T,int N>
void log_taylor(taylor<T,1,N> &t, const T &x0)
{
  assert(x0 != T(0) && "log(x) not analytic at x = 0");
  t[0] = log(x0);
  T xn = x0;
  for (int i=1;i<=N;i++)
    {
      t[i] = (2*(i & 1)-1)/(i*xn);
      xn *= x0;
    }
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> log(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  log_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}

/* Use that (x0+x)^a=x0^a*(1+x/x0)^a */
template<class S,class T,int N>
void pow_taylor(taylor<T,1,N>& t, const T &x0, const S &a)
{
  assert(x0 != T(0) && "pow(x,a) not analytic at x = 0");
  t[0] = pow(x0,a);
  for (int i=1;i<=N;i++)
    t[i] = t[i-1]*(a-i+1)/(x0*i);
}

template<class T,int Nvar, int Ndeg, class S>
taylor<T,Nvar,Ndeg> pow(const taylor<T,Nvar,Ndeg> &t, const S &a)
{
  taylor<T,1,Ndeg> tmp;
  pow_taylor(tmp,t[0],T(a));

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> sqrt(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  pow_taylor(tmp,t[0],0.5);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}

// Integer exponent version is analytical at t[0] = 0
// This function gets priority over the normal pow 
// when the exponent is an integer, but does not force
// conversion to integer.
template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> pow(const taylor<T,Nvar,Ndeg> &t, int n)
{
  if (n < 1)
    return pow(t,T(n));
  taylor<T,Nvar,Ndeg> res = t;
  while (n-- > 1)
      res *= t;
  return res;
}

// Use that d/dx atan(x) = 1/(1 + x^2),
// Taylor expand in x^2 and integrate.
template<class T,int Ndeg>
void atan_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
  // Calculate taylor expansion of 1/(1+a^2+x)
  taylor<T,1,Ndeg> invt,x;
  inv_taylor(invt,1+a*a);
  //insert x = 2*a*x + x^2
  x = taylor<T,1,Ndeg>(0);
  if (Ndeg > 0)
    x[1] = 2*a;
  if (Ndeg > 1)
    x[2] = 1;
  x.compose(t,invt);
  //Integrate each term and set the constant
  t.integrate();
  t[0] = atan(a);
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> atan(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  atan_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}


/* 
   Taylor expansion of exp(-(a+x)^2) =
   exp(-a^2-2a*x)*exp(-x^2)
 */
template<class T,int Ndeg>
void gauss_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
  exp_taylor(t,-a*a);
  t.stretch(-2*a);
  taylor<T,1,Ndeg> g;
  g = 1;
  for (int i=1;i<=Ndeg/2;i++)
    g[2*i] = -g[2*(i-1)]/i;
  t*=g;
}

// Use that d/dx erf(x) = 2/sqrt(pi)*exp(-x^2),
// Taylor expand in x^2 and integrate.
template<class T,int Ndeg>
void erf_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
  gauss_taylor(t,a);
  t*=2/sqrt(M_PI);
  //Integrate each term and set the constant
  t.integrate();
  t[0] = erf(a);
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> erf(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  erf_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}


template<class T,int Ndeg>
void sin_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
    if (Ndeg > 0)
    {
	T s = sin(a), c = cos(a), fac = 1;
	for (int i=0;2*i<Ndeg;i++)
	{
	    t[2*i] = fac*s;
	    fac /= (2*i+1);
	    t[2*i+1] = fac*c;
	    fac /= -(2*i+2);
	}
	if (Ndeg % 2 == 0)
	    t[Ndeg] = s*fac;
    }
    else
    {
	t[0] = sin(a);
    }   
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> sin(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  sin_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}


template<class T,int Ndeg>
void cos_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
    if (Ndeg > 0)
    {
	T s = sin(a), c = cos(a), fac = 1;
	for (int i=0;2*i<Ndeg;i++)
	{
	    t[2*i] = fac*c;
	    fac /= -(2*i+1);
	    t[2*i+1] = fac*s;
	    fac /= (2*i+2);
	}
	if (Ndeg % 2 == 0)
	    t[Ndeg] = c*fac;
    }
    else
    {
	t[0] = cos(a);
    }   
}


template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> cos(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  cos_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}


// hyperbolic arcsin function. d/dx asinh(x) = 1/sqrt(1+x^2)
// 1 + (a+x)^2 = 1+a^2 + 2ax + x^2
template<class T,int Ndeg>
void asinh_taylor(taylor<T,1,Ndeg>& t, const T &a)
{
  taylor<T,1,Ndeg> tmp(1+a*a);
  if (Ndeg>0)
    tmp[1] = 2*a;
  if (Ndeg>1)
    tmp[2] = 1;
  t = pow(tmp,-0.5);
  t.integrate();
  t[0] = asinh(a);
}

template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> asinh(const taylor<T,Nvar,Ndeg> &t)
{
  taylor<T,1,Ndeg> tmp;
  asinh_taylor(tmp,t[0]);

  taylor<T,Nvar,Ndeg> res;
  t.compose(res,tmp);
  return res;
}


template<class T,int Ndeg>
void sinc_taylor_at0(taylor<T,1,Ndeg>& t)
{
    t[0] = 1;
    T fac = 1;
    for (int i=1;i<=Ndeg/2;i++)
    {
	fac /= (2*i);
	t[2*i-1] = 0;
	fac /= -(2*i+1);
	t[2*i] = fac;
    }
    if (Ndeg % 2 == 1)
	t[Ndeg] = 0;
}


template<class T,int Nvar, int Ndeg>
taylor<T,Nvar,Ndeg> sinc(const taylor<T,Nvar,Ndeg> &t)
{
    if (fabs(t[0]) < 1e-3)
    {
	taylor<T,1,Ndeg+8> tmp;
	sinc_taylor_at0(tmp);
	taylor<T,1,Ndeg> tmp2;	
	T dx = t[0];
	tmp.shift(tmp2,&dx);
	taylor<T,Nvar,Ndeg> res;
	t.compose(res,tmp2);
	return res;
    }
    else
    {
	return sin(t)/t;
    }
}

