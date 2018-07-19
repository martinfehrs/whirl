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
auto read_temperature(std::istream& ins, LLk::code_position& pos) // throws unexpected_token
{
    auto has_sign = LLk::ignore_if(ins, pos, '-');
    auto val = LLk::expect(ins, pos, LLk::digit) - '0';

    while(LLk::is(ins, LLk::digit))
        val = val * 10 + LLk::read(ins, pos) - '0';

    return val * (has_sign ? -1 : 1);
}

auto read_temperatures(std::istream& ins, LLk::code_position& pos) // throws unexpected_token
{
    std::vector<int> temperatures;

    LLk::ignore_while(ins, pos, LLk::space);

    while(LLk::is_not(ins, EOF))
    {
        temperatures.push_back(read_temperature(ins, pos));
        LLk::ignore_while(ins, pos, LLk::space);
    }

    return temperatures;
}
```

