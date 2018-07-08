# LLK
a header only library for implementing LL(k) parsers 

This library provides functions to easily implement LL(k) parsers.
Currently, only a lookahead of 1 is supported.
There is no unicode support, yet. Only utf-32 is fully supported.
For utf-16 only the characters of the BMP are recognized correctly.
Supports all single byte encodings.

## Targets
- supporting arbitrary lookaheads
- full unicode support for utf-8 and utf-16
- support for regular expressions as comparsion objects

## Simple Example
Reading sequential data from an input stream provided by a sensor or a file.

```C++
auto read_temperature(std::istream& ins, LLk::code_position& pos) // throws unexpected_token
{
    auto sign = LLk::read_if(ins, pos, '-');
    auto val = LLk::expect(ins, pos, LLk::digit<char>) - '0';

    while(LLk::is(ins, LLk::digit<char>))
        val = val * 10 + LLk::read(ins, pos) - '0';

    return val * (sign ? -1 : 1);
}

auto read_temperatures(std::istream& ins, LLk::code_position& pos) // throws unexpected_token
{
    std::vector<int> temperatures;

    while(LLk::is_not(ins, EOF))
    {
        temperatures.push_back(read_temperature(ins, pos));
        LLk::ignore_while(ins, pos, LLk::space<char>);
    }

    return temperatures;
}
```

