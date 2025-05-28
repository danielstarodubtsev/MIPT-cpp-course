//! If you store your BigInteger as a vector of digits, take a moment to read the comment below.

// When running these tests, it is recommended to use a small base for your BigInteger (as long as you can change it easily).
// This will increase the chances of finding an error in case one exists.
// However, generally, using a larger base is preferrable due to better performance.

#include "biginteger.h"

#include <cassert>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

template <typename A, typename B>
void ASSERT_EQUAL(A a, B b) {
  if (a != b) {
    std::cout << a << " is not equal to " << b << std::endl;
    std::exit(EXIT_FAILURE);
  }
}

void TestBigintegerInitialization() {
  BigInteger a;
  assert(a == 0_bi);

  BigInteger b(0);
  BigInteger c(12345);
  BigInteger d(-67890);
  assert(b == 0_bi);
  assert(c == 12345_bi);
  assert(d == -67890_bi);

  BigInteger e("0");
  BigInteger f("12345678901234567890");
  BigInteger g("-12345678901234567890");
  assert(e == 0_bi);
  assert(f == "12345678901234567890"_bi);
  assert(g == "-12345678901234567890"_bi);

  BigInteger h = f;
  BigInteger i(g);
  assert(h == f);
  assert(i == g);

  a = d;
  b = 0_bi;
  assert(a == -67890_bi);
  assert(b == 0_bi);

  std::cout << "TestBigintegerInitialization passed!" << std::endl;
}

void TestComparisonOperators() {
  assert(0_bi == 0_bi);
  assert(123_bi == 123_bi);
  assert(-456_bi == -456_bi);
  assert(0_bi == -0_bi);

  assert(123_bi != 124_bi);
  assert(-123_bi != 123_bi);
  assert(0_bi != 1_bi);

  assert(100_bi > 99_bi);
  assert(-99_bi > -100_bi);
  assert(0_bi > -1_bi);
  assert(-1_bi < 0_bi);
  assert(1000_bi < "1000000000000"_bi);

  assert(100_bi >= 100_bi);
  assert(100_bi >= 99_bi);
  assert(-100_bi <= -100_bi);
  assert(-100_bi <= -99_bi);

  std::cout << "TestComparisonOperators passed!" << std::endl;
}

void TestArithmetic() {
  assert(0_bi + 0_bi == 0_bi);
  assert(100_bi + 200_bi == 300_bi);
  assert(-100_bi + 50_bi == -50_bi);
  assert(100_bi + (-200_bi) == -100_bi);
  assert("10000000000000000000"_bi + "10000000000000000000"_bi == "20000000000000000000"_bi);

  assert(0_bi - 0_bi == 0_bi);
  assert(100_bi - 50_bi == 50_bi);
  assert(50_bi - 100_bi == -50_bi);
  assert(-100_bi - (-200_bi) == 100_bi);
  assert("20000000000000000000"_bi - "10000000000000000000"_bi == "10000000000000000000"_bi);

  assert(-0_bi * 12345_bi == 0_bi);
  assert(0_bi * 12345_bi == 0_bi);
  assert(1_bi * 98765_bi == 98765_bi);
  assert(-1_bi * 98765_bi == -98765_bi);
  assert(123_bi * 456_bi == 56088_bi);
  assert(-123_bi * 456_bi == -56088_bi);
  assert(-123_bi * -456_bi == 56088_bi);
  assert("10000000000"_bi * "10000000000"_bi == "100000000000000000000"_bi);

  assert(0_bi / 123_bi == 0_bi);
  assert(-0_bi / 123_bi == 0_bi);
  assert(100_bi / 50_bi == 2_bi);
  assert(99_bi / 50_bi == 1_bi);
  assert(100_bi / -50_bi == -2_bi);
  assert(-100_bi / -50_bi == 2_bi);
  assert("100000000000000000000"_bi / "10000000000"_bi == "10000000000"_bi);

  assert(0_bi % 123_bi == 0_bi);
  assert(-0_bi % 123_bi == 0_bi);
  assert(100_bi % 3_bi == 1_bi);
  assert(100_bi % -3_bi == 1_bi);
  assert(-100_bi % 3_bi == -1_bi);
  assert(-100_bi % -3_bi == -1_bi);
  assert("100000000000000000001"_bi % "10000000000"_bi == 1_bi);

  assert(-(0_bi) == 0_bi);
  assert(-(123_bi) == -123_bi);
  assert(-(-123_bi) == 123_bi);

  BigInteger a = 0_bi;
  assert(a++ == 0_bi); assert(a == 1_bi);
  assert(++a == 2_bi); assert(a == 2_bi);
  assert(a-- == 2_bi); assert(a == 1_bi);
  assert(--a == 0_bi); assert(a == 0_bi);

  BigInteger b = 10_bi;
  b += 5_bi; assert(b == 15_bi);
  b -= 3_bi; assert(b == 12_bi);
  b *= 2_bi; assert(b == 24_bi);
  b /= 3_bi; assert(b == 8_bi);
  b %= 5_bi; assert(b == 3_bi);

  std::cout << "TestArithmetic passed!" << std::endl;
}

