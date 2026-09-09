import std;

template <typename T>
class polynomial final {
private:
  int _deg = 0;
  int _capacity = 0;
  T *_coeffs = nullptr;

public:
  polynomial(int max_deg) : _deg(0), _capacity(0), _coeffs(nullptr) {
    if (max_deg < 0) {
      throw std::invalid_argument("max_deg must be non-negative");
    }

    _capacity = max_deg + 1;
    _coeffs = new T[_capacity]();
  }

  ~polynomial() { delete[] _coeffs; }
};

int main() {
  try {
    polynomial<int> p(-2);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << '\n';
  }
  return 0;
}