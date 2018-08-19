// EBNF:
// =================================================================================================
// end                    = ? virtual end token (not part of the character set) ? ;
// non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
// decimal-digit          = "0" | non-zero-decimal-digit ;
// whitespace             = " " | "\t" | "\n" ;
// decimal-whole-number   =  ["-"], ( "0" | non-zero-decimal-digit, {decimal-digit} ) ;
// data-entry             = decimal-whole-number, { whitespace };
// data                   = { whitespace }, { data-entry }, end;
// =================================================================================================

#include "LL1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


using namespace LL1;
using namespace LL1::sets;

auto read_decimal_whole_number(std::istream& ins, LL1::code_position& pos)
{
    auto has_sign = ignore_if(ins, pos, '-');
    auto val = expect(ins, pos, digit) - '0';

    while(is(ins, digit))
        val = val * 10 + read(ins, pos) - '0';

    return val * (has_sign ? -1 : 1);
}

auto read_data_entry(std::istream& ins, LL1::code_position& pos)
{
    auto temperature = read_decimal_whole_number(ins, pos);
    ignore_while(ins, pos, space);

    return temperature;
}

auto read_data(std::istream& ins, LL1::code_position& pos)
{
    std::vector<int> temperatures;

    ignore_while(ins, pos, space);

    while(is(ins, digit) || is(ins, '-'))
        temperatures.push_back(read_data_entry(ins, pos));

    return temperatures;
}

int main()
{
    std::locale::global(std::locale(""));

    LL1::code_position pos{ 1, 1 };

    std::ifstream ifs("sequential_input.txt");

    try
    {
        for(auto temperature : read_data(ifs, pos))
           std::cout << temperature << "\n";

        return EXIT_SUCCESS;
    }
    catch(unexpected_input)
    {
        if (is(ifs, any))
        {
            std::cerr << "unexpeced token "
                << static_cast<char>(read(ifs))
                << " at ("
                << pos.row
                << ", "
                << pos.col
                << ")\n";
        }
        else
        {
            std::cerr << "unexpected end\n";
        }

        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
