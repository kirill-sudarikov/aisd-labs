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

  // Добавление списка LinkedList в начало списка — push_head (перегруженный
  // метод).
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