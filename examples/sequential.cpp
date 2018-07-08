#include "LLk.hpp"
#include <fstream>
#include <iostream>

auto read_temperature(std::istream& ins, LLk::code_position& pos)
{
    auto sign = LLk::read_if(ins, pos, '-');
    auto val = LLk::expect(ins, pos, LLk::digit<char>) - '0';

    while(LLk::is(ins, LLk::digit<char>))
        val = val * 10 + LLk::read(ins, pos) - '0';

    return val * (sign ? -1 : 1);
}

auto read_temperatures(std::istream& ins, LLk::code_position& pos)
{
    std::vector<int> temperatures;

    while(LLk::is_not(ins, EOF))
    {
        temperatures.push_back(read_temperature(ins, pos));
        LLk::ignore_while(ins, pos, LLk::space<char>);
    }

    return temperatures;
}

int main()
{
    LLk::code_position pos{ 1, 1 };

    std::ifstream ifs("sequential_input.txt");

    try
    {
        for(auto temperature : read_temperatures(ifs, pos))
            std::cout << temperature << '\n';
    }
    catch(LLk::unexpected_token)
    {
        std::cout << "unexpeced token "
                  << static_cast<char>(ifs.peek())
                  << " at ("
                  << pos.row
                  << ", "
                  << pos.col
                  << ")\n";
    }

    return 0;
}
