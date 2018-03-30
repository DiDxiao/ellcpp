#ifndef _HOME_UBUNTU_CUBSTORE_ELLCPP_CHOL_EXT_HPP
#define _HOME_UBUNTU_CUBSTORE_ELLCPP_CHOL_EXT_HPP 1

// #include <boost/numeric/ublas/symmetric.hpp>
// #include <boost/numeric/ublas/triangular.hpp>
// #include <cassert>

// namespace bnu = boost::numeric::ublas;

#include <cassert>
#include <xtensor/xarray.hpp>
#include <xtensor-blas/xlinalg.hpp>

#include <iostream>

/**
 * @brief Cholesky factorization
 * 
 */
class chol_ext {
  // using Mat = bnu::symmetric_matrix<double, bnu::upper>;
  // using UTMat = bnu::triangular_matrix<double, bnu::upper>;
  // using Vec = bnu::vector<double>;
  using Vec = xt::xarray<double>;
  using Mat = xt::xarray<double>;
  using shape_type = Vec::shape_type;

private:
  size_t _p;
  size_t _n;
  xt::xarray<double> _R;

public:
  /**
   If $A$ is positive definite, then $p$ is zero.
   If it is not, then $p$ is a positive integer,
   such that $v = R^{-1} e_p$ is a certificate vector
   to make $v'*A[:p,:p]*v < 0$
  **/
  explicit chol_ext(const Mat &A)
    : _p{0} {
    auto shape = A.shape();
    _n = shape[0];
    _R = xt::zeros<double>(shape);
    
    for (auto i = 0u; i < _n; ++i) {
      for (auto j = 0u; j <= i; ++j) {
        double d = A(j, i);
        for (auto k = 0u; k < j; ++k) {
          d -= _R(k, i) * _R(k, j);
        }
        if (i == j) {
          if (d < 0.) {
            _p = i + 1;
            _R(j, i) = std::sqrt(-d);
            return;
          }
          else {
            _R(j, i) = std::sqrt(d);
          }
        } else {
          _R(j, i) = 1.0 / _R(j, j) * d;
        }
      }
    }
  }

  bool is_sd() const { return _p == 0; }

  auto witness() const {
    assert(!this->is_sd());
    Vec v = xt::zeros<double>({_p});
    using xt::placeholders::_;
    
    v[_p - 1] = 1.0 / _R(_p - 1, _p - 1);
    for (int i = _p - 2; i >= 0; --i) {
      double s = 0.0;
      for (auto k = i + 1; k < _p; ++k) {
        s += _R(i, k) * v[k];
      }
      // double s = xt::linalg::dot(xt::view(_R, i, xt::range(i+1, _p)),
      //                    xt::view(v, xt::range(i+1, _p)))();
      v[i] = -s / _R(i, i);
    }
    return v;
  }
};

/**
 * Constructs a upper triangular matrix R, such that R'*R= A.
 * If A is not symmetric positive-definite (SPD), only a partial
 * factorization is performed. If isspd() evalutate true then
 * the factorizaiton was successful.

chol_ext::chol_ext(const chol_ext::Mat& A )
    : _p{0}, _n{A.size1()}, _R(_n, _n) {
    auto sq = [](auto a) { return a*a; }; // square

    for (; _p<_n; ++_p) {
        auto d = A(_p,_p);
        for (auto k=0u; k<_p; ++k ) {
            auto s = A(k,_p);
            for (auto i=0u; i<k; ++i ) {
                s -= _R(i,k)*_R(i,_p);
            }
            d -= sq( _R(k,_p) = s / _R(k,k) );
        }

        if (d < 0) {
            _R(_p,_p) = std::sqrt( -d );
            break;
        }
        _R(_p,_p) = sqrt( d );
    }
}

chol_ext::Vec chol_ext::witness() const
{
    chol_ext::Vec v(_p+1);
    v[_p] = 1.0 / _R(_p,_p);

    for (int i=_p-1; i>=0; --i) {
        auto s = 0.0;
        for (int j=_p; j>i; --j) {
            s += _R(i,j) * v[j];
        }
        v[i] = -s / _R(i,i);
    }
    return v;
}

**/
#endif
