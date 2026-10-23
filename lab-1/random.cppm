export module random;
import std;
import Polynomial;

export Polynomial<int> make_random(std::size_t degree, int min_val,
                                   int max_val) {
  std::vector<int> coef(degree + 1);
  std::random_device rd;
  std::mt19937 prng(rd());
  std::uniform_int_distribution<int> d(min_val, max_val);

  for (int &val : coef) {
    val = d(prng);
  }

  Polynomial<int> p(coef);

  return p;
}

export Polynomial<float> make_random(std::size_t degree, float min_val,
                                     float max_val) {
  std::vector<float> coef(degree + 1);
  std::random_device rd;
  std::mt19937 prng(rd());
  std::uniform_real_distribution<float> d(min_val, max_val);

  for (float &val : coef) {
    val = d(prng);
  }

  Polynomial<float> p(coef);

  return p;
}

export Polynomial<double> make_random(std::size_t degree, double min_val,
                                      double max_val) {
  std::vector<double> coef(degree + 1);
  std::random_device rd;
  std::mt19937 prng(rd());
  std::uniform_real_distribution<double> d(min_val, max_val);

  for (double &val : coef) {
    val = d(prng);
  }

  Polynomial<double> p(coef);

  return p;
}

export Polynomial<std::complex<float>>
make_random_complex(std::size_t degree, float min_val, float max_val) {
  std::vector<std::complex<float>> coef(degree + 1);
  std::random_device rd;
  std::mt19937 prng(rd());
  std::uniform_real_distribution<float> d(min_val, max_val);

  for (auto &val : coef) {
    val = {d(prng), d(prng)};
  }

  Polynomial<std::complex<float>> p(coef);

  return p;
}

export Polynomial<std::complex<double>>
make_random_complex(std::size_t degree, double min_val, double max_val) {
  std::vector<std::complex<double>> coef(degree + 1);
  std::random_device rd;
  std::mt19937 prng(rd());
  std::uniform_real_distribution<double> d(min_val, max_val);

  for (auto &val : coef) {
    val = {d(prng), d(prng)};
  }

  Polynomial<std::complex<double>> p(coef);

  return p;
}
