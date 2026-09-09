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
  polynomial(std::size_t max_degree)
      : _coeffs(new T[max_degree + 1]), _degree(0), _capacity(max_degree + 1) {}

  polynomial(const std::vector &coeffs) {
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

int main() { return 0; }