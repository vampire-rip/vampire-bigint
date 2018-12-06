
/*
 * Vampire Big Integer Lib -- Bright
 * You may (or may not) find a update version at:
 *  https://github.com/vampire-rip/vampire-bigint
 * License: MIT or whatever you like, as long as if you make some improvement on it, inform me.
 */

#ifndef BRIGHT_H
#define BRIGHT_H

#include <iostream>
#include <vector>
#include <cctype>
#include <algorithm>
#include <string>
#include <bitset>
#include <sstream>
#include <iomanip>

using namespace std;

class BigInt {
    typedef vector<unsigned long long> vector_t;
    vector_t v;
    int sign;

public:
    const static unsigned int base = 0x40000000;
    const static unsigned int mask = 0x3FFFFFFF;
    const static unsigned int base2 = 30;
    BigInt();
    BigInt(const long long& v);
    BigInt(const vector_t& v);
    BigInt(const string &s, int base = 10);
    BigInt(const char* &s, int base = 10);

    BigInt& operator=(const BigInt &b);
    BigInt& operator=(long long num);

    BigInt operator+(const BigInt &b) const;
    BigInt operator-(const BigInt &b) const;
    BigInt operator*(int b) const;
    BigInt operator*(const BigInt &b) const;
    BigInt operator/(int b) const;
    BigInt operator/(const BigInt &b) const;
    BigInt operator%(const BigInt &b) const;
    BigInt operator<<(const size_t &b) const;
    BigInt operator>>(const size_t &b) const;

    BigInt& operator+=(const BigInt &b);
    BigInt& operator-=(const BigInt &b);
    BigInt& operator*=(int b);
    BigInt& operator*=(const BigInt &b);
    BigInt& operator/=(int b);
    BigInt& operator/=(const BigInt &b);
    BigInt& operator%=(const BigInt &b);
    BigInt& operator<<=(const size_t &b);
    BigInt& operator>>=(const size_t &b);

    unsigned int shift();
    BigInt& unshift(unsigned int b);

    bool operator<(const BigInt &b) const;
    bool operator>(const BigInt &b) const;
    bool operator<=(const BigInt &b) const;
    bool operator>=(const BigInt &b) const;
    bool operator==(const BigInt &b) const;
    bool operator!=(const BigInt &b) const;

    BigInt operator-() const;

    BigInt abs() const;

    size_t size() const;
    size_t vector_size() const;

    static BigInt fromStr(const string &s, int base);
    string toStr(int base) const;

private:
    BigInt& trim();

    static vector_t _fromBase2Str(const string &s);
    static vector_t _fromBase10Str(const string &s);

    static char _base16ToChar(unsigned short t);
    static char _base8ToChar(unsigned short t);

// friends:
    friend std::pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1);
    friend ostream &operator<<(ostream &os, const BigInt &b);
    friend istream &operator>>(istream &is, BigInt &b);
};

typedef BigInt Bright;

class ErrorType {
public:
    ErrorType() = delete;
};

#define BigInt ErrorType
#endif // BRIGHT_H

