#define CATCH_CONFIG_MAIN
#pragma warning(disable : 4996)  
#include "catch.hpp"
#include "vector.h"
#include <vector>
#include <string>

class NonCopy {
public:
  NonCopy(std::string name, int age) : _name(name), _age(age) {}
  NonCopy(const NonCopy&) = delete;
  NonCopy(NonCopy&&) = default;
  std::string getName() const {
    return _name;
  }
  int getAge() const {
    return _age;
  }
private:
  std::string _name = "default";
  int _age = 0;
};

class Person {
public:
  Person(std::string name, int age) : _name(name), _age(age) {}
  std::string getName() const {
    return _name;
  }
  int getAge() const {
    return _age;
  }
private:
  std::string _name = "default";
  int _age = 0;
};

TEST_CASE("Default constrctor. Empty vector") {
  size_t size = 0;
  Vector<int> vector;

  REQUIRE(vector.empty() == true);
  REQUIRE(vector.size() == size);
  REQUIRE(vector.max_size() == 1073741823);
  REQUIRE(vector.capacity() == size);
  REQUIRE_THROWS_AS(vector.at(size), std::out_of_range);
}

TEST_CASE("Constructor with number and value of objects") {
  size_t size = 4;
  double value = 28.563;
  Vector<double> vector(size, value);

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  REQUIRE(vector.max_size() == 536870911);
  REQUIRE(vector.capacity() == size);
  for (size_t i = 0; i < vector.size(); i++) {
    REQUIRE(vector.at(i) == value);
    REQUIRE(vector[i] == value);
  }
  REQUIRE_THROWS_AS(vector.at(size), std::out_of_range);
}

TEST_CASE("Constructor with number") {
  size_t size = 5;
  float value = 0.0;
  Vector<float> vector(size);

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  REQUIRE(vector.max_size() == 1073741823);
  REQUIRE(vector.capacity() == size);
  for (size_t i = 0; i < vector.size(); i++) {
    REQUIRE(vector.at(i) == value);
    REQUIRE(vector[i] == value);
  }
  REQUIRE_THROWS_AS(vector.at(size), std::out_of_range);
}

TEST_CASE("Iterators. Constructor with iterators") {
  size_t size = 5;
  int value = 4;
  Vector<int> src_vector(size, value);
  Vector<int> new_vector(src_vector.begin(), src_vector.end());

  REQUIRE(new_vector.empty() == false);
  REQUIRE(new_vector.size() == size);
  for (size_t i = 0; i < new_vector.size(); i++) {
    REQUIRE(new_vector.at(i) == value);
    REQUIRE(new_vector[i] == value);
  }
}

TEST_CASE("Copy constructor") {
  size_t size = 3;
  char value = '!';
  Vector<char> src_vector(size, value);
  Vector<char> new_vector(src_vector);

  REQUIRE(new_vector.empty() == false);
  REQUIRE(new_vector.size() == size);
  for (size_t i = 0; i < new_vector.size(); i++) {
    REQUIRE(new_vector.at(i) == value);
    REQUIRE(new_vector[i] == value);
  }
}

TEST_CASE("Move constructor") {
  size_t size = 3;
  Vector<int> vector(std::move(Vector<int>{9, 0, 111}));

  std::vector<int> expec_vec = { 9, 0, 111};

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  for (size_t i = 0; i < vector.size(); i++) {
    REQUIRE(vector.at(i) == expec_vec.at(i));
    REQUIRE(vector[i] == expec_vec[i]);
  }
  REQUIRE(vector.front() == 9);
  REQUIRE(vector.back() == 111);
}

TEST_CASE("Constructor with init list") {
  size_t size = 5;
  Vector<int> vector({ 9, -8, 0, 101, 1 });

  std::vector<int> expec_vec = { 9, -8, 0, 101, 1 };

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  for (size_t i = 0; i < vector.size(); i++) {
    REQUIRE(vector.at(i) == expec_vec.at(i));
    REQUIRE(vector[i] == expec_vec[i]);
  }
  REQUIRE(vector.front() == 9);
  REQUIRE(vector.back() == 1);

  int* ptr = &vector[0];
  REQUIRE(vector.data() == ptr);
}

