#include "bright.h"
#include <iomanip>

#undef BigInt

BigInt::BigInt() : sign(1) {

}

BigInt::BigInt(const long long &v) : sign(1) {
    *this = v;
}

BigInt::BigInt(const BigInt::vector_t &v) : sign(1)
{
    this->v = v;
}

BigInt::BigInt(const string &s, int _base) : sign(1)
{
    *this = fromStr(s, _base);
}

BigInt::BigInt(const char *&s, int _base) : sign(1)
{
    *this = fromStr(s, _base);
}

BigInt& BigInt::operator=(const BigInt &b)
{
    this->sign = b.sign;
    this->v = b.v;
    return *this;
}

BigInt& BigInt::operator=(long long num) {
    this->sign = 1;
    this->v.clear();
    if (num < 0) {
        this->sign = -1;
        num = -num;
    }
    for (; num > 0; num = num / base)
        v.push_back(num % base);
    return *this;
}

BigInt BigInt::operator+(const BigInt &b) const
{
    if (sign != b.sign)
        return *this - (-b);
    BigInt res = b;
    size_t q = std::max(vector_size(), b.vector_size());
    unsigned int carry = 0;
    for (unsigned int i = 0; i < q || carry; ++i) {
        if (i == res.vector_size()) res.v.push_back(0);
        res.v[i] += carry + (i < vector_size() ? v[i] : 0);
        carry = res.v[i] >= base;
        if (carry) res.v[i] -= base;
    }
    return res;
}

BigInt BigInt::operator-(const BigInt &b) const
{
    if (sign != b.sign)
        return *this + (-b);
    if (abs() >= b.abs()) {
        BigInt res = *this;
        unsigned int carry = 0;
        for (size_t i = 0; i < b.vector_size() || carry; ++i) {
            unsigned int temp = carry + (i < b.vector_size() ? static_cast<unsigned int>(b.v[i]) : 0);
            if (res.v[i] >= temp) {
                res.v[i] -= temp;
                carry = 0;
            }
            else {
                res.v[i] = res.v[i] - temp + base;
                carry = 1;
            }
        }
        res.trim();
        return res;
    }
    return -(b - *this);
}

BigInt BigInt::operator*(int b) const
{
    BigInt res = *this;
    return res *= b;
}

BigInt BigInt::operator*(const BigInt &b) const
{
    // " << 1 " is of the same complexity as " << i "
    BigInt res(0);
    for (unsigned int i = 0; i < b.vector_size(); i++) {
        res += [&] {
            BigInt t = *this * static_cast<int>(b.v[i]);
            t.sign = 1;
            t <<= i;
            return t;
        }();
    }
    res.sign = sign * b.sign;
    return res;
}

BigInt BigInt::operator/(int _b) const
{
    BigInt res = *this;
    if (_b < 0) {
        res.sign = -res.sign;
        _b = -_b;
    }
    auto b = static_cast<unsigned int>(_b);
    unsigned long long rem = 0;
    for (size_t i = v.size() - 1; i < v.size(); --i) {
        unsigned long long cur = v[i] + rem * base;
        res.v[i] = static_cast<unsigned int>(cur / b);
        rem = static_cast<unsigned int>(cur % b);
    }
    res.trim();
    return res;
}

BigInt BigInt::operator/(const BigInt &b) const
{
    return divmod(*this, b).first;
}

unsigned long long BigInt::operator%(unsigned long long b) const
{
    unsigned long long m = 0;
    for (size_t i = v.size() - 1; i < v.size(); --i)
        m = (v[i] + m * base) % b;
    return m;
}

BigInt BigInt::operator%(const BigInt &b) const
{
    return divmod(*this, b).second;
}

BigInt BigInt::operator>>(const unsigned int &b) const
{
    BigInt res = *this;
    res.v.erase(res.v.begin(), res.v.begin() + static_cast<long long>(b));
    return res;
}

BigInt BigInt::operator<<(const unsigned int &b) const
{
    BigInt res = *this;
    vector_t t(b);
    res.v.insert(res.v.begin(), t.begin(), t.end());
    return res;
}

BigInt &BigInt::operator+=(const BigInt &b)
{
    *this = *this + b;
    return *this;
}

BigInt &BigInt::operator-=(const BigInt &b)
{
    *this = *this - b;
    return *this;
}

BigInt &BigInt::operator*=(int b)
{
    BigInt& res = *this;
    if (b < 0) {
        res.sign *= -1;
        b = -b;
    }
    if (b == base) {
        res.v.insert(res.v.begin(), 0);
        return res;
    }
    unsigned long long carry = 0;
    for (size_t i = 0; i < res.vector_size() || carry; ++i) {
        if (i == vector_size()) res.v.push_back(0);
        unsigned long long cur
            = static_cast<unsigned long long>(b) * res.v[i] + carry;
        carry = cur / base;
        res.v[i] = cur % base;
    }
    res.trim();
    return res;
}

