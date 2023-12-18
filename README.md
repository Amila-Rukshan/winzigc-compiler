# winzigc

## Token Definitions

### Definitions.
* **Identifier**: Any sequence of characters which may contain alphabets, digits from
0 to 9 or an underscore. The sequence must start with an alphabet or an
underscore.
* **Integer**: A sequence of characters which may contain any combination of digits
from 0 to 9.
* White Space: Any sequence of characters containing any combination of single
space, form feed, horizontal tab and vertical tab.
* **Char**: A sequence of three characters which contains a single character between
two single quotes. The single character between the two quotes can be any
character except a single quote itself. For example, ‘’’ is an invalid character.
* **String**: A sequence of characters which contains any number of characters
between two double quotes. The character sequence between the two double
quotes can be any character except a double quote itself. For example,
“Hello”how” is an invalid string.
* **Comment**: Type 1: a sequence of characters which starts with # and ends with a
newline. Type2: begins with ‘{‘, continues with any character (including end-of-
line), and ends with ‘}’.

### Pre-defined tokens in the language are given below:

| No. | Token         | Description                         |
| --- |-------------- | ----------------------------------- |
|   1 |**\n**         | Newline  |
|   2 |**program**         | Start of Program  |
|   3 |**var**         | Variable  |
|   4 |**const**         | Constant  |
|   5 |**type**         | To define a data type  |
|   6 |**function**         | To define a function  |
|   7 |**return**         | return from function  |
|   8 |**begin**         | start of a block  |
|   9 |**end**         | end of a block  |
|  10 |**:=:**         | swap |
|  11 |**:=**         | assignment operator |
|  12 |**output**         | output an expression or string |
|  13 |**if**         | keyword |
|  14 |**then**         | keyword |
|  15 |**else**         | keyword |
|  16 |**while**         | keyword for loop |
|  17 |**do**         | keyword for loop |
|  18 |**case**         | keyword |
|  19 |**of**         | keyword |
|  20 |**..**         | dots for case expression |
|  21 |**otherwise**         | keyword |
|  22 |**repeat**         | keyword for repeat-until loop |
|  23 |**for**         | keyword for loop |
|  24 |**until**         | keyword for repeat-until loop |
|  25 |**loop**         | keyword for loop-pool loop |
|  26 |**pool**         | keyword for loop-pool loop |
|  27 |**exit**         | keyword |
|  28 |**<=**         | less than equal to binary operator |
|  29 |**<>**         | not equal to binary operator |
|  30 |**<**         | less than binary operator |
|  31 |**>=**         | greater than equal to binary operator |
|  32 |**>**         | greater than binary operator |
|  33 |**=**         | equal to binary operator |
|  34 |**mod**         | modulus binary operator |
|  35 |**and**         | and binary operator |
|  36 |**or**         | or binary operator |
|  37 |**not**         | not unary operator |
|  38 |**read**         | read an identifier |
|  39 |**succ**         | successor of an ordinal value |
|  40 |**pred**         | predecessor of a ordinal value |
|  41 |**chr**         | keyword for character function |
|  42 |**ord**         | keyword for ordinal function |
|  43 |**eof**         | keyword for end of file |
|  44 |**{**         | begin of a block |
|  45 |**:**         | colon |
|  46 |**;**         | semi colon |
|  47 |**.**         | single dot |
|  48 |**,**         | comma |
|  49 |**(**         | opening bracket |
|  50 |**)**         | closing bracket |
|  51 |**+**         | plus |
|  52 |**-**         | minus |
|  53 |*         | multiply |
|  54 |**/**         | divide |

### Grammar rules for WinZig

<pre>
Winzig     ->  'program' Name ':' Consts Types Dclns SubProgs Body Name '.'  => "program";

Consts     ->  'const' Const list ',' ';'                                    => "consts"
           ->                                                                => "consts";
 
Const      ->  Name '=' ConstValue                                           => "const"; 

ConstValue ->  '&#x1438;integer&#x1433;'  
           ->  '&#x1438;char&#x1433;'
           ->  Name;

Types      ->  'type' (Type ';')+                                            => "types"
           ->                                                                => "types";
           
Type       ->  Name '=' LitList                                              => "type";

LitList    ->  '(' Name list ',' ')'                                         => "lit";

SubProgs   ->  Fcn*                                                          => "subprogs";

Fcn        ->  'function' Name '(' Params ')' ':' Name                       
               ';' Consts Types Dclns Body Name ';'                          => "fcn";

Params     ->  Dcln list ';'                                                 => "params";

Dclns      ->  'var' (Dcln ';')+                                             => "dclns"
           ->                                                                => "dclns";

Dcln       ->  Name list ',' ':' Name                                        => "var";

Body       ->  'begin' Statement list ';' 'end'                              => "block";
 
Statement  ->  Assignment
           ->  'output' '(' OutExp list ',' ')'                              => "output"
           ->  'if' Expression 'then' Statement ('else' Statement)?          => "if"
           ->  'while' Expression 'do' Statement                             => "while"
           ->  'repeat' Statement list ';' 'until' Expression                => "repeat"
           ->  'for' '(' ForStat ';' ForExp ';' ForStat ')' Statement        => "for"
           ->  'loop' Statement list ';' 'pool'                              => "loop"
           ->  'case' Expression 'of' Caseclauses OtherwiseClause 'end'      => "case"
           ->  'read' '(' Name list ',' ')'                                  => "read"
           ->  'exit'                                                        => "exit"
           ->  'return' Expression                                           => "return"
           ->  Body
           ->                                                                => "&#x1438;null&#x1433;"; 
  
OutExp     ->  Expression                                                    => "integer"
           ->  StringNode                                                    => "string";
          
StringNode ->  '&#x1438;string&#x1433;'; 
  
Caseclauses->  (Caseclause ';')+;
 
Caseclause ->  CaseExpression list ',' ':' Statement => "case_clause";

CaseExpression 
           ->  ConstValue
           ->  ConstValue '..' ConstValue                                    => "..";
           
OtherwiseClause
           ->  'otherwise' Statement                                         => "otherwise"
           ->  ; 
           
Assignment ->  Name ':=' Expression                                          => "assign"
           ->  Name ':=:' Name                                               => "swap";                                          
ForStat    ->  Assignment
           ->                                                                => "&#x1438;null&#x1433;"; 
  
ForExp     ->  Expression
           ->                                                                => "true";

Expression ->  Term
           ->  Term '<=' Term                                                => "<="
           ->  Term '<' Term                                                 => "<"
           ->  Term '>=' Term                                                => ">="
           ->  Term '>' Term                                                 => ">"
           ->  Term '=' Term                                                 => "="
           ->  Term '<>' Term                                                => "<>";

Term       ->  Factor
           ->  Term '+' Factor                                               => "+"
           ->  Term '-' Factor                                               => "-"
           ->  Term 'or' Factor                                              => "or";
           
Factor     ->  Factor '*' Primary                                            => "*"
           ->  Factor '/' Primary                                            => "/"
           ->  Factor 'and' Primary                                          => "and"
           ->  Factor 'mod' Primary                                          => "mod"
           ->  Primary;  
  
Primary    ->  '-' Primary                                                   => "-"
           ->  '+' Primary                                                   
           ->  'not' Primary                                                 => "not"
           ->  'eof'
           ->  Name 
           ->  '&#x1438;integer&#x1433;' 
           ->  '&#x1438;char&#x1433;'
           ->  Name '(' Expression list ',' ')'                              => "call"      
           ->  '(' Expression ')'
           ->  'succ' '(' Expression ')'                                     => "succ"
           ->  'pred' '(' Expression ')'                                     => "pred"
           ->  'chr' '(' Expression ')'                                      => "chr"
           ->  'ord' '(' Expression ')'                                      => "ord";
  
Name       ->  '&#x1438;identifier&#x1433;'; 
 
</pre>


####  Infix, binary regular expression operators

<pre>
 +   :   (meaning one or more).  This is a unary, postfix regular expression
    
          operator.   It is used in the production rule
          Types      -> 'type' (Type ';')+
    
          It means "the keyword 'type', followed by one or more instances of (Type ';')"
    
          It does not mean "the keyword 'type', followed by (Type ';'), followed by +  "
    
          Note that the +  is not enclosed in single quotes, meaning it is not a terminal
          symbol in the grammar.   There are instances of + that are enclosed in single
          quotes, i.e. those *are* terminals.

*    :   (meaning zero or more).  This is a unary, postfix regular expression operator.
    
          It is used in the production rule
    
          SubProgs   -> Fcn*
          It means "zero or more instances of Fcn"
    
          It does not mean "Fcn followed by * " 
          Note that the *  is not enclosed in single quotes, meaning it is not a terminal
          symbol in the grammar.  There are instances of * that are enclosed in single
          quotes, i.e. those *are* terminals.

?    :    (meaning zero or one, i.e. optional).  This is a unary, postfix regular expression operator.
    
          It is used in the production rule 
    
          Statement ->  'if' Expression 'then' Statement ('else' Statement)?    
    
          It means "the phrase ('else' Statement)  is optional"
    
          It does not mean "('else' Statement) followed by ? " 
          Note that the ?  is not enclosed in single quotes, meaning it is not a terminal
           symbol in the grammar.  There are no instances of ? that are enclosed in single
    ​       quotes, i.e.  ? is not keyword in the Tiny language.

list :    (meaning a list of items, with separators).  This a binary, infix regular expression operator.
          It is used in several places in the grammar, for example ​
          Statement list ';'
          It means "a list of Statement's, separated by ';'s  "
          It does not mean "a Statement, followed by a list, followed by a ';'  " 
          The definition:   a list b   =  a (b a)*, so
          Statement list ';'    =   Statement (';'  Statement)*
          This means "a Statement, and then zero or more instances of (';' Statement)"
          This exactly what it means to have a list of Statements, separated by ';'s, i.e.
          the list must begin with a Statement, must end with a Statement, and between every
          two Statement's there is a ';' separating them.

</pre>

