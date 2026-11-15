export module random;
import std;

export template <typename T>
class RandomGenerator {
private:
  std::random_device _rd;
  std::mt19937 _gen;
  using dist_type = std::conditional_t<std::is_integral_v<T>,
                                       std::uniform_int_distribution<T>,
                                       std::uniform_real_distribution<T>>;
  dist_type _dist;

public:
  RandomGenerator(const T &min, const T &max) : _gen(_rd()), _dist(min, max) {}
  RandomGenerator(const T &min, const T &max, unsigned int seed)
      : _gen(seed), _dist(min, max) {}
  void set_seed(unsigned int seed) { _gen.seed(seed); }
  T get() { return _dist(_gen); }
  T operator()() { return get(); }
};