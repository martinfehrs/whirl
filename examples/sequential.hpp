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


#include <fstream>
#include <iostream>
#include <sstream>
#include <deque>

#include "whirl.hpp"


using namespace whirl::predicates;


namespace sequential
{
    constexpr auto separator           = whirl::is(space);
    constexpr auto sign                = whirl::is('-');
    constexpr auto number              = whirl::is(digit) || whirl::is(sign);
    constexpr auto non_zero_number     = whirl::is(number) && !whirl::is(zero);

    constexpr auto read_sign           = whirl::next_if(sign, whirl::as(-1)) || 1;
    constexpr auto read_digit          = whirl::next(whirl::as_digit);
    constexpr auto read_digit_sequence = whirl::next_while(digit, whirl::as_digits);

    constexpr auto ignore_whitespace   = whirl::next_while(space);

    auto read_decimal_whole_number(std::istream& ins, whirl::code_position& pos)
    {
        if (whirl::is(ins, zero))
            return read_digit(ins, pos);
        else
            return read_digit_sequence(ins, pos, read_sign(ins, pos) * read_digit(ins, pos));

    }

    std::vector<int> read_data_entry(std::istream& ins, whirl::code_position& pos)
    {
        std::vector<int> temperatures;

        if (whirl::is(ins, number))
        {
            temperatures.push_back(read_decimal_whole_number(ins, pos));

            if (whirl::is(ins, separator))
            {
                whirl::next(ins, pos);
                ignore_whitespace(ins, pos);

                const auto further_temperatures = read_data_entry(ins, pos);

                temperatures.insert(
                    std::end(temperatures),
                    std::begin(further_temperatures), std::end(further_temperatures)
                );
            }
            else
                whirl::next_is(ins, pos, end);
        }
        else
            whirl::next_is(ins, pos, end);

        return temperatures;
    }

    auto read_data(std::istream& ins, whirl::code_position& pos)
    {
        ignore_whitespace(ins, pos);
        return read_data_entry(ins, pos);
    }
}

#endif /*__SEQUENTIAL_HPP__*/
