# LL1
a header only library for implementing LL(1) parsers 

This library provides functions to easily implement LL(1) parsers with a lookahead of 1. There is no unicode support, yet. Only utf-32 is fully supported. For utf-16 only the characters of the BMP are recognized correctly. Supports all single byte encodings.

## Targets
- arbitrary lookaheads
- full unicode support for utf-8 and utf-16
- regular expressions as comparsion objects

## Simple Example
Reading sequential measurement data from an input stream provided by an input device. The full example can be found in the examples directory.

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