TEST_CASE("Const vector") {
  size_t size = 3;
  const Vector<double> vector = { 9.5, 36.6, -3.14 };

  std::vector<double> expec_vec = { 9.5, 36.6, -3.14 };

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  size_t i = 0;
  for (auto it = vector.begin(); it != vector.end(); it++, i++)
    REQUIRE(*it == expec_vec[i]);
  
  REQUIRE(vector.front() == vector.at(0));
  REQUIRE(vector.back() == vector.at(size - 1));

  const double* ptr = &vector[0];
  REQUIRE(vector.data() == ptr);
}

TEST_CASE("Cbegin and cend") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };

  std::vector<double> expec_vec = { 9.5, 36.6, -3.14 };

  size_t i = 0;
  for (auto it = vector.cbegin(); it != vector.cend() || i < expec_vec.size(); it++, i++)
    REQUIRE(*it == expec_vec[i]);
}

TEST_CASE("Reverse iterators") {
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  Vector<double> new_vector(vector.rbegin(), vector.rend());

  std::vector<double> expec_vec = { -3.14, 36.6, 9.5 };
  for (size_t i = 0; i < new_vector.size(); i++)
    REQUIRE(new_vector[i] == expec_vec[i]);

  size_t i = 0;
  for (auto it = vector.crbegin(); it != vector.crend(); it++, i++)
    REQUIRE(*it == expec_vec[i]);

  const Vector<double> const_vector = { 9.5, 36.6, -3.14 };
  i = 0;
  for (auto it = const_vector.rbegin(); it != const_vector.rend(); it++, i++)
    REQUIRE(*it == expec_vec[i]);
}

TEST_CASE("Reserve and shrink_to_fit") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  
  size_t new_cap = vector.capacity() * 2;
  vector.reserve(new_cap);

  std::vector<double> expec_vec = { 9.5, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  REQUIRE(vector.capacity() == new_cap);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);

  vector.shrink_to_fit();
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size);
  REQUIRE(vector.capacity() == size);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);

  REQUIRE_THROWS_AS(vector.reserve(vector.max_size() + 1), std::length_error);

  vector.reserve(vector.capacity() - 1);
  REQUIRE(vector.capacity() == expec_vec.capacity());
}

TEST_CASE("Clear") {
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  double* ptr = vector.data();
  size_t cap = vector.capacity();

  vector.clear();
  REQUIRE(vector.empty() == true);
  REQUIRE(vector.size() == 0);
  REQUIRE(vector.capacity() == cap);
  REQUIRE(vector.data() == ptr);
}

TEST_CASE("Insert with iterator pos") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  auto it = vector.begin() + 1;
  vector.insert(it, 78.88);

  std::vector<double> expec_vec = { 9.5, 78.88, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size + 1);
  REQUIRE(vector.capacity() == size + 1);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);
}

TEST_CASE("Insert with iterator pos and rhs") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  auto it = vector.begin() + 1;
  vector.insert(it, std::move(78.88));

  std::vector<double> expec_vec = { 9.5, 78.88, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size + 1);
  REQUIRE(vector.capacity() == size + 1);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);
}

TEST_CASE("Insert with iterator pos and count") {
  size_t size = 3;
  size_t count = 3;
  double value = 78.88;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  auto it = vector.begin() + 1;
  vector.insert(it, count, value);

  std::vector<double> expec_vec = { 9.5, 78.88, 78.88, 78.88, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size + count);
  REQUIRE(vector.capacity() == size + count);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);
}

TEST_CASE("Insert with iterators") {
  size_t size = 3;
  size_t count = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  auto it = vector.begin() + 1;

  Vector<double> inserted_vector = { 28, 34.1, -19 };
  vector.insert(it, inserted_vector.begin(), inserted_vector.end());

  std::vector<double> expec_vec = { 9.5, 28, 34.1, -19, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size + count);
  REQUIRE(vector.capacity() == size + count);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);
}

TEST_CASE("Insert with ilist") {
  size_t size = 3;
  size_t count = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  auto it = vector.begin() + 1;

  vector.insert(it, { 28, 34.1, -19 });

  std::vector<double> expec_vec = { 9.5, 28, 34.1, -19, 36.6, -3.14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == size + count);
  REQUIRE(vector.capacity() == size + count);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expec_vec[i]);
}

