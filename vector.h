#pragma once
#include "iterator.h"
#include <allocators>
#include <exception>
#include <initializer_list>

template<class T, class Allocator = std::allocator<T>>
class Vector {
public:
  //Member types
  using value_type = T;
  using allocator_type = Allocator;
  using size_type = std::size_t;
  using differnce_type = std::ptrdiff_t;
  using reference = T&;
  using const_reference = const T&;
  using pointer = typename std::allocator_traits<Allocator>::pointer;
  using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;
  using iterator = typename Iterator<T>;
  using const_iterator = typename const Iterator<T>;
  using reverse_iterator = typename std::reverse_iterator<iterator>;
  using const_reverse_iterator = typename const std::reverse_iterator<iterator>;

  //Constructors
  Vector() noexcept(noexcept(Allocator()))
    : _size(0),
      _capacity(_size),
      _alloc(Allocator()),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {}

  explicit Vector(const Allocator& alloc) noexcept
    : _size(0),
      _capacity(_size),
      _alloc(alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {}

  explicit Vector(size_type count, const T& value, const Allocator& alloc = Allocator())
    : _size(count),
      _capacity(_size),
      _alloc(alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {
    for (size_type i = 0; i < count; i++)
      std::allocator_traits<Allocator>::construct(_alloc, _ptr + i, value);
  }

  explicit Vector(size_type count, const Allocator& alloc = Allocator())
    : _size(count),
      _capacity(_size),
      _alloc(alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {
    for (size_type i = 0; i < count; i++)
      std::allocator_traits<Allocator>::construct(_alloc, _ptr + i);
  }

  template<class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
  Vector(InputIt first, InputIt last, const Allocator& alloc = Allocator())
    : _size(std::distance(first, last)),
      _capacity(_size),
      _alloc(alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {
    for (auto it = first; it != last; it++)
      std::allocator_traits<Allocator>::construct(_alloc, _ptr + (it - first), *it);
  }

  Vector(const Vector& other)
    : _size(other._size),
      _capacity(other._capacity), 
      _alloc(other._alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {
    for (size_type i = 0; i < other._size; i++)
      std::allocator_traits<Allocator>::construct(_alloc, _ptr + i, other[i]);
  }

  Vector(const Vector& other, const Allocator& alloc)
    : _size(other._size),
      _capacity(other._capacity),
      _alloc(alloc),
      _ptr(std::allocator_traits<Allocator>::allocate(_alloc, _capacity)) {
    for (size_type i = 0; i < other._size; i++)
      std::allocator_traits<Allocator>::construct(_alloc, _ptr + i, other[i]);
  }

  Vector(const Vector&& other) noexcept
    : _size(other._size),
      _capacity(other._capacity),
      _alloc(other._alloc),
      _ptr(other._ptr) {}

  Vector(const Vector&& other, const Allocator& alloc) noexcept
    : _size(other._size),
      _capacity(other._capacity),
      _alloc(alloc),
      _ptr(other._ptr) {}

  Vector(std::initializer_list<T> init, const Allocator& alloc = Allocator())
    : Vector(init.begin(), init.end(), alloc) {}

  //~Vector() {
  //  std::allocator_traits<Allocator>::deallocate(_alloc, _ptr, _capacity);
  //}

  //operator= and assign
  Vector& operator=(const Vector& other) {
    clear();
    _size = other._size;
    _capacity = other._capacity;
    _alloc = other._alloc;
    _ptr = other._ptr;
    return *this;
  }

  Vector& operator=(Vector&& other) noexcept {
    clear();
    swap(other);
    return *this;
  }

  Vector& operator=(std::initializer_list<T> ilist) {
    return operator=(Vector(ilist));
  }

  void assign(size_type count, const T& value) {
    clear();
    insert(begin(), count, value);
  }

  template<class InputIt, class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
  void assign(InputIt first, InputIt last) {
    clear();
    insert(begin(), first, last);
  }
  
  void assign(std::initializer_list<T> ilist) {
    clear();
    insert(begin(), ilist);
  }

  allocator_type getAllocator() const {
    return _alloc;
  }

  //Element access
  reference at(size_type pos) {
    if (pos >= _size)
      throw std::out_of_range("Vector subscript out of range");
    return _ptr[pos];
  }

  const_reference at(size_type pos) const {
    return _ptr[pos];
  }

  reference operator[](size_type pos) {
    return _ptr[pos];
  }

  const_reference operator[](size_type pos) const {
    return _ptr[pos];
  }

  reference front() {
    return _ptr[0];
  }

  const_reference front() const {
    return _ptr[0];
  }

  reference back() {
    return _ptr[_size - 1];
  }

  const_reference back() const {
    return _ptr[_size - 1];
  }

  T* data() noexcept {
    return _ptr;
  }

  const T* data() const noexcept {
    return _ptr;
  }

  //Iterators
  iterator begin() noexcept {
    return iterator(_ptr);
  }

  const_iterator begin() const noexcept {
    return iterator(_ptr);
  }

  const_iterator cbegin() noexcept {
    return const_iterator(_ptr);
  }

  iterator end() noexcept {
    return iterator(_ptr + _size);
  }

  const_iterator end() const noexcept {
    return iterator(_ptr + _size);
  }

  const_iterator cend() noexcept {
    return const_iterator(_ptr + _size);
  }

  reverse_iterator rbegin() noexcept {
    return reverse_iterator(_ptr + _size);
  }

  const_reverse_iterator rbegin() const noexcept {
    return const_reverse_iterator(_ptr + _size);
  }

  const_reverse_iterator crbegin() noexcept {
    return const_reverse_iterator(_ptr + _size);
  }

  reverse_iterator rend() noexcept {
    return reverse_iterator(_ptr);
  }

  const_reverse_iterator rend() const noexcept {
    return const_reverse_iterator(_ptr);
  }

  const_reverse_iterator crend() noexcept {
    return const_reverse_iterator(_ptr);
  }

  //Capacity
  bool empty() const noexcept {
    return !(_size);
  }

  size_type size() const noexcept {
    return _size;
  }

  size_type max_size() const noexcept {
    return std::numeric_limits<size_type>::max() / sizeof(value_type);
  }

  void reserve(size_type new_cap) {
    if (new_cap <= _capacity)
      return;

    try {
      reallocate(new_cap);
    }
    catch (std::exception&) {
      throw;
    }
  }

  size_type capacity() const noexcept {
    return _capacity;
  }

  void shrink_to_fit() {
    try {
      reallocate(_size);
    }
    catch (std::exception& ex) {
      throw ex;
    }
  }

  //Modifiers
  void clear() noexcept {
    for (auto it = begin(); it != end(); it++)
      std::allocator_traits<Allocator>::destroy(_alloc, _ptr + (it - begin()));
    _size = 0;
  }

  iterator insert(const_iterator pos, const T& value) {
    return insert(pos, 1, value);
  }

  iterator insert(const_iterator pos, T&& value) {
    return insert(pos, value);
  }

  iterator insert(const_iterator pos, size_type count, const T& value) {
    size_type new_size = _size + count;
    size_type index = pos - begin();
    if (new_size > _capacity)
      reallocate(new_size);
    auto it = begin() + index;
    std::copy_backward(it, end(), begin() + new_size);
    std::fill(it, it + count, value);

    _capacity = new_size;
    _size = new_size;
    return it;
  }

  template< class InputIt >
  void insert(const_iterator pos, InputIt first, InputIt last) {
    size_type new_size = _size + last - first;
    size_type index = pos - begin();
    if (new_size > _capacity)
      reallocate(new_size);
    auto it = begin() + index;
    std::copy_backward(it, end(), begin() + new_size);
    std::copy(first, last, it);

    _capacity = new_size;
    _size = new_size;
  }

  iterator insert(const_iterator pos, std::initializer_list<T> ilist) {
    insert(pos, ilist.begin(), ilist.end());
    return pos;
  }

  template<class... Args>
  iterator emplace(const_iterator pos, Args&&... args) {
    size_type new_size = _size + 1;
    size_type index = pos - begin();
    if (new_size > _capacity)
      reallocate(new_size);
    auto it = begin() + index;
    std::copy_backward(it, end(), _ptr + _size);

    std::allocator_traits<Allocator>::construct(
      _alloc, 
      _ptr + index, 
      std::forward<Args>(args)...
    );

    _capacity = new_size;
    _size = new_size;
    return it;
  }

  iterator erase(const_iterator pos) {
    std::allocator_traits<Allocator>::destroy(_alloc, &*pos);
    std::copy(pos + 1, end(), pos);
    _size--;
    return pos;
  }

  iterator erase(const_iterator first, const_iterator last) {
    for (auto it = first; it != last; it++)
      std::allocator_traits<Allocator>::destroy(_alloc, &*it);
    std::copy(last, end(), first);
    _size -= std::distance(first, last);
    return first;
  }

  void push_back(const T& value) {
    emplace_back(value);
  }

  void push_back(T&& value) {
    emplace_back(std::forward<T&&>(value));
  }

  template<class... Args>
  void emplace_back(Args&&... args) {
    size_type new_size = _size + 1;
    if (new_size > _capacity)
      reallocate(new_size);
    std::allocator_traits<Allocator>::construct(
      _alloc,
      _ptr + _size,
      std::forward<Args>(args)...
    );

    _size = new_size;
  }

  void pop_back() {
    erase(begin() + _size - 1);
  }

  void resize(size_type count) {
    resize(count, T());
  }

  void resize(size_type count, const value_type& value) {
    if (count < _size)
      for (auto it = begin() + count; it != end(); it++)
        std::allocator_traits<Allocator>::destroy(_alloc, &*it);
    else if (count > _size) {
      reallocate(count);
      for (auto it = end(); it != end() + count - _size; it++)
        std::allocator_traits<Allocator>::construct(
          _alloc, 
          _ptr + _size + (it - end()),
          value
        );
    }
    _size = count;
  }

  void swap(Vector& other) noexcept {
    std::swap(this->_size, other._size);
    std::swap(this->_capacity, other._capacity);
    std::swap(this->_ptr, other._ptr);
    std::swap(this->_alloc, other._alloc);
  }

private:
  void reallocate(size_type new_cap) {
    if (new_cap > max_size())
      throw std::length_error("New capacity over limit");
    pointer new_ptr = nullptr;
    new_ptr = std::allocator_traits<Allocator>::allocate(_alloc, new_cap);
    for (auto it = begin(); it != end(); it++)
      std::allocator_traits<Allocator>::construct(
        _alloc,
        new_ptr + (it - begin()),
        std::forward<T>(*it)
      );

    _capacity = new_cap;
    _ptr = new_ptr;
  }

  size_type _size;
  size_type _capacity;
  allocator_type _alloc;
  pointer _ptr;
};

template <class T, class Allocator>
bool operator==(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  if (lhs.size() != rhs.size()) 
    return false;
  for (size_t i = 0; i < lhs.size(); i++)
    if (lhs[i] != rhs[i]) 
      return false;
  return true;
}

template <class T, class Allocator>
bool operator!=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(lhs == rhs);
}

template <class T, class Allocator>
bool operator<(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

template <class T, class Allocator>
bool operator>(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return rhs < lhs;
}

template <class T, class Allocator>
bool operator>=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(lhs < rhs);
}

template <class T, class Allocator>
bool operator<=(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) {
  return !(rhs < lhs);
}

template <class T, class Allocator>
void swap(const Vector<T, Allocator>& lhs, const Vector<T, Allocator>& rhs) noexcept {
  lhs.swap(rhs);
}