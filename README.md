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

