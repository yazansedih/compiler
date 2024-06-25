# compiler
...


In the first part, we write a scanner by implemented a FileDescriptor class that can open a file, read lines, track the current character and line number, and report errors. This class serves as a foundation for handling file input in our compiler. Then, we extended this by developing a Scanner class that reads COMP2024 language programs, tokenizes the input based on defined lexemes, and outputs the tokens with their types. The scanner identifies identifiers, keywords, integer literals, strings, operators, comments, and handles whitespace and errors appropriately.
