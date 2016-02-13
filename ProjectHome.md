Libtaylor is a C++ template implementation of truncated multivariate Taylor series arithmetics. Besides the basic arithmetic operations the library also implements most standard C mathematical functions, on Taylor series. It is for example possible to evaluate the numerical Taylor coefficients of `sin(2+x)`, which gives the Taylor expansion of the `sin` function at 2. This makes libtaylor a complete solution for arbitrary order automatic differentiation in C++.

Example - Taylor expanding `sqrt(x^2 + y^2)` around `(2,3)`, to third order:

```
template<typename T>
T dist(T x, T y)
{
   return sqrt(x*x + y*y);
}

void expand_it()
{
   taylor<double, 2, 3> dx(0,0), dy(0,1); //Set up two infinitesimal variables
   cout << "The taylor expansion is" << dist(2+dx,3+dy) << endl;
}

```

Due to caching, inlining and other effects the library performs best at certain sweet spots. In this region the computation cost _per output coefficient_ is almost constants. The sweet region for a "typical" application using some math functions are

| Variables | Degree |
|:----------|:-------|
| 1         |   <= 10 |
| 2         |   <= 7  |
| 3         |   <= 4-5 |
| 4         |   <= 4   |
| 5-9       |   <= 3   |
| 10-?      |   <= 2   |

Note that libtaylor can be used outside these ranges, just with lower performance per coefficient, due to the quadratic scaling of the polynomial multiplication algorithm used.

Libtaylor is built on the [polymul](http://polymul.googlecode.com) template library. It is currently at a alpha/beta stage, and needs more real world testing. It is licensed under the LGPL license, but may require special care since libtaylor is a template library and cannot be linked dynamically.

If you use this library in a scientific publication please cite "Libtaylor, Ulf Ekström 2009". For alternative licensing please contact [Ulf Ekström](http://admol.org/uekstrom).