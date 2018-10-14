#ifndef __SEQUENTIAL_HPP__
#define __SEQUENTIAL_HPP__


// EBNF:
// =================================================================================================
// end                    = ? virtual end token (not part of the character set) ? ;
// non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
// decimal-digit          = "0" | non-zero-decimal-digit ;
// whitespace             = " " | "\t" | "\n" ;
// decimal-whole-number   =  ["-"], ( "0" | non-zero-decimal-digit, {decimal-digit} ) ;
// separator              = { whitespace }- ;
// data-entry             = ( decimal-whole-number, ( ( separator, data-entry ) | end ) ) ) | end;
// data                   = { whitespace }, data-entry ;
// =================================================================================================


#include "LL1.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>


namespace sequential
{
    constexpr auto is_separator      = LL1::is_space;
    constexpr auto is_sign           = LL1::is('-');
    constexpr auto is_number         = LL1::is_digit || is_sign;

    constexpr auto read_sign         = LL1::next_if(is_sign, [](const auto&){ return -1; });
    constexpr auto read_digit        = LL1::next(LL1::as_digit);
    constexpr auto ignore_whitespace = LL1::next_while(LL1::is_space);


    auto read_decimal_whole_number(std::istream& ins, LL1::code_position& pos)
    {
        auto sign = read_sign(ins, pos);
        auto val = 0;

        if (LL1::is_zero(ins))
        {
            LL1::next(ins, pos);
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

    std::vector<int> read_data_entry(std::istream& ins, LL1::code_position& pos)
    {
        std::vector<int> temperatures;

        if (is_number(ins))
        {
            temperatures.push_back(read_decimal_whole_number(ins, pos));
        
            if (is_separator(ins))
            {
                LL1::next(ins, pos);
                ignore_whitespace(ins, pos);
                const auto further_temperatures = read_data_entry(ins, pos);
                
                temperatures.insert(
                    std::end(temperatures),
                    std::begin(further_temperatures), std::end(further_temperatures)
                );
                
                return temperatures;
            }
            else if(LL1::is_end(ins))
            {
                return temperatures;
            }
            else
            {
                throw LL1::unexpected_input{};
            }
        }
        else if (LL1::is_end(ins))
        {
            return temperatures;
        }
        else
        {
            throw LL1::unexpected_input{};
        }
    }

    auto read_data(std::istream& ins, LL1::code_position& pos)
    {
        ignore_whitespace(ins, pos);

        return read_data_entry(ins, pos);
    }

}

#endif /*__SEQUENTIAL_HPP__*/
