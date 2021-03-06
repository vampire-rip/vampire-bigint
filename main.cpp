
#include <iostream>
#include "bright.h"

using namespace std;

int main()
{
    // 输入

    // 自动隐式类型转换
    Bright a = 10; 

    // from a long long int
    Bright b(123456); 
    
    // from a really long number
    Bright c("262019165839795466636015683089358648091");

    // from a really long hex (or oct) number
    Bright d("C51F0E45981B5BACA24281DB78E3F31B", 16);

    // from 4 * 2^30 bytes (2^120 bytes total)
    Bright e;
    e.unshift(Bright::mask); // mask is just 2^30-1, 111111111111111111111111111111
    e.unshift(Bright::mask);
    e.unshift(Bright::mask);
    e.unshift(Bright::mask);

    // from input stream
    // Bright e;
    // cin >> e;

    // from hex input stream (or oct)
    // Bright f;
    // cin >> hex >> f;

    // 输出

    // output stream

    cout << "a is: " << a << endl;
    cout << "b(oct) is: " << oct << b << endl;
    cout << "c(hex) is: " << hex << c << endl;
    cout << "d(dec) is: " << dec << d << endl;

    // get string (support base 2, 8, 10, 16)

    cout << "e(bin) is: " << e.toStr(2) << endl;
    cout << endl;

    // 获得长度

    cout << "bit length of e is: " << dec << e.size() << endl;
    cout << endl;

    // 大小比较

    cout << "a > b?\t" << boolalpha << (a > b) << endl;
    cout << "a <= b?\t" << boolalpha << (a <= b) << endl;
    cout << "c == d?\t" << boolalpha << (c == d) << endl;
    cout << "c != d?\t" << boolalpha << (c != d) << endl;
    cout << endl;

    // 运算

    cout << "c + d = " << c + d << endl;
    cout << "c * 2 = " << c * 2 << endl;
    cout << "c % c = " << c % c << endl;

    cout << "c * d = " << hex << c * d << endl;
    cout << "c / (d / 2) = " << dec << c / (d / 2) << endl;
    cout << "c % (d / 2) = " << dec << c % (d / 2) << endl;
    cout << endl;

    // 2^30 进制移位

    cout << "e >> 3 = " << (e >> 3).toStr(2) << endl;
    cout << "1 << 9 = " << hex << (Bright(1) << 9) << endl;
    cout << endl;

    return 0;
}
