#include "LL1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

auto read_temperature(std::istream& ins, LL1::code_position& pos)
{
    using LL1::sets::digit;

    auto has_sign = LL1::ignore_if(ins, pos, '-');
    auto val = LL1::expect(ins, pos, digit) - '0';

    while(LL1::is(ins, digit))
        val = val * 10 + LL1::read(ins, pos) - '0';

    return val * (has_sign ? -1 : 1);
}

auto read_temperatures(std::istream& ins, LL1::code_position& pos)
{
    using LL1::sets::space;

    std::vector<int> temperatures;

    while(LL1::is(ins, LL1::not_(EOF)))
    {
        temperatures.push_back(read_temperature(ins, pos));
        LL1::ignore_while(ins, pos, space);
    }

    return temperatures;
}

int main()
{
    std::locale::global(std::locale(""));

    LL1::code_position pos{ 1, 1 };

    std::ifstream ifs("sequential_input.txt");

    try
    {
        for(auto temperature : read_temperatures(ifs, pos))
           std::cout << temperature << "\n";

        return EXIT_SUCCESS;
    }
    catch(LL1::unexpected_token)
    {
        std::cout << "unexpeced token "
                  << static_cast<char>(ifs.peek())
                  << " at ("
                  << pos.row
                  << ", "
                  << pos.col
                  << ")\n";
                  
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
