## WinZigC programming language

This tree includes the code for a compiler for the language specified by [WinZigC language spectification](/SPECIFICATION.md).
It generates [LLVM](https://llvm.org/) bitcode for the WinZigC program and compiles the bitcode to an executable binary using the [Clang](https://clang.llvm.org/) backend.
## Related content

### Blog posts

- [The journey of implementing the WinZigC programming language](https://developerdiary.me/the-journey-of-implementing-the-winzigc-programming-language/)
- [Making WinZigC programs debuggable](https://developerdiary.me/making-winzigc-programs-debuggable/)

### Videos

- [Demo: Debugging a WinZigC program using LLDB](https://youtu.be/AF6I5WRHq2E?si=3xELeZTg1plAgbat)

## Give me some example programs!

You can find a set of example programs in [example-programs](/example-programs) and optimized and non optimized control flow graphs for the corresponding programs in [cfg](/cfg).

## How to run/debug a WinZigC program?

Visit [developer guide](DEVELOPER.md).

## Memory allocation

We stay in stack memory (within functions) and global static memory (program-global memory) until the program exits. Therefore, there is no heap memory allocation or access.

### Global variables

You can declare variables in the global scope that can be modified from anywhere in your program.
Global variables are cleaned up when your program exits.

### Local variables

Memory is allocated for variables declared within a function (parameters and local variable declarations).
All local variable memory allocations are cleaned up when the function exits.

## Features

### Built-in types

- `integer` - 32 bit.

  > If your input or the program processes intermediate values that do not fit into 32 bits, unexpected results may occur.

- `char` - 8 bit ASCII characters.
  > No Unicode characters.
- `boolean` - Think of it as 1 bit. You can assign `true` and `false` values.

### User types

Users can define enumetated types:

```
type Result = ( Composite, Prime, TooBig );
```

### Functions

Users can define functions and call them. For example:

Define a function:

```
function Factor ( i : integer ):integer;
var
    j : integer;
begin
    if i > 0 then
	for (j := 1; j <= i; j:=j+1)
	    if i mod j = 0 then output ( j )
end Factor;
```

Call the function:

```
d := Factor(120);
```

### Discarding output of a function

To discard a function output by assigning it to a discard variable `d`:

```
d := MyPrintFunc(12, 'c', true);
```

### Return value of a function

WinZigC supports two ways to change the return value:

- Assign a value to the function name:
  ```
  FuncName := 7;
  ```
  > This will not cause the function to exit.
- Using a return statement:
  ```
  return 7;
  ```
  > This will cause the function to exit.

### Buit-in functions

- `read` - Read user input from the command line. 
  > Note: [Enter] key press is considered as input when reading to a _char_.

- `output` - Write output to the command line.

### Commeting code

- Single-line comment:

```
# Please ingore this line of text
```

- Multi-line/block comments:

```
{ Zero line of comment
    First line of the comment
    Second line of the comment
ingore this as well }
```

## Control flows

### Branching

- `if-else` two-way branching
  ```
  if n > 0 then
      return (n + fact(sum(n-1)))
  else return (0)
  ```
- `case` multi-way branching
  ```
  case R of
      Composite:  output ('C');
      Prime:      output ('P');
      TooBig:     output ('B');
  end
  ```

### Looping

- `for` looping
  ```
  for (i:=1; i<=7; i:=i+1)
  begin
      output (fibonacci(i))
  end
  ```
- `while` looping
  ```
  while ((c = '+') or (c = '-')) do begin
      if (c = '+') then begin
          d:=GetNext(3);
          v := v + T(3);
      end
      else begin { c = '-' }
          d:=GetNext(3);
          v := v - T(3)
      end;
  end;
  ```
- `repeat-until` looping
  ```
  repeat
      read(i);
      d:=Factor(i)
  until i <= 0
  ```

### Operators

#### Assignment

- `:=` - Assignment
- `:=:` - Swap assignment

#### Binary

- `*` - Multiplication
- `/` - Division
- `mod` - Modulus
- `+` - Addition
- `-` - Subtraction
- `<` - Less than
- `<=` - Less than or equal to
- `>` - Greater than
- `>=` - Greater than or equal to
- `=` - Equal to
- `<>` - Not equal to
- `and` - Logical _and_
- `or` - Logical _or_

#### Unary

- `not` - Logical _not_
- `-` - Negative
- `+` - Positive
- `succ(..)` - Successive value (`++` operator semantics as in other languages)
- `pred(..)` - Predecessor value (`--` operator semantics as in other languages)
