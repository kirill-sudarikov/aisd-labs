import std;
import random;
import linked_list;

template <typename T>
LinkedList<T> merge_sorted_unique(const LinkedList<T> &a,
                                  const LinkedList<T> &b) {
  LinkedList<T> result;
  int i = 0;
  int j = 0;

  auto add_unique = [&](const T &val) {
    if (result.size() == 0 || !LinkedList<T>::is_equal(val, result[-1], 2)) {
      result.push_tail(val);
    }
  };

  while (i < a.size() && j < b.size()) {
    if (a[i] < b[j]) {
      add_unique(a[i]);
      i++;
    } else if (b[j] < a[i]) {
      add_unique(b[j]);
      j++;
    } else {
      add_unique(a[i]);
      i++;
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

    list_1.push_tail(1.0);
    list_1.push_tail(2.0);
    list_1.push_tail(3.0);
    list_1.push_tail(4.0);

    list_2.push_tail(1.0);
    list_2.push_tail(4.0);
    list_2.push_tail(6.0);
    list_2.push_tail(10.0);
    list_2.push_tail(11.0);
    list_2.push_tail(11.1);

    std::cout << list_1 << std::endl;
    std::cout << list_2 << std::endl;

    LinkedList<double> merged = merge_sorted_unique<double>(list_1, list_2);

    std::cout << merged << std::endl;
  } catch (const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  return 0;
}