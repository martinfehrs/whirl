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


namespace sequential
{
    constexpr auto number = whirl::is(whirl::digit) || whirl::is(whirl::negative_sign);

    constexpr auto read_sign = whirl::next_is(
        whirl::negative_sign, whirl::as(whirl::Sign::negative)) || whirl::Sign::positive;

    constexpr auto read_digit = whirl::next(whirl::as_digit<int>);
    constexpr auto read_digit_sequence = whirl::next_while(whirl::digit, whirl::as_digit<int>);

    auto read_data_entries(std::istream& ins, whirl::code_position& pos)
    {
        std::vector<int> temperatures;

        whirl::next_while(ins, pos, whirl::space);

        while(whirl::is(ins, number))
        {
            const auto numtok = whirl::is(ins, whirl::zero) ?
                read_digit(ins, pos).as_signed() :
                read_digit_sequence(read_digit(read_sign(ins, pos), ins, pos), ins, pos);

            temperatures.push_back(numtok.value());

            whirl::next_while(ins, pos, whirl::space);
        }

        whirl::next_is(ins, pos, whirl::end);

        return temperatures;
    }
}

#endif /*__SEQUENTIAL_HPP__*/
