<div class="heading">
    <div><img src="assets/images/logo.svg" alt="whirl"/></div>
    <div><i>header only library for implementing LL(1) parsers.</i></div>
    <br />
</div>

Its purpose is to simplify parser implementations providing a set of higher level functions on top
of the stl's streaming capabilities or compatible APIs. It can be used as a more verbose but better readable alternative to regular expressions.

This library is in an early development stage.
There is no unicode support, yet. Only utf-32 is fully supported. For utf-16 only the characters of
the BMP are recognized correctly. Supports all single byte encodings.

## Goals for Version 1
- full unicode support for utf-8 and utf-16
- functional composition of primitives.
- transformators for reading functions
- some simple examples
- unit tests for all components

## Long term goals
- a rich set of predefined lookahead and reading functions for recurring token sequences
- some complex examples
- a doxypress generated api documentation
- an introducing tutorial
- a constexpr implementation of regular expressions on top of this library

## Simple Example
Reading sequential measurement data from an input stream provided by an input device. The full
example can be found in the examples directory.

The EBNF we want to represent is:

```
end                    = ? virtual end token (not part of the character set) ? ;
non-zero-decimal-digit = "1" | "2" | "3" | "4" | "5" | "6" | "7" | "8" | "9" ;
decimal-digit          = "0" | non-zero-decimal-digit ;
whitespace             = " " | "\t" | "\n" ;
decimal-whole-number   =  "0" | [ "-" ], non-zero-decimal-digit, {decimal-digit} ;
separator              = { whitespace }- ;
data-entry             = ( decimal-whole-number, ( ( separator, data-entry ) | end ) ) ) | end;
data                   = { whitespace }, data-entry ;
```

```C++
constexpr auto sign                = whirl::is('-');
constexpr auto number              = whirl::is(digit) || whirl::is(sign);

constexpr auto read_sign           = whirl::next_if(sign, whirl::as(-1)) || 1;
constexpr auto read_digit          = whirl::next(whirl::as_digit);
constexpr auto read_digit_sequence = whirl::next_while(digit, whirl::as_digits);

constexpr auto ignore_whitespace   = whirl::next_while(space);

auto read_data_entries(std::istream& ins, whirl::code_position& pos)
{
constexpr auto sign                = whirl::is('-');
constexpr auto number              = whirl::is(digit) || whirl::is(sign);

constexpr auto read_sign           = whirl::next_if(sign, whirl::as(-1)) || 1;
constexpr auto read_digit          = whirl::next(whirl::as_digit);
constexpr auto read_digit_sequence = whirl::next_while(digit, whirl::as_digits);

auto read_data_entries(std::istream& ins, whirl::code_position& pos)
{
    std::vector<int> temperatures;

    whirl::next_while(ins, pos, space);

    while(whirl::is(ins, number))
    {
        const auto number = whirl::is(ins, zero) ?
            read_digit(ins, pos) :
            read_digit_sequence(ins, pos, read_sign(ins, pos) * read_digit(ins, pos));

        temperatures.push_back(number);
        whirl::next_while(ins, pos, space);
    }

    whirl::next_is(ins, pos, end);

    return temperatures;
}
}
```
