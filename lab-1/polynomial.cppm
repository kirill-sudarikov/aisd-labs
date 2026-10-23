export module Polynomial;

import std;

template <typename T>
struct is_complex : std::false_type {};

template <typename T>
struct is_complex<std::complex<T>> : std::true_type {};

template <typename T>
constexpr bool is_complex_v = is_complex<T>::value;

template <typename U>
struct complex_traits {
  using real_type = void;
};

template <typename U>
struct complex_traits<std::complex<U>> {
  using real_type = U;
};

export template <class T>
class Polynomial {
private:
  T *_coef = nullptr;
  std::size_t _size = 0;
  std::size_t _capacity = 0;
  std::size_t _degree = 0;
  static constexpr float EPS_FLOAT = 1e-9f;
  static constexpr double EPS_DOUBLE = 1e-6;

  void normalize_degree() {
    auto get_eps = []() {
      if constexpr (is_complex_v<T>) {
        using real_type = typename complex_traits<T>::real_type;

        if constexpr (std::is_same_v<real_type, float>)
          return EPS_FLOAT;
        else if constexpr (std::is_same_v<real_type, double>)
          return EPS_DOUBLE;
        else if constexpr (std::is_same_v<real_type, int>)
          return 0.0;
      } else {
        if constexpr (std::is_same_v<T, float>)
          return EPS_FLOAT;
        else if constexpr (std::is_same_v<T, double>)
          return EPS_DOUBLE;
        else if constexpr (std::is_same_v<T, int>)
          return 0;
      }
    };

    const auto eps = get_eps();

    if constexpr (is_complex_v<T>) {
      for (std::size_t i = _size; i > 0; --i) {
        if (std::abs(_coef[i - 1].real()) > eps ||
            std::abs(_coef[i - 1].imag()) > eps) {
          _degree = i - 1;
          return;
        }
      }
    } else {
      for (std::size_t i = _size; i > 0; --i) {
        if (std::abs(_coef[i - 1]) > eps) {
          _degree = i - 1;
          return;
        }
      }
    }

    _degree = 0;
  }

public:
  Polynomial() {
    _coef = new T();
    _size = 1;
    _capacity = 1;
    _degree = 0;
  }

  Polynomial(const std::size_t max_degree) {

    _coef = new T[max_degree + 1]();
    _size = max_degree + 1;
    _capacity = _size;
    _degree = 0;
  }

  Polynomial(const std::vector<T> &coef) {
    _size = coef.size();
    _coef = new T[_size];
    _capacity = _size;

    std::copy(coef.begin(), coef.end(), _coef);

    normalize_degree();
  }

  Polynomial(const Polynomial &other) {
    _size = other._size;
    _coef = new T[_size]();
    _capacity = _size;

    std::copy(other._coef, other._coef + _size, _coef);

    normalize_degree();
  }

  Polynomial(Polynomial &&other) noexcept {
    _coef = other._coef;
    _size = other._size;
    _capacity = other._capacity;
    _degree = other._degree;

    other._coef = nullptr;
    other._size = 0;
    other._capacity = 0;
    other._degree = 0;

    normalize_degree();
  }

  friend void swap(Polynomial &l, Polynomial &r) noexcept {
    std::swap(l._coef, r._coef);
    std::swap(l._size, r._size);
    std::swap(l._capacity, r._capacity);
    std::swap(l._degree, r._degree);
  }

  Polynomial &operator=(const Polynomial &other) {
    Polynomial tmp(other);
    swap(*this, tmp);
    return *this;
  }

  Polynomial &operator=(Polynomial &&other) noexcept {
    swap(*this, other);
    return *this;
  }

  bool operator==(const Polynomial &other) const {
    if (_degree != other._degree) {
      return false;
    }

    if constexpr (is_complex_v<T>) {
      using real_type = typename T::value_type;
      const real_type epsilon = std::numeric_limits<real_type>::epsilon();

      for (std::size_t i = 0; i <= _degree; ++i) {
        if (std::abs(_coef[i].real() - other._coef[i].real()) > epsilon ||
            std::abs(_coef[i].imag() - other._coef[i].imag()) > epsilon) {
          return false;
        }
      }
    } else {
      for (std::size_t i = 0; i <= _degree; ++i) {
        if (std::abs(_coef[i] - other._coef[i]) >
            std::numeric_limits<T>::epsilon()) {
          return false;
        }
      }
    }

    return true;
  }

  bool operator!=(const Polynomial &other) const { return !(*this == other); }

  std::size_t degree() const noexcept { return _degree; }
  T operator[](int degree) const {
    if (degree < 0) {
      throw std::invalid_argument("degree must be non-negative");
    }

    if (degree > _degree) {
      return T();
    }

    return _coef[degree];
  }

  Polynomial &operator+=(const Polynomial &other) {
    if (other._degree >= _capacity)
      expand(other._degree);

    for (std::size_t i = 0; i <= other._degree; ++i) {
      _coef[i] += other._coef[i];
    }

    normalize_degree();

    return *this;
  }