BigInt &BigInt::operator*=(const BigInt &b)
{
    *this = *this * b;
    return *this;
}

BigInt &BigInt::operator/=(int b)
{
    *this = *this / b;
    return *this;
}

BigInt &BigInt::operator/=(const BigInt &b)
{
    *this = *this / b;
    return *this;
}

BigInt &BigInt::operator%=(const BigInt &b)
{
    *this = *this % b;
    return *this;
}

BigInt &BigInt::operator<<=(const unsigned int &b)
{
    *this = *this << b;
    return *this;
}

BigInt &BigInt::operator>>=(const unsigned int &b)
{
    *this = *this >> b;
    return *this;
}

unsigned int BigInt::shift()
{
    if (v.empty()) return 0u;
    unsigned int i = static_cast<unsigned int>(v.front());
    v.erase(v.begin());
    return i;
}

BigInt &BigInt::unshift(unsigned int b)
{
    v.insert(v.begin(), b & mask);
    return *this;
}


bool BigInt::operator<(const BigInt &b) const
{
    if (sign != b.sign)
        return sign < b.sign;
    if (vector_size() != b.vector_size()) {
        return sign > 0
            ? vector_size() < b.vector_size()
            : vector_size() > b.vector_size();
    }
    for (size_t i = vector_size() - 1; i < vector_size(); i--) {
        if (v[i] != b.v[i]) {
            return sign > 0
                ? v[i] < b.v[i]
                : v[i] > b.v[i];
        }
    }
    return false;
}

bool BigInt::operator>(const BigInt &b) const
{
    return b < *this;
}

bool BigInt::operator<=(const BigInt &b) const
{
    return !(b < *this);
}

bool BigInt::operator>=(const BigInt &b) const
{
    return !(*this < b);
}

bool BigInt::operator==(const BigInt &b) const
{
    if (sign != b.sign) return false;
    if (vector_size() != b.vector_size())
        return false;
    for (size_t i = vector_size() - 1; i < vector_size(); i--)
        if (v[i] != b.v[i])
            return false;
    return true;
}

bool BigInt::operator!=(const BigInt &b) const
{
    return !(*this == b);
}

BigInt BigInt::operator-() const
{
    BigInt res = *this;
    res.sign = -sign;
    return res;
}

BigInt BigInt::abs() const
{
    BigInt res = *this;
    res.sign = 1;
    return res;
}

BigInt &BigInt::trim()
{
    while (!v.empty() && !v.back()) v.pop_back();
    if (v.empty()) sign = 1;
    return *this;
}

size_t BigInt::size() const
{
    if (v.empty()) return 0;
    size_t b = 0;
    auto i = static_cast<unsigned int>(v.back());
    while (i != 0) {
        b++;
        i >>= 1;
    }
    return (v.size() - 1) * base2 + b;
}

inline size_t BigInt::vector_size() const
{
    return this->v.size();
}

BigInt BigInt::fromStr(const string &s, int _base)
{
    BigInt res;
    size_t pos = 0;
    while (pos != s.size() && (s[pos] == '-' || s[pos] == '+')) {
        if (s[pos] == '-')
            res.sign = -res.sign;
        ++pos;
    }
    if (_base == 10) {
        res.v = _fromBase10Str(s.substr(pos));
        return res;
    }
    if (_base == 2) {
        res.v = _fromBase2Str(s.substr(pos));
        return res;
    }
    string temp;
    if (_base == 16) {
        for (; pos != s.size(); ++pos) {
            switch (s[pos]) {
            case '0': temp += "0000"; break;
            case '1': temp += "0001"; break;
            case '2': temp += "0010"; break;
            case '3': temp += "0011"; break;
            case '4': temp += "0100"; break;
            case '5': temp += "0101"; break;
            case '6': temp += "0110"; break;
            case '7': temp += "0111"; break;
            case '8': temp += "1000"; break;
            case '9': temp += "1001"; break;
            case 'A': temp += "1010"; break;
            case 'B': temp += "1011"; break;
            case 'C': temp += "1100"; break;
            case 'D': temp += "1101"; break;
            case 'E': temp += "1110"; break;
            case 'F': temp += "1111"; break;
            default: temp += s[pos]; break;
            }
        }
    }
    else if (_base == 8) {
        for (; pos != s.size(); ++pos) {
            switch (s[pos]) {
            case '0': temp += "000"; break;
            case '1': temp += "001"; break;
            case '2': temp += "010"; break;
            case '3': temp += "011"; break;
            case '4': temp += "100"; break;
            case '5': temp += "101"; break;
            case '6': temp += "110"; break;
            case '7': temp += "111"; break;
            default: temp += s[pos]; break;
            }
        }
    }
    res.v = _fromBase2Str(temp);
    return res;
}

