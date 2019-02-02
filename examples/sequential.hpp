#ifndef __SEQUENTIAL_HPP__
#define __SEQUENTIAL_HPP__


// EBNF:
// =================================================================================================
// end                    = ? virtual end token (not part of the character set) ? ;
// non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
// decimal-digit          = "0" | non-zero-decimal-digit ;
// whitespace             = " " | "\t" | "\n" ;
// decimal-whole-number   = "0" | [ "-" ], non-zero-decimal-digit, { decimal-digit } ;
// separator              = { whitespace }- ;
// data-entry             = ( decimal-whole-number, ( ( separator, data-entry ) | end ) ) ) | end;
// data                   = { whitespace }, data-entry ;
// =================================================================================================


#include "whirl.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>


namespace sequential
{
    constexpr auto is_separator        = whirl::is_space;
    constexpr auto is_sign             = whirl::is('-');
    constexpr auto is_number           = whirl::is_digit || is_sign;
    constexpr auto is_non_zero_number  = is_number && !whirl::is_zero;

    constexpr auto read_sign           = whirl::next_if(is_sign, whirl::as(-1)) || 1;
    constexpr auto read_digit          = whirl::next(whirl::as_digit);
    constexpr auto read_digit_sequence = whirl::next_while(whirl::is_digit, whirl::as_digits);

    constexpr auto ignore_whitespace   = whirl::next_while(whirl::is_space);

    auto read_decimal_whole_number(std::istream& ins, whirl::code_position& pos)
    {
        if (whirl::is_zero(ins))
            return read_digit(ins, pos);
        else
            return read_digit_sequence(ins, pos, read_sign(ins, pos) * read_digit(ins, pos));

    }

    std::vector<int> read_data_entry(std::istream& ins, whirl::code_position& pos)
    {
        std::vector<int> temperatures;

        if (is_number(ins))
        {
            temperatures.push_back(read_decimal_whole_number(ins, pos));

            if (is_separator(ins))
            {
                whirl::next(ins, pos);
                ignore_whitespace(ins, pos);

                const auto further_temperatures = read_data_entry(ins, pos);

                temperatures.insert(
                    std::end(temperatures),
                    std::begin(further_temperatures), std::end(further_temperatures)
                );

                return temperatures;
            }
            else
            {
                whirl::next(ins, pos, whirl::is_end);
                return temperatures;
            }
        }
        else
        {
            whirl::next(ins, pos, whirl::is_end);
            return temperatures;
        }
    }

    auto read_data(std::istream& ins, whirl::code_position& pos)
    {
        ignore_whitespace(ins, pos);
        return read_data_entry(ins, pos);
    }
}

#endif /*__SEQUENTIAL_HPP__*/
