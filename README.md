# LLX
a header only library for implementing LL(X) parsers 

This library provides functions to easily implement LL(X) parsers.
Currently, only a lookahead of 1 is supported.
There is no unicode support, yet. Only utf-32 is fully supported.
For utf-16 only the characters of the BMP are supported.
Supports all single byte encodings.

## Targets
- supporting arbitrary lookaheads
- full unicode support for utf-8 and utf-16
- support for regular expressions as comparsion objects

## Simple Example

```C++
auto read_temperatures(std::istream& is)
{
  std::vector<int> temperatures;

  LL1::ignore_while(ss, LL1::space);

  while(LL1::is_not(ss, EOF))
  {  
    std::string numstr;
    
    numstr += LL1::expect(ss, digit);
    numstr += LL1::read_while(ss, digit);
    temperatures.push_back(std::stoi(numstr));
   
    LL1::ignore_while(ss, LL1::space);
  }
  
  return temperatures;
}
```

