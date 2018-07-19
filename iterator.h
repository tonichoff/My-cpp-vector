#pragma once
#include <iterator>

template<class T>
class Iterator;

template<class T>
struct std::iterator_traits<Iterator<T>> {
  using difference_type = std::ptrdiff_t;
  using value_type = T;
  using reference = T&;
  using pointer = T*;
  using iterator_category = std::random_access_iterator_tag;
};

template<class T>
class Iterator {
public:
  using value_type = T;
  using difference_type = std::ptrdiff_t;
  using pointer = T*;
  using reference = T&;
  using iterator_category = std::random_access_iterator_tag;
  using size_type = std::size_t;

  Iterator() {}
  Iterator(T* ptr) : _ptr(ptr) {}
  Iterator(const Iterator& other) : _ptr(other._ptr) {}
  Iterator(const Iterator&& other) : _ptr(std::move(other._ptr)) {}
  ~Iterator() {}

  Iterator& operator=(const Iterator& other) {
    _ptr = other._ptr;
    return *this;
  }

  Iterator& operator=(Iterator&& other) {
    _ptr = std::move(_ptr, other._ptr);
  }

  Iterator& operator++() {
    _ptr++;
    return *this;
  }

  Iterator& operator++(int) {
    Iterator it(_ptr);
    _ptr++;
    return it;
  }

  Iterator& operator+=(size_type count) {
    _ptr += count;
    return *this
  }

  Iterator& operator--() {
    _ptr--;
    return *this;
  }

  Iterator& operator--(int) {
    Iterator it(_ptr);
    _ptr--;
    return it;
  }

  Iterator& operator-=(size_type count) {
    _ptr -= count;
    return *this
  }

  friend Iterator operator+(const Iterator& other, size_type count) {
    return Iterator(other._ptr + count);
  }

  friend Iterator operator+(size_type count, const Iterator& other) {
    return Iterator(other._ptr + count);
  }

  friend Iterator operator-(const Iterator& other, size_type count) {
    return Iterator(other._ptr - count);
  }

  friend difference_type operator-(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr - rhs._ptr;
  }

  reference operator*() const {
    return *_ptr;
  }

  pointer operator->() const {
    return _ptr;
  }

  reference operator[](size_type pos) const {
    return *(_ptr + pos);
  }

  friend bool operator==(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr == rhs._ptr;
  }

  friend bool operator!=(const Iterator& rhs, const Iterator& lhs) {
    return lhs._ptr != rhs._ptr;
  }

  friend bool operator<(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr < rhs._ptr;
  }

  friend bool operator>(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr > rhs._ptr;
  }

  friend bool operator<=(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr <= rhs._ptr;
  }

  friend bool operator>=(const Iterator& lhs, const Iterator& rhs) {
    return lhs._ptr >= rhs._ptr;
  }

  friend void swap(Iterator& lhs, Iterator& rhs) {
    std::swap(lhs._ptr, rhs._ptr);
  }
private:
  pointer _ptr;
};