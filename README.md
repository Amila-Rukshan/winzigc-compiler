## Winzigc programming language

This project includes the code for the language specified by [winzigc language spectification](/SPECIFICATION.md).

## Memeory allocation

No heap, we stay on stack till the program exits. So no pointer arithmetic for heap.

### Global variables

You can declare variables in the global scope that can be modified from anywhere in your program. 
Global variables are cleaned up when your program exits.

### Local variables

Memory is allocated for variables declared within a function (parameters and local variable declarations). 
All local variable memory allocations are cleaned up when the function exits.

## Features

### Built in types

- `integer` - 32 bit. 
    >If your input or the program processes intermediate values that do not fit into 32 bits, unexpected results may occur.

- `char` - 8 bit ASCII characters.
    >No Unicode characters.
- `boolean` - Think of it as 1 bit. You can assign `true` and `false` values.

### User defined types

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

Winzigc supports two ways to change the return value:

- Assign a value to the function name:
    ```
    FuncName := 7;
    ```
    >This will not cause the function to exit.
- Using a return statement:
    ```
    return 7;
    ```
    >This will cause the function to exit.

### Buit-in functions

`read`   - Read user input from the command line. 
         >Note: Enter is considered as input when reading a char.
`output` - Write output to the command line.

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