string BigInt::toStr(int _base) const
{
    if (vector_size() == 0) return "0";
    string _sign = this->sign == 1 ? "" : "-";

    if (_base == 10) {
        string res;
        BigInt a = *this, b;
        a.sign = 1;
        while (a != 0) {
            auto t = divmod(a, 10);
            a = t.first;
            res += '0' + static_cast<char>(t.second.shift());
        }
        std::reverse(res.begin(), res.end());
        return _sign + res;
    }

    string base2str;

    for (auto i = v.rbegin(); i != v.rend(); ++i) {
        base2str += bitset<base2>(*i).to_string();
    }

    size_t _begin = 0;
    while (base2str[_begin] == '0') _begin++;
    base2str.erase(0, _begin);

    if (_base == 2)
        return _sign + base2str;

    string res;
    unsigned int b = 0;
    unsigned short temp = 0;

    if (_base == 16) {
        for (auto i = base2str.rbegin(); i != base2str.rend(); ++i) {
            switch (*i) {
            case '0': break;
            case '1': temp += 1 << b; break;
            }
            ++b;
            if (b == 4) {
                res += _base16ToChar(temp);
                temp = 0;
                b = 0;
            }
        }
        if (temp != 0)
            res += _base16ToChar(temp);
    }
    else if (_base == 8) {
        for (auto i = base2str.rbegin(); i != base2str.rend(); ++i) {
            switch (*i) {
            case '0': break;
            case '1': temp += 1 << b; break;
            }
            ++b;
            if (b == 3) {
                res += _base8ToChar(temp);
                temp = 0;
                b = 0;
            }
        }
        if (temp != 0)
            res += _base8ToChar(temp);
    }
    std::reverse(res.begin(), res.end());
    return _sign + res;
}

BigInt::vector_t BigInt::_fromBase2Str(const string &s)
{
    vector_t v;
    unsigned int b = 0;
    unsigned int temp = 0;
    for (auto i = s.rbegin(); i != s.rend(); ++i) {
        switch (*i) {
        case '0': break;
        case '1': temp += 1 << b; break;
        default:
            throw std::invalid_argument("wrong base");
        }
        ++b;
        if (b == base2) {
            v.push_back(temp);
            temp = 0;
            b = 0;
        }
    }
    if (temp != 0)
        v.push_back(temp);
    return v;
}

BigInt::vector_t BigInt::_fromBase10Str(const string &s)
{
    BigInt tmp;
    for (auto i = s.begin(); i != s.end(); i++) {
        int what = (*i - '0');
        if (what < 0 || what > 9)
            throw std::invalid_argument("wrong base");
        tmp = tmp * 10 + what;
    }
    return tmp.v;
}

inline char BigInt::_base16ToChar(unsigned short t)
{
    switch (t) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    }
    throw std::invalid_argument("wrong base");
}

inline char BigInt::_base8ToChar(unsigned short t)
{
    switch (t) {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    }
    throw std::invalid_argument("wrong base");
}

std::pair<BigInt, BigInt> divmod(const BigInt &a1, const BigInt &b1) {
    unsigned int norm = BigInt::base / static_cast<unsigned int>(b1.v.back() + 1);
    BigInt a = a1.abs() * norm;
    BigInt b = b1.abs() * norm;
    BigInt q, r;
    q.v.resize(a.vector_size());

    for (size_t i = a.vector_size() - 1; i < a.vector_size(); i--) {
        r *= BigInt::base;
        r += static_cast<unsigned int>(a.v[i]);
        unsigned int s1 = r.vector_size() <= b.vector_size() ? 0 : static_cast<unsigned int>(r.v[b.vector_size()]);
        unsigned int s2 = r.vector_size() <= b.vector_size() - 1 ? 0 : static_cast<unsigned int>(r.v[b.vector_size() - 1]);
        long long d = (static_cast<unsigned long long>(BigInt::base) * s1 + s2) / b.v.back();
        r -= b * BigInt(d);
        while (r < 0) {
            r += b;
            --d;
        }
        q.v[i] = static_cast<unsigned int>(d);
    }

    q.sign = a1.sign * b1.sign;
    r.sign = a1.sign;
    q.trim();
    r.trim();
    return make_pair(q, r / norm);
}

ostream &operator<<(ostream &os, const BigInt &b)
{
    int base = 10;
    if (os.flags() & std::ios::hex) {
        base = 16;
    }
    else if (os.flags() & std::ios::oct) {
        base = 8;
    }
    os << b.toStr(base);
    return os;
}

istream &operator>>(istream &is, BigInt &b)
{
    string str;
    is >> str;
    int _base = 10;
    if (is.flags() & std::ios::hex) {
        _base = 16;
    }
    else if (is.flags() & std::ios::oct) {
        _base = 8;
    }
    b = BigInt::fromStr(str, _base);
    return is;
}
