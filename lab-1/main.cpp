import std;
import Polynomial;
import random;

template <class T>
void demonstrate(Polynomial<T> p, Polynomial<T> q) {
  std::cout << "\n\n";
  std::cout << "Original polynomials:" << '\n';
  std::cout << "P(x) = " << p << '\n';
  std::cout << "Q(x) = " << q << "\n\n";
  std::cout << "Addition:" << '\n';
  std::cout << "P(x) + Q(x) = " << p + q << "\n\n";
  std::cout << "Subtraction:" << '\n';
  std::cout << "P(x) - Q(x) = " << p - q << "\n";
  std::cout << "Q(x) - P(x) = " << q - p << "\n\n";
  std::cout << "Scalar multiplication:" << '\n';
  std::cout << "P(x) * 5 = " << p * T(5) << "\n";
  std::cout << "5 * P(x) = " << T(5) * p << "\n\n";
  std::cout << "Reading the coefficient:" << '\n';
  std::cout << "Input degree: ";
  int degree = 0;
  std::cin >> degree;
  std::cout << "P[" << degree << "] = " << p[degree] << "\n\n";
  std::cout << "Change coefficient:" << '\n';
  std::cout << "Input degree: ";
  std::cin >> degree;
  std::cout << "Before: P[" << degree << "] = " << p[degree] << "\n";
  p.set(degree, T(5));
  std::cout << "After: P[" << degree << "] = " << p[degree] << "\n\n";
  std::cout << "Updated P(x) = " << p << "\n\n";
  std::cout << "Equality P(x) and Q(X): " << (p == q) << '\n';
  std::cout << "Inequality P(x) and Q(X): " << (p != q) << "\n\n";
  std::cout << "P(2) = " << p(T(2)) << "\n\n";

  Polynomial<T> source(p);
  Polynomial<T> moved(std::move(source));
  std::cout << "Move construction: " << moved << '\n';

  Polynomial<T> assigned;
  assigned = std::move(moved);
  std::cout << "Move assignment: " << assigned << "\n\n";

  std::cout << "Root finding:\n";

  if (p.degree() == 1) {
    const auto root = find_linear_root(p);
    std::cout << "Root: " << root << '\n';
  } else if (p.degree() == 2) {
    const auto [x1, x2] = find_quadratic_roots(p);
    std::cout << "x1 = " << x1 << '\n';
    std::cout << "x2 = " << x2 << '\n';
  } else {
    std::cout << "Root finding is implemented only for degrees 1 and 2.\n";
  }

  std::cout << '\n';
}

int main() {
  std::cout << "Coeffs type:" << '\n';
  std::cout << "1. int" << '\n';
  std::cout << "2. float" << '\n';
  std::cout << "3. double" << '\n';
  std::cout << "4. std::complex<float>" << '\n';
  std::cout << "5. std::complex<double>" << "\n\n";

  std::cout << "Select a menu item: ";
  int choice = 0;
  std::cin >> choice;

  try {
    if (choice < 1 || choice > 5) {
      throw std::invalid_argument("choice must be in range(1,5)");
    }

    std::cout << "Input polynomial degree: ";
    int degree = 0;
    std::cin >> degree;

    switch (choice) {
    case 1: {
      Polynomial<int> p = make_random(degree, -10, 10);
      Polynomial<int> q = make_random(degree, -10, 10);
      demonstrate(p, q);
      break;
    }
    case 2: {
      Polynomial<float> p = make_random(degree, -10.0f, 10.0f);
      Polynomial<float> q = make_random(degree, -10.0f, 10.0f);
      demonstrate(p, q);
      break;
    }
    case 3: {
      Polynomial<double> p = make_random(degree, -10.0, 10.0);
      Polynomial<double> q = make_random(degree, -10.0, 10.0);
      demonstrate(p, q);
      break;
    }
    case 4: {
      Polynomial<std::complex<float>> p =
          make_random_complex(degree, -10.0f, 10.0f);
      Polynomial<std::complex<float>> q =
          make_random_complex(degree, -10.0f, 10.0f);
      demonstrate(p, q);
      break;
    }
    case 5: {
      Polynomial<std::complex<double>> p =
          make_random_complex(degree, -10.0, 10.0);
      Polynomial<std::complex<double>> q =
          make_random_complex(degree, -10.0, 10.0);
      demonstrate(p, q);
      break;
    }
    default:
      break;
    }
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}