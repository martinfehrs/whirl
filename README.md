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
Reading sequential data from an input stream provides by a sensor or a file.

```C++
auto read_temperatures(std::istream& ins)
{
  std::vector<int> temperatures;

  LL1::ignore_while(ins, LL1::space);

  while(LL1::is_not(ins, EOF))
  {  
    std::string numstr;
    
    numstr += LL1::expect(ins, digit);
    numstr += LL1::read_while(ins, digit);
    temperatures.push_back(std::stoi(numstr));
   
    LL1::ignore_while(ins, LL1::space);
  }
  
  return temperatures;
}
```