TEST_CASE("Emplace") {
  Vector<int> vector = { 4, 6, 8 };
  vector.emplace(vector.begin() + 2, 9);
  
  std::vector<int> expected_vector = { 4, 6, 9, 8 };
  REQUIRE(vector.size() == 4);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Operator= copy") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };

  Vector<double> new_vector;
  new_vector = vector;

  REQUIRE(new_vector.empty() == false);
  REQUIRE(new_vector.size() == vector.size());
  REQUIRE(vector.capacity() == vector.capacity());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(new_vector[i] == vector[i]);
}

TEST_CASE("Operator= move") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  vector = Vector<double>{3.14, 56.6, 32, 4};
  std::vector<double> expected_vector = { 3.14, 56.6, 32, 4 };

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == expected_vector.size());
  REQUIRE(vector.capacity() == expected_vector.capacity());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Operator= ilist") {
  size_t size = 3;
  Vector<double> vector = { 9.5, 36.6, -3.14 };
  vector = { 3.14, 56.6, 32, 4 };
  std::vector<double> expected_vector = { 3.14, 56.6, 32, 4 };

  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == expected_vector.size());
  REQUIRE(vector.capacity() == expected_vector.capacity());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Assign count and value") {
  Vector<int> vector = {9, 8, 1};
  vector.assign(4, 1);

  std::vector<int> expected_vector(4, 1);
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == expected_vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Assign iterators") {
  Vector<int> vector = { 9, 8, 1 };
  Vector<int> src_vector = { 5, 6, 7 };
  vector.assign(src_vector.begin() + 1, src_vector.end());

  std::vector<int> expected_vector = { 6, 7 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == expected_vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Assign ilist") {
  Vector<int> vector = { 9, 8, 1 };
  vector.assign({3, 8, 9, 14});

  std::vector<int> expected_vector = { 3, 8, 9, 14 };
  REQUIRE(vector.empty() == false);
  REQUIRE(vector.size() == expected_vector.size());
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Emplace back") {
  std::string name = "emplaced";
  Vector<NonCopy> vector;
  vector.emplace_back(NonCopy(name, 10));
  vector.emplace_back(name, 10);

  REQUIRE(vector.size() == 2);
  REQUIRE(vector[0].getName() == "emplaced");
  REQUIRE(vector[0].getAge() == 10);
}

TEST_CASE("Erase") {
  Vector<double> vector = {3.14, 88.88, 0};
  vector.erase(vector.end() - 1);

  std::vector<double> expected_vector = { 3.14, 88.88 };
  REQUIRE(vector.size() == 2);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Erase with iterators") {
  Vector<double> vector = { 3.14, 88.88, 99.017, -555, 0 };
  vector.erase(vector.begin() + 1, vector.end() - 1);

  std::vector<double> expected_vector = { 3.14, 0 };
  REQUIRE(vector.size() == 2);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Push back and pop back") {
  Vector<double> vector = { 3.14, 88.88, 99.017, -555, 0 };
  vector.pop_back();
  double new_value = 16.99;
  vector.push_back(new_value);

  std::vector<double> expected_vector = { 3.14, 88.88, 99.017, -555, 16.99 };
  REQUIRE(vector.size() == 5);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Push back rhs") {
  std::string name = "emplaced";
  Vector<NonCopy> vector;
  //std::vector<NonCopy> vector;
  vector.push_back(NonCopy(name, 10));
  
  REQUIRE(vector.size() == 1);
  REQUIRE(vector[0].getName() == "emplaced");
  REQUIRE(vector[0].getAge() == 10);
}

TEST_CASE("Resize") {
  Vector<int> vector = { 2, 4, 6 };
  vector.resize(6);

  std::vector<int> expected_vector = { 2, 4, 6, 0, 0, 0 };
  REQUIRE(vector.size() == 6);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);

  vector.resize(2);
  expected_vector = { 2, 4 };
  REQUIRE(vector.size() == 2);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);

  vector.resize(4, 9);
  expected_vector = { 2, 4, 9, 9 };
  REQUIRE(vector.size() == 4);
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Swap") {
  Vector<int> odd_vector = { 2, 4, 6 };
  Vector<int> no_odd_vector = { 1, 3, 5 };

  odd_vector.swap(no_odd_vector);
  for (size_t i = 0; i < odd_vector.size(); i++) {
    REQUIRE(odd_vector[i] % 2 == 1);
    REQUIRE(no_odd_vector[i] % 2 == 0);
  }
}

TEST_CASE("Sort") {
  Vector<int> vector = { 12, 3, 5, 0, -45, 4 , 3};
  std::sort(vector.begin(), vector.end());

  std::vector<int> expected_vector = { -45, 0, 3, 3, 4, 5, 12 };
  for (size_t i = 0; i < vector.size(); i++)
    REQUIRE(vector[i] == expected_vector[i]);
}

TEST_CASE("Using custom iterators for std::vector") {
  SECTION("Constructor") {
    Vector<int> my_vector = { 12, 3, 5, 0, -45, 4, 3 };
    std::vector<int> std_vector(my_vector.begin() + 1, my_vector.end() - 1);

    std::vector<int> expected_vector = { 3, 5, 0, -45, 4, 3 };
    for (size_t i = 0; i < std_vector.size(); i++)
      REQUIRE(std_vector[i] == expected_vector[i]);
  }

  SECTION("Assign") {
    Vector<double> my_vector = { 55.5, 89.5, 23.5 };
    std::vector<double> std_vector = { 3.14, 96.99, 88, 78, 0 };
    std_vector.assign(my_vector.begin() + 1, my_vector.end());

    std::vector<double> expected_vector = { 89.5, 23.5 };
    for (size_t i = 0; i < std_vector.size(); i++)
      REQUIRE(std_vector[i] == expected_vector[i]);
  }

  SECTION("Insert") {
    std::vector<std::string> std_vector = { "Hello", ",", "World", "!" };
    Vector<std::string> my_vector = { "insane", "and", "lonely" };
    std_vector.insert(std_vector.begin() + 1, my_vector.begin(), my_vector.end());

    std::vector<std::string> expected_vector = { "Hello", "insane", "and", "lonely", ",", "World", "!"  };
    for (size_t i = 0; i < std_vector.size(); i++)
      REQUIRE(std_vector[i] == expected_vector[i]);

  }
}

TEST_CASE("Iterator") {
  Vector<int> vector{1, 2, 3, 4, 5 ,6};
  auto first = vector.begin();
  auto last = vector.end();
  REQUIRE(first < last);

  first++;
  REQUIRE(last > first);

  last--;
  REQUIRE(last >= first);
  
  first++;
  REQUIRE(first <= last);
  
  REQUIRE(first != last);

  first += 1;
  last -= 2;
  REQUIRE(first == last);
}

TEST_CASE("Vector's compare") {
  SECTION("EQUAL") {
    Vector<int> first = { 5, 6, 8, 9 };
    Vector<int> second = { 5, 6, 8, 9 };
    Vector<int> thrid = { 1, 2, 3};
    REQUIRE(first == second);
    REQUIRE((first == thrid) == false);
  }

  SECTION("Not equal") {
    Vector<int> first = { 5, 6, 8, 9 };
    Vector<int> second = { 1, 2, 3, 4 };
    REQUIRE(first != second);
  }

  SECTION("Less") {
    Vector<int> first = { 1, 2, 3, 4 };
    Vector<int> second = { 5, 6, 8, 9 };
    REQUIRE(first < second);
  }

  SECTION("More") {
    Vector<int> first = { 5, 6, 8, 9 };
    Vector<int> second = { 1, 2, 3, 4 };
    REQUIRE(first > second);
  }

  SECTION("Less or equal") {
    Vector<int> first = { 1, 2, 3, 4 };
    Vector<int> second = { 5, 6, 8, 9 };
    Vector<int> thrid = { 1, 2, 3, 4 };
    REQUIRE(first <= second);
    REQUIRE(first <= thrid);
  }

  SECTION("More or equal") {
    Vector<int> first = { 5, 6, 8, 9 };
    Vector<int> second = { 1, 2, 3, 4 };
    Vector<int> thrid = { 5, 6, 8, 9 };
    REQUIRE(first >= second);
    REQUIRE(first >= thrid);
  }
}

TEST_CASE("Swap non member") {
  Vector<int> odd_vector = { 2, 4, 6 };
  Vector<int> no_odd_vector = { 1, 3, 5 };

  swap(odd_vector, no_odd_vector);
  for (size_t i = 0; i < odd_vector.size(); i++) {
    REQUIRE(odd_vector[i] % 2 == 1);
    REQUIRE(no_odd_vector[i] % 2 == 0);
  }
}