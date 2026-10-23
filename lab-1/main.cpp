import std;
import Polynomial;
import random;

int main() {
  Polynomial<int> p = make_random(2, -10, 10);
  Polynomial<float> q = make_random(2, -10.0f, 10.0f);
  Polynomial<double> a = make_random(2, -10.0, 10.0);
  Polynomial<std::complex<float>> b = make_random_complex(2, -10.0f, 10.0f);
  Polynomial<std::complex<double>> c = make_random_complex(4, -10.0, 10.0);

  std::cout << p << '\n';
  std::cout << q << '\n';
  std::cout << a << '\n';
  std::cout << b << '\n';
  std::cout << c << '\n';

  return 0;
}