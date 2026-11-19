export module linked_list;
import std;
import random;

template <typename T>
struct Node {
  T info;
  Node *next;

  Node() : info(T()), next(nullptr) {}
};

export template <typename T>
class LinkedList {
private:
  Node<T> *_head = nullptr;
  int _size = 0;

public:
  LinkedList() {
    _head = new Node<T>;
    _head->next = _head;
  }

  LinkedList(const LinkedList &other) {
    if (other._size < 0) {
      throw std::invalid_argument("list size must be non-negative");
    } else if (!other._head) {
      throw std::invalid_argument("list must exists");
    }

    _size = other._size;
    _head = new Node<T>;
    _head->next = _head;

    if (!_size) {
      return;
    }

    Node<T> *p = _head;
    Node<T> *q = other._head->next;

    while (q != other._head) {
      Node<T> *w = new Node<T>;

      w->info = q->info;
      p->next = w;
      w->next = _head;

      p = p->next;
      q = q->next;
    }
  }

  LinkedList(int size, unsigned int seed, const T &min_val = T(0),
             const T &max_val = T(100)) {
    if (size < 0) {
      throw std::invalid_argument("list size must be non-negative");
    }
    RandomGenerator<T> rg(min_val, max_val, seed);

    _size = size;
    _head = new Node<T>;
    _head->next = _head;

    Node<T> *p = _head;

    for (int i = 0; i < size; ++i) {
      Node<T> *q = new Node<T>;
      q->info = rg();
      q->next = _head;
      p->next = q;

      p = p->next;
    }
  }

  LinkedList &operator=(const LinkedList &other) {
    if (this == &other) {
      return *this;
    }

    if (other._size < 0) {
      throw std::invalid_argument("list size must be non-negative");
    } else if (!other._head) {
      throw std::invalid_argument("list must exists");
    }

    _size = other._size;

    if (_head) {
      Node<T> *u = _head->next;

      while (u != _head) {
        Node<T> *q = u->next;
        delete u;
        u = q;
      }

      delete _head;
    }

    _head = new Node<T>;
    _head->next = _head;

    Node<T> *p = _head;
    Node<T> *q = other._head->next;

    while (q != other._head) {
      Node<T> *w = new Node<T>;

      w->info = q->info;
      p->next = w;
      w->next = _head;

      p = p->next;
      q = q->next;
    }

    return *this;
  }

  void push_tail(const T &elm) {
    if (!_head) {
      _head = new Node<T>;
      _head->next = _head;
    }

    Node<T> *p = _head;

    while (p->next != _head) {
      p = p->next;
    }

    Node<T> *q = new Node<T>;
    q->info = elm;
    q->next = _head;
    p->next = q;

    _size += 1;
  }

  void push_tail(const LinkedList &other) {
    if (!other._head) {
      throw std::invalid_argument("other list must exist");
    } else if (other._size < 0) {
      throw std::invalid_argument("list size must be non-negative");
    }

    if (other._head->next == other._head) {
      return;
    }

    if (this == &other) {
      LinkedList<T> copy(other);
      push_tail(copy);
      return;
    }

    if (!_head) {
      _head = new Node<T>;
      _head->next = _head;
    }

    Node<T> *p = _head;

    while (p->next != _head) {
      p = p->next;
    }

    Node<T> *q = other._head->next;

    while (q != other._head) {
      Node<T> *w = new Node<T>;

      w->info = q->info;
      p->next = w;
      w->next = _head;

      p = p->next;
      q = q->next;
    }

    _size += other._size;
  }

  void push_head(const T &elm) {
    if (!_head) {
      _head = new Node<T>;
      _head->next = _head;
    }

    Node<T> *p = new Node<T>;
    p->info = elm;
    p->next = _head->next;
    _head->next = p;

    _size += 1;
  }

  void push_head(const LinkedList &other) {
    if (!other._head) {
      throw std::invalid_argument("other list must exist");
    } else if (other._size < 0) {
      throw std::invalid_argument("list size must be non-negative");
    }

    if (other._head->next == other._head) {
      return;
    }

    if (this == &other) {
      LinkedList<T> copy(other);
      push_head(copy);
      return;
    }

    if (!_head) {
      _head = new Node<T>;
      _head->next = _head;
    }

    Node<T> *p = _head;
    Node<T> *q = other._head->next;

    while (q != other._head) {
      Node<T> *w = new Node<T>;

      w->info = q->info;
      w->next = p->next;
      p->next = w;

      p = p->next;
      q = q->next;
    }

    _size += other._size;
  }

  void pop_head() {
    if (!_head || _head->next == _head) {
      return;
    }

    Node<T> *p = _head->next;
    _head->next = p->next;
    delete p;
    _size -= 1;
  }

  void pop_tail() {
    if (!_head || _head->next == _head) {
      return;
    }

    Node<T> *p = _head;
    while (p->next->next != _head) {
      p = p->next;
    }
    delete p->next;
    p->next = _head;
    _size -= 1;
  }

  void delete_node(const T &Info) {
    if (!_head || _head->next == _head) {
      return;
    }

    Node<T> *p = _head;

    while (p->next != _head) {
      if (is_equal(p->next->info, Info, 2)) {
        Node<T> *q = p->next;
        p->next = q->next;
        delete q;
        _size -= 1;
      } else {
        p = p->next;
      }
    }
  }

  int size() const { return _size; }

  static bool is_equal(const T &a, const T &b, int ulp) {
    if constexpr (std::is_floating_point_v<T>) {
      return std::abs(a - b) <= std::numeric_limits<T>::epsilon() *
                                    std::max(std::abs(a), std::abs(b)) * ulp ||
             std::abs(a - b) < std::numeric_limits<T>::min();
    } else {
      return a == b;
    }
  }

  T &operator[](int index) {
    if (!_head || _size <= 0) {
      throw std::out_of_range("list is empty");
    }

    int norm_i = ((index % _size) + _size) % _size;

    Node<T> *p = _head->next;
    for (int i = 0; i < norm_i; ++i) {
      p = p->next;
    }

    return p->info;
  }

  const T &operator[](int index) const {
    if (!_head || _size <= 0) {
      throw std::out_of_range("list is empty");
    }

    int norm_i = ((index % _size) + _size) % _size;

    Node<T> *p = _head->next;
    for (int i = 0; i < norm_i; ++i) {
      p = p->next;
    }

    return p->info;
  }

  friend std::ostream &operator<<(std::ostream &os, const LinkedList<T> &list) {
    os << "[";

    if (list._head && list._size > 0) {
      Node<T> *p = list._head->next;

      while (p->next != list._head) {
        os << p->info << ", ";
        p = p->next;
      }

      os << p->info;
    }

    os << "]";
    return os;
  }

  ~LinkedList() {
    if (!_head) {
      return;
    }

    Node<T> *p = _head->next;

    while (p != _head) {
      Node<T> *q = p->next;
      delete p;
      p = q;
    }

    delete _head;
  }
};