# LL1
a header only library for implementing LL(1) parsers 

This library provides functions to easily implement LL(1) parsers with a lookahead of 1. There is no unicode support, yet. Only utf-32 is fully supported. For utf-16 only the characters of the BMP are recognized correctly. Supports all single byte encodings.

## Targets
- arbitrary lookaheads
- full unicode support for utf-8 and utf-16
- regular expressions as comparsion objects

## Simple Example
Reading sequential data from an input stream provided by a sensor or a file. The full example can be found in the examples directory.

```C++
auto read_temperature(std::istream& ins, LL1::code_position& pos) // throws unexpected_token
{
    using LL1::sets::digit;

    auto has_sign = LL1::ignore_if(ins, pos, '-');
    auto val = LL1::expect(ins, pos, digit) - '0';

    while(LL1::is(ins, digit))
        val = val * 10 + LL1::read(ins, pos) - '0';

    return val * (has_sign ? -1 : 1);
}

auto read_temperatures(std::istream& ins, LL1::code_position& pos) // throws unexpected_token
{
    using LL1::sets::space;

    std::vector<int> temperatures;

    LL1::ignore_while(ins, pos, space);

    while(LL1::is(ins, LL1::not_(EOF)))
    {
        temperatures.push_back(read_temperature(ins, pos));
        LL1::ignore_while(ins, pos, space);
    }

    return temperatures;
}
```