  Polynomial &operator-=(const Polynomial &other) {
    if (other._degree >= _capacity)
      expand(other._degree);

    for (std::size_t i = 0; i <= other._degree; ++i) {
      _coef[i] -= other._coef[i];
    }

    normalize_degree();

    return *this;
  }

  Polynomial &operator*=(const T &scalar) {
    for (std::size_t i = 0; i <= _degree; ++i) {
      _coef[i] *= scalar;
    }

    normalize_degree();
    return *this;
  }

  T operator()(const T &x) const {
    T r = _coef[_degree];

    for (std::size_t i = _degree; i > 0; --i) {
      r = r * x + _coef[i - 1];
    }

    return r;
  }

  friend std::ostream &operator<<(std::ostream &os, const Polynomial &p) {
    if (p._degree == 0 && p._coef[0] == T()) {
      return os << 0;
    }

    bool is_first = true;

    for (std::size_t i = p._degree + 1; i-- > 0;) {
      if (p._coef[i] == T()) {
        continue;
      }

      if constexpr (is_complex_v<T>) {
        if (!is_first) {
          os << "+";
        }

        os << '(' << std::noshowpos << p._coef[i].real() << std::showpos
           << p._coef[i].imag() << std::noshowpos << "i)";

      } else {
        if (is_first) {
          os << std::noshowpos << p._coef[i];
        } else {
          os << std::showpos << p._coef[i];
        }
      }

      is_first = false;

      if (i == 1) {
        os << 'x';
      } else if (i != 0) {
        os << 'x' << '^' << i;
      }
    }

    return os << std::noshowpos;
  }

  void expand(int degree) {
    if (degree < 0) {
      throw std::invalid_argument("degree must be non-negative");
    }

    if (degree < _capacity) {
      return;
    } else {
      std::size_t new_capacity = degree + 1;
      T *new_coef = new T[new_capacity]();
      std::size_t new_size = new_capacity;

      for (std::size_t i = 0; i <= _degree; ++i) {
        new_coef[i] = _coef[i];
      }

      delete[] _coef;
      _coef = new_coef;
      _capacity = new_capacity;
      _size = new_size;
    }
  }

  void shrink_to_fit() {
    std::size_t new_capacity = _degree + 1;

    if (_capacity == new_capacity) {
      return;
    } else {
      T *new_coef = new T[new_capacity]();
      std::size_t new_size = new_capacity;

      for (std::size_t i = 0; i <= _degree; ++i) {
        new_coef[i] = _coef[i];
      }

      delete[] _coef;
      _coef = new_coef;
      _capacity = new_capacity;
      _size = new_size;
    }
  }

  void set(int degree, T value) {
    if (degree < 0) {
      throw std::invalid_argument("degree must be non-negative");
    }

    if (degree >= _capacity) {
      expand(degree);
    }

    _coef[degree] = value;

    normalize_degree();
  }

  ~Polynomial() { delete[] _coef; }
};

export template <class T>
Polynomial<T> operator+(Polynomial<T> a, const Polynomial<T> &b) {
  return a += b;
}

export template <class T>
Polynomial<T> operator-(Polynomial<T> a, const Polynomial<T> &b) {
  return a -= b;
}

export template <class T>
Polynomial<T> operator*(Polynomial<T> p, const T &scalar) {
  return p *= scalar;
}

export template <class T>
Polynomial<T> operator*(const T &scalar, Polynomial<T> p) {
  return p *= scalar;
}

export template <typename T>
auto find_linear_root(const Polynomial<T> &p) {
  using R = std::conditional_t<std::is_integral_v<T>, double, T>;

  if (p.degree() != 1) {
    throw std::invalid_argument("Polynomial degree must be 1");
  }

  const R a = R(p[1]);
  const R b = R(p[0]);

  if (a == R{}) {
    throw std::invalid_argument("Leading coefficient must not be zero");
  }

  return -b / a;
}

export template <typename T>
auto find_quadratic_roots(const Polynomial<T> &p) {
  using R = std::conditional_t<std::is_integral_v<T>, double, T>;

  if (p.degree() != 2) {
    throw std::invalid_argument("Polynomial degree must be 2");
  }

  const R a = R(p[2]);
  const R b = R(p[1]);
  const R c = R(p[0]);

  if (a == R{}) {
    throw std::invalid_argument("Leading coefficient must not be zero");
  }

  const R discriminant = b * b - R(4) * a * c;

  if constexpr (std::is_floating_point_v<R>) {
    if (discriminant < R{}) {
      throw std::domain_error("Polynomial has no real roots");
    }
  }

  const R sqrt_d = std::sqrt(discriminant);
  const R denominator = R(2) * a;

  return std::pair<R, R>{(-b + sqrt_d) / denominator,
                         (-b - sqrt_d) / denominator};
}