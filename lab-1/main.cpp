import std;

template <typename T>
class polynomial final {
private:
  std::vector<T> _coeffs;

public:
  polynomial() = default;
  polynomial(std::size_t max_deg) : _coeffs(max_deg + 1) {}
  polynomial(const std::vector<T>& coeffs) : _coeffs(coeffs) {}

  T operator[](std::size_t deg) const {
    if (deg > _coeffs.size() - 1) {
      return T(0);
    }
    return _coeffs[deg];
  }
};

int main() { return 0; }