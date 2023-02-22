# Parser

```ebnf
Start ::= codelines
```

## Statements

### Code Block
```ebnf
codeblock ::= "{" codelines "}"
codelines ::= [statement]*
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
declare_stmt ::= Qualifier Identifier ["=" expression]? ";"
```

### If Statement
```ebnf
if_stmt ::= If "(" expression ")" codeblock [Else codeblock]?
```

### While Statement
```ebnf
while_stmt ::= While "(" expression ")" codeblock
```

### Function Define Statement
```ebnf
funcdef_stmt ::= Qualifier Identifier "(" arg_list ")" codeblock
```

## Expressions

### Atom
```ebnf
const ::= IntConst | DoubleConst
atom ::= Identifier | const | "(" expression ")"
```