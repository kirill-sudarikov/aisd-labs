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

template <class T>
class Polynomial {
private:
  T *_coef = nullptr;
  std::size_t _size = 0;
  std::size_t _capacity = 0;
  std::size_t _degree = 0;
  static constexpr float EPS_FLOAT = 1e-9f;
  static constexpr double EPS_DOUBLE = 1e-6;

  void normalize_degree() { // Метод нормализации степени
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
  Polynomial() {     // Конструктор по умолчанию
    _coef = new T(); // гарантировано нулевое значение
    _size = 1;
    _capacity = 1;
    _degree = 0;
  }

  Polynomial(const std::size_t max_degree) { // Конструктор с параметрами:
                                             // максимальная степень многочлена
    _coef = new T[max_degree + 1]();
    _size = max_degree + 1;
    _capacity = _size; // Потому что все элементы занулены при выделении памяти
    _degree = 0;
  }

  Polynomial(
      const std::vector<T> &coef) { // Конструктор с параметрами: вектор
                                    // значений при соответствующих степенях
    _size = coef.size();
    _coef = new T[_size];
    _capacity = _size;

    std::copy(coef.begin(), coef.end(), _coef);

    normalize_degree();
  }

  Polynomial(const Polynomial &other) { // Конструктор копирования
    _size = other._size;
    _coef = new T[_size]();
    _capacity = _size;

    std::copy(other._coef, other._coef + _size, _coef);

    normalize_degree();
  }

  Polynomial(Polynomial &&other) noexcept { // Конструктор перемещения
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

  friend void swap(Polynomial &l,
                   Polynomial &r) noexcept { // Дружественная функция для
                                             // реализации copy-and-swap idiom
    std::swap(l._coef, r._coef);
    std::swap(l._size, r._size);
    std::swap(l._capacity, r._capacity);
    std::swap(l._degree, r._degree);
  }

  Polynomial &
  operator=(const Polynomial &other) { // Оператор присваивания копированием
    Polynomial tmp(other);
    swap(*this, tmp);
    return *this;
  }

  Polynomial &
  operator=(Polynomial &&other) noexcept { // Оператор присваивания перемещением
    swap(*this, other);
    return *this;
  }

  bool
  operator==(const Polynomial &other) const { // Оператор сравнения на равенство
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

  T operator[](int degree) const {
    if (degree < 0) {
      throw std::invalid_argument("degree must be non-negative");
    }

    if (degree > _degree) {
      return T();
    }

    return _coef[degree];
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

  ~Polynomial() { // Деструктор
    delete[] _coef;
  }
};

int main() {

  std::vector<std::complex<double>> coef = {{0.0, 4.0}, {0.0, 5.0}, {0.0, 0.0}};
  Polynomial<std::complex<double>> p(coef);

  std::cout << p;
  p.set(4, {0.1, 5.0});
  std::cout << p;

  return 0;
}