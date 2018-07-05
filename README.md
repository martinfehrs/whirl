# LLX
a header only library for implementing LL(X) parsers 

This library provides functions to easyly implement LL(X) parsers.
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
int main()
{
  std::stringstream ss("\t 12345 \n 67890");

  while(LL1::is_not(ss, EOF)
  {
    while(LL1::is_one_of(ss, ' \t\n'))
      LL1::ignore(ss);
   
    std::string numstr;
   
    while(LL1::is(ss, digit))
      numstr += read();
  
    std::cout << numstsr << std::endl;
  }
  
  return 0;
}
```