void StressTestArithmetic() {
  for (long long num_size : std::vector<int>{10, 100, 10000, 1000000000}) {
    for (int iter = 0; iter < 5000; ++iter) {
      unsigned long long first = rand() % num_size;
      unsigned long long second = rand() % num_size;

      BigInteger first_big = first;
      BigInteger second_big = second;

      ASSERT_EQUAL(first_big + second_big, second_big + first_big);
      ASSERT_EQUAL(BigInteger(first + second), first_big + second_big);

      ASSERT_EQUAL(first_big - second_big, -(second_big - first_big));
      if (first >= second) {
        ASSERT_EQUAL(BigInteger(first - second), first_big - second_big);
      }

      ASSERT_EQUAL(first_big * second_big, second_big * first_big);
      ASSERT_EQUAL(BigInteger(first * second), first_big * second_big);

      if (second != 0) {
        ASSERT_EQUAL(BigInteger(first / second), first_big / second_big);
        ASSERT_EQUAL(BigInteger(first % second), first_big % second_big);
      }
    }
  }

  std::cout << "StressTestArithmetic passed!" << std::endl;
}

void TestIO() {
  assert("12345678901234567890"_bi.toString() == "12345678901234567890");

  std::ostringstream oss;
  oss << 0_bi; assert(oss.str() == "0"); oss.str("");
  oss << 123_bi; assert(oss.str() == "123"); oss.str("");
  oss << -456_bi; assert(oss.str() == "-456"); oss.str("");
  oss << "12345678901234567890"_bi; 
  assert(oss.str() == "12345678901234567890");

  std::istringstream iss;
  BigInteger a, b, c;
  iss.str("0"); iss >> a; assert(a == 0_bi);
  iss.clear(); iss.str("12345"); iss >> b; assert(b == 12345_bi);
  iss.clear(); iss.str("-67890"); iss >> c; assert(c == -67890_bi);
  iss.clear(); iss.str("   9876543210   "); iss >> a; 
  assert(a == 9876543210_bi);

  BigInteger d;
  iss.clear(); iss.str("12345678901234567890 8765432109876543210");
  iss >> a >> b;
  oss.str(""); oss << a; assert(oss.str() == "12345678901234567890");
  oss.str(""); oss << b; assert(oss.str() == "8765432109876543210");
    
  std::cout << "TestIO passed!" << std::endl;
}

void TestLargeArithmetic() {
  BigInteger a = "123456789012345678901234567890"_bi;
  BigInteger b = "987654321098765432109876543210"_bi;

  BigInteger sum = a + b;
  assert(sum == "1111111110111111111011111111100"_bi);

  BigInteger diff = b - a;
  assert(diff == "864197532086419753208641975320"_bi);

  BigInteger product = a * b;
  assert(product == "121932631137021795226185032733622923332237463801111263526900"_bi);

  BigInteger div = b / a;
  assert(div == 8_bi);

  BigInteger mod = b % a;
  assert(mod == "987654321098765432109876543210"_bi - "123456789012345678901234567890"_bi * 8_bi);

  BigInteger x = 1_bi;

  for(int i = 0; i < 1000; i++) {
    x *= 2_bi;
  }

  std::string x_str = x.toString();
  assert(x_str.size() == 302);
  assert(x_str.substr(0, 10) == "1071508607");

  std::cout << "TestLargeArithmetic passed!" << std::endl;
}

void TestBoolOperator() {
  assert(123_bi);
  assert(-1_bi);
  assert(1000000000_bi);
  assert(5_bi);

  assert(!0_bi);
  assert(!-0_bi);

  std::cout << "TestBoolOperator passed!" << std::endl;
}

int main() {
  std::srand(std::time(0));

  TestBigintegerInitialization();
  TestComparisonOperators();
  TestArithmetic();
  StressTestArithmetic();
  TestIO();
  TestLargeArithmetic();
  TestBoolOperator();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}