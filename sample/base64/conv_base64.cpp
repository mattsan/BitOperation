#include "Bits.h"

#include <iostream>
#include <string>
#include <algorithm>

using namespace emattsan::bits;

static const char Table[] =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

std::string encode_base64(const std::string& s)
{
    std::string src(s + std::string(2, '\0'));
    std::string result(((s.size() + 2) / 3) * 4, ' ');

    std::string::iterator ri = result.begin();
    for(std::size_t i = 0; i < s.size(); i += 3)
    {
        Bits<6> a, b, c, d;
        (a, b, c, d) = (Bits<8>(src[i]), Bits<8>(src[i + 1]), Bits<8>(src[i + 2]));
        *ri++ = Table[a];
        *ri++ = Table[b];
        *ri++ = Table[c];
        *ri++ = Table[d];
    }

    std::string::reverse_iterator rri = result.rbegin();
    for(std::size_t i = 0; i < ((2 - s.size()) % 3); ++i)
    {
        *rri++ = '=';
    }

    return result;
}

std::string decode_base64(const std::string& s)
{
    static const std::string table(Table);

    std::string result(s.size() * 3 / 4, '?');

    std::string::const_iterator si = s.begin();
    std::string::iterator       ri = result.begin();
    for(std::size_t i = 0; i < s.size(); i += 4)
    {
        std::string::size_type a = table.find(*si++); if(si == s.end()) return result;
        std::string::size_type b = table.find(*si++); if(si == s.end()) return result;
        std::string::size_type c = table.find(*si++); if(si == s.end()) return result;
        std::string::size_type d = table.find(*si++);
        a = (a != std::string::npos) ? a : 0;
        b = (b != std::string::npos) ? b : 0;
        c = (c != std::string::npos) ? c : 0;
        d = (d != std::string::npos) ? d : 0;

        Bits<8> r1, r2, r3;
        (r1, r2, r3) = (Bits<6>(a), Bits<6>(b), Bits<6>(c), Bits<6>(d));
        *ri++ = r1;
        *ri++ = r2;
        *ri++ = r3;
    }

    result.resize(result.size() - std::count(s.begin(), s.end(), '='));

    return result;
}

int main(int argc, char* argv[])
{
    for(int i = 1; i < argc; ++i)
    {
        std::cout << encode_base64(argv[i]) << "/" << decode_base64(argv[i]) << std::endl;
    }

    return 0;
}