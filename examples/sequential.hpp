#ifndef __SEQUENTIAL_HPP__
#define __SEQUENTIAL_HPP__


// EBNF:
// =================================================================================================
// end                    = ? virtual end token (not part of the character set) ? ;
// non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
// decimal-digit          = "0" | non-zero-decimal-digit ;
// whitespace             = " " | "\t" | "\n" ;
// decimal-whole-number   =  ["-"], ( "0" | non-zero-decimal-digit, {decimal-digit} ) ;
// separator              = end | whitespace, { whitespace } ;
// data-entry             = decimal-whole-number, separator ;
// data                   = { whitespace }, { data-entry } ;
// =================================================================================================


#include "LL1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>


namespace sequential
{

    constexpr auto is_separator = LL1::is_end || LL1::is_space;
    constexpr auto is_sign      = LL1::is('-');
    constexpr auto is_number    = LL1::is_digit || is_sign;

    constexpr auto read_sign  = LL1::read_if(is_sign, [](const auto& c){ return - 1; });
    constexpr auto read_digit = LL1::read([](const auto& c){ return c - '0'; });


    auto read_decimal_whole_number(std::istream& ins, LL1::code_position& pos)
    {
        auto sign = read_sign(ins, pos);
        auto val = 0;

        if (LL1::is_zero(ins))
        {
            next(ins, pos);
        }
        else if (LL1::is_non_zero_digit(ins))
        {
            val = read_digit(ins, pos);

            while (LL1::is_digit(ins))
                val = val * 10 + read_digit(ins, pos);
        }
        else
        {
            throw LL1::unexpected_input{};
        }

        return val * sign.value_or(1);
    }

    auto read_data_entry(std::istream& ins, LL1::code_position& pos)
    {
        auto temperature = read_decimal_whole_number(ins, pos);
        LL1::next(is_separator, ins, pos);
        LL1::next_while(LL1::is_space, ins, pos);

        return temperature;
    }

    auto read_data(std::istream& ins, LL1::code_position& pos)
    {
        std::vector<int> temperatures;

        LL1::next_while(LL1::is_space, ins, pos);

        while(is_number(ins))
            temperatures.push_back(read_data_entry(ins, pos));

        LL1::next_while(LL1::is_space, ins, pos);
        LL1::next(LL1::is_end, ins, pos);

        return temperatures;
    }

}

#endif /*__SEQUENTIAL_HPP__*/
