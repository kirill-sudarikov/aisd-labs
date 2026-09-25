import std;

template <typename T>
class polynomial final {
private:
  T *_coeffs = nullptr;
  std::size_t _degree = 0;
  std::size_t _capacity = 0;

  void normalize() {
    std::size_t new_degree = 0;

    for (std::size_t i = 0; i < _capacity; ++i) {
      if (_coeffs[i] != T()) {
        new_degree = i;
      }
    }

    _degree = new_degree;
  }

public:
  friend std::ostream &operator<<(std::ostream &os, const polynomial &p) {
    bool is_first = true;

    for (std::size_t i = 0; i < p._capacity; ++i) {
      if (p._coeffs[i] == T{}) {
        continue;
      }

      if (!is_first) {
        os << " + ";
      }

      os << p._coeffs[i];

      if (i > 0) {
        os << "x";

        if (i > 1) {
          os << "^" << i;
        }
      }

      is_first = false;
    }

    if (is_first) {
      os << T{};
    }

    return os;
  }

  polynomial(std::size_t max_degree)
      : _coeffs(new T[max_degree + 1]), _degree(0), _capacity(max_degree + 1) {}

  polynomial(const std::vector<T> &coeffs) {
    if (coeffs.empty()) {
      _degree = 0;
      _capacity = 1;
      _coeffs = new T[_capacity]();
    }

    _capacity = coeffs.size();
    _coeffs = new T[_capacity]();
    for (std::size_t i = 0; i < _capacity; ++i) {
      _coeffs[i] = coeffs[i];
    }
    normalize();
  }

  polynomial(const polynomial &other)
      : _coeffs(new T[other._capacity]()), _degree(other._degree),
        _capacity(other._capacity) {
    for (std::size_t i = 0; i < other._capacity; ++i) {
      _coeffs[i] = other._coeffs[i];
    }
  }

  polynomial(polynomial &&other)
      : _coeffs(other._coeffs), _degree(other._degree),
        _capacity(other._capacity) {
    other._coeffs = nullptr;
    other._degree = 0;
    other._capacity = 0;
  }

  polynomial &operator=(const polynomial &other) {
    if (this != &other) {
      delete[] _coeffs;
      _coeffs = nullptr;

      _coeffs = new T[other._capacity]();
      for (std::size_t i = 0; i < other._capacity; ++i) {
        _coeffs[i] = other._coeffs[i];
      }
      _degree = other._degree;
      _capacity = other._capacity;
    }

    return *this;
  }

  polynomial &operator=(polynomial &&other) {
    if (this != &other) {
      delete[] _coeffs;

      _coeffs = other._coeffs;
      _degree = other._degree;
      _capacity = other._capacity;

      other._coeffs = nullptr;
      other._degree = 0;
      other._capacity = 0;
    }

    return *this;
  }

  T operator[](std::size_t degree) const {
    if (degree > _degree) {
      return T();
    }

    return _coeffs[degree];
  }

  polynomial &operator+=(const polynomial &other) {
    if (other._degree >= _capacity) {
      expand(other._degree);
    }

    for (std::size_t i = 0; i <= other._degree; ++i) {
      _coeffs[i] += other._coeffs[i];
    }

    normalize();

    return *this;
  }

  polynomial &operator-=(const polynomial &other) {
    if (other._degree >= _capacity) {
      expand(other._degree);
    }

    for (std::size_t i = 0; i <= other._degree; ++i) {
      _coeffs[i] -= other._coeffs[i];
    }

    normalize();

    return *this;
  }

  polynomial &operator*=(const T &scalar) {
    for (std::size_t i = 0; i <= _degree; ++i) {
      _coeffs[i] *= scalar;
    }

    normalize();

    return *this;
  }

  T evaluate(const T &x) const {
    T r = _coeffs[_degree];

    for (std::size_t i = _degree; i > 0; --i) {
      r = r * x + _coeffs[i - 1];
    }

    return r;
  }

  void shrink_to_fit() {
    normalize();

    if (_degree + 1 != _capacity) {
      std::size_t new_capacity = _degree + 1;

      T *new_coeffs = new T[new_capacity]{};

      for (std::size_t i = 0; i < new_capacity; ++i) {
        new_coeffs[i] = _coeffs[i];
      }

      delete[] _coeffs;

      _coeffs = new_coeffs;
      _capacity = new_capacity;
    }
  }

  void expand(std::size_t degree) {
    if (degree < _capacity) {
      throw std::runtime_error(
          "requested degree is already within allocated capacity");
    }

    T *new_coeffs = new T[degree + 1]();

    for (std::size_t i = 0; i < _capacity; ++i) {
      new_coeffs[i] = _coeffs[i];
    }

    delete[] _coeffs;
    _coeffs = new_coeffs;
    _capacity = degree + 1;
  }

  void set(std::size_t degree, const T &coeff) {
    if (degree >= _capacity) {
      expand(degree);
    }

    _coeffs[degree] = coeff;
  }

  ~polynomial() { delete[] _coeffs; }
};

template <typename T>
polynomial<T> operator+(polynomial<T> a, const polynomial<T> &b) {
  return a += b;
}

template <typename T>
polynomial<T> operator-(polynomial<T> a, const polynomial<T> &b) {
  return a -= b;
}

template <typename T>
polynomial<T> operator*(polynomial<T> p, const T &scalar) {
  return p *= scalar;
}

template <typename T>
polynomial<T> operator*(const T &scalar, polynomial<T> p) {
  return p *= scalar;
}

template <typename T>
auto find_linear_root(const polynomial<T> &p) {
  T a = p[1];
  T b = p[0];

  if (a == T{0}) {
    if (b == T{0}) {
      throw std::domain_error(
          "нулевой многочлен имеет бесконечно много корней");
    }
    throw std::domain_error("линейный многочлен не имеет корней");
  }

  if constexpr (std::is_integral_v<T>) {
    return -static_cast<double>(b) / static_cast<double>(a);
  } else {
    return -b / a;
  }
}

/*
template <typename T>
std::vector<std::complex<double>> find_square_root(const polynomial<T> &p) {
  const T a = p[2];
  const T b = p[1];
  const T c = p[0];

  T D = b * b - 4 * a * c;

  if (D == T{0}) {
    if constexpr (std::is_integral_v<T>) {
      T x = -static_cast<double>(b) / (2 * a);
    }

    return x;
  } else if (D < T{0}) {
    std::complex<double> Dr(0, std::sqrt(-D));

    T x1 = (-b + Dr) / (2 * a);
    T x2 = (-b - Dr) / (2 * a);
  } else {
    T x1 = (-b + std::sqrt(D)) / (2 * a);
    T x2 = (-b - std::sqrt(D)) / (2 * a);
  }

  std::vector<T> r = {x1, x2};

  return r;
}
*/

int main() {
  std::vector<int> c = {2, 1};

  polynomial<int> p(c);

  std::cout << p;

  return 0;
}