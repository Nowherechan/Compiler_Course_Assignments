# Parser

```ebnf
Start ::= codelines
```

## Statements

### Code Block
```ebnf
codeblock ::= "{" codelines "}"
codelines ::= statement { codelines }
statement ::= simple_stmt | if_stmt | while_stmt | funcdef_stmt 
```

### Simple Statement
```ebnf
simple_stmt ::= expression_stmt | continue_stmt | return_stmt | declare_stmt
expression_stmt ::= expression ";"
continue_stmt ::= continue ";"
return_stmt ::= return ";"
```
`expression ::=` [`See Expressions`](#Expressions)

### Declare Statement
```ebnf
declare_stmt ::= Qualifier Identifier {"=" expression} ";"
```

### If Statement
```ebnf
if_stmt ::= If "(" expression ")" codeblock {Else codeblock}
```

### While Statement
```ebnf
while_stmt ::= While "(" expression ")" codeblock
```

### Function Define Statement
```ebnf
funcdef_stmt ::= Qualifier Identifier "(" {arg_list} ")" codeblock
arg_list ::= Qualifier Identifier {, arg_list}
```

## Expressions
> prim_exp means the highest priority
```ebnf
prim_exp ::= attr_ref | func_call | subscription | atom
attr_ref ::= prim_exp "." Identifier
func_call ::= prim_exp "(" expr_list ")"
expr_list ::= prim_exp {"," expr_list}
subscription ::= prim_exp "[" expression "]"
```

```ebnf
prim_exp ::= atom {"." Identifier | "(" expr_list ")" | "[" expression "]"}
expr_list ::= expression {"," expr_list}
```

### Atom
```ebnf
atom ::= Identifier | const | "(" expression ")"
const ::= IntConst | DoubleConst
```
> Priority: func() > *, /, % > +, - > ">>", "<<" > & > ^ > | > relop > ! > && > ||

### Logic
```ebnf
expression ::= or_exp
or_exp ::= and_exp "||" or_exp | and_exp
and_exp ::= neg_exp "&&" and_exp | neg_exp
neg_exp ::= "!" neg_exp | relop_exp
```

### Bit
```ebnf
relop_exp ::= bitor_exp Relop relop_exp | bitor_exp
bitor_exp ::= bitxor_exp "|" bitor_exp | bitxor_exp
bitxor_exp ::= bitand_exp "^" bitxor_exp | bitand_exp
bitand_exp ::= bitsft_exp "&" bitand_exp | bitsft_exp
bitsft_exp ::= add_exp "<<" bitsft_exp | add_exp ">>" bitsft_exp | add_exp
```

### Bin Op to Primary Expression
```ebnf
add_exp ::= mult_exp "+" add_exp | mult_exp "-" add_exp | mult_exp
mult_exp ::= prim_exp "*" mult_exp | prim_exp "/" mult_exp | prim_exp "%" mult_exp | prim_exp
```