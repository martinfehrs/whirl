![whirl++](assets/images/logo.svg)<br/>
_a header only library for implementing LL(1) parsers_. 

Its purpose is to simplify parser implementations providing a set of higher level functions on top
of the stl's streaming capabilities or compatible APIs. It can be used as a more verbose but better readable alternative to regular expressions.

This library is in an early development stage.
There is no unicode support, yet. Only utf-32 is fully supported. For utf-16 only the characters of
the BMP are recognized correctly. Supports all single byte encodings.

## Goals for Version 1
- arbitrary lookaheads
- full unicode support for utf-8 and utf-16
- functional composition of primitives.
- implicit transformators for reading functions
- a doxypress generated api documentation
- further small examples
- unit tests for all components 

## Long term goals
- an objectoriented layer on top of the procedural basic layer
- a rich set of predefined lookahead and reading functions for recurring token sequences
- support for lexical analysers.
- some more complex examples.


## Simple Example
Reading sequential measurement data from an input stream provided by an input device. The full
example can be found in the examples directory.

The EBNF we want to represent is:

```
end                    = ? virtual end token (not part of the character set) ? ;
non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
decimal-digit          = "0" | non-zero-decimal-digit ;
whitespace             = " " | "\t" | "\n" ;
decimal-whole-number   =  ["-"], ( "0" | non-zero-decimal-digit, {decimal-digit} ) ;
separator              = whitespace, { whitespace } ;
data-entry             = decimal-whole-number, ( separator | end ) ;
data                   = { whitespace }, { data-entry }, end ;
```

```C++
    constexpr auto is_separator = LL1::is_space;
    constexpr auto is_sign      = LL1::is('-');
    constexpr auto is_number    = LL1::is_digit || is_sign;

    constexpr auto read_sign  = LL1::read_if(is_sign, [](const auto&){ return -1; });
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

        if(is_separator(ins))
        {
            LL1::next(ins, pos);
            LL1::next_while(LL1::is_space, ins, pos);
        }
        else if (LL1::is_end(ins))
        {
            // No seperator needed at the end of the input sequence.
            // Nothing to do here.
        }
        else
        {
            throw LL1::unexpected_input{};
        }

        return temperature;
    }

    auto read_data(std::istream& ins, LL1::code_position& pos)
    {
        std::vector<int> temperatures;

        LL1::next_while(LL1::is_space, ins, pos);

        while(is_number(ins))
            temperatures.push_back(read_data_entry(ins, pos));

        LL1::next(LL1::is_end, ins, pos);

        return temperatures;
    }
```

