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
decimal-whole-number   =  [ "-" ], ( "0" | non-zero-decimal-digit, { decimal-digit } ) ;
data-entry             = decimal-whole-number, { whitespace } ;
data                   = { whitespace }, { data-entry }, end ;
```

```C++
auto read_decimal_whole_number(std::istream& ins, code_position& pos)
{
    auto has_sign = ignore_if(ins, pos, '-');
    auto val = 0;

    if (is(ins, '0'))
    {
        ignore(ins, pos);
    }
    else if (is(ins, digit))
    {
        val = read(ins, pos) - '0';

        while (is(ins, digit))
            val = val * 10 + read(ins, pos) - '0';
    }
    else
    {
        throw unexpected_input{};
    }

    return val * (has_sign ? -1 : 1);
}

auto read_data_entry(std::istream& ins, code_position& pos)
{
    auto temperature = read_decimal_whole_number(ins, pos);
    ignore_while(ins, pos, space);

    return temperature;
}

auto read_data(std::istream& ins, code_position& pos)
{
    std::vector<int> temperatures;

    ignore_while(ins, pos, space);

    while(is(ins, digit) || is(ins, '-'))
        temperatures.push_back(read_data_entry(ins, pos));

    return temperatures;
}
```

