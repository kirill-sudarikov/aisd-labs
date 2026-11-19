import std;
import random;
import linked_list;

template <typename T, typename Compare = std::less<T>>
LinkedList<T> merge_sorted_unique(const LinkedList<T> &a,
                                  const LinkedList<T> &b,
                                  Compare comp = Compare()) {
  LinkedList<T> result;
  int i = 0;
  int j = 0;

  auto add_unique = [&](const T &val) {
    if (result.size() == 0 || !LinkedList<T>::is_equal(val, result[-1], 2)) {
      result.push_tail(val);
    }
  };

  while (i < a.size() && j < b.size()) {
    if (LinkedList<T>::is_equal(a[i], b[j], 2)) {
      add_unique(a[i]);
      i++;
      j++;
    } else if (comp(a[i], b[j])) {
      add_unique(a[i]);
      i++;
    } else {
      add_unique(b[j]);
      j++;
    }
  }

  while (i < a.size()) {
    add_unique(a[i]);
    i++;
  }

  while (j < b.size()) {
    add_unique(b[j]);
    j++;
  }

  return result;
}

int main() {
  try {
    LinkedList<double> list_1;
    LinkedList<double> list_2;

    list_1.push_tail(4.0);
    list_1.push_tail(3.0);
    list_1.push_tail(2.0);
    list_1.push_tail(1.0);

    list_2.push_tail(11.3);
    list_2.push_tail(11.1);
    list_2.push_tail(6.0);
    list_2.push_tail(3.3);
    list_2.push_tail(2.0);
    list_2.push_tail(1.1);

    std::cout << list_1 << std::endl;
    std::cout << list_2 << std::endl;

    LinkedList<double> merged =
        merge_sorted_unique<double, std::greater<double>>(list_1, list_2);

    std::cout << merged << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}