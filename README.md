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
auto read_temperatures(std::istream& ins)
{
  std::vector<int> temperatures;

  LLk::ignore_while(ins, LLk::space);

  while(LLk::is_not(ins, EOF))
  {  
    std::string numstr;
    
    numstr += LLk::expect(ins, LLk::digit);
    numstr += LLk::read_while(ins, LLk::digit);
    temperatures.push_back(std::stoi(numstr));
   
    LLk::ignore_while(ins, LLk::space);
  }
  
  return temperatures;
}
```

