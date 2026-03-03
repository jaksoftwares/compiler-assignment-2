# Question 3: LEX and YACC Calculator

A LEX and YACC (Flex and Bison) based calculator that handles addition (`+`) and subtraction (`-`) operations on integers.

---

## Table of Contents

1. [Overview](#overview)
2. [File Structure](#file-structure)
3. [LEX File (calculator2.l)](#lex-file-calculator2l)
   - [What is LEX?](#what-is-lex)
   - [Code Explanation](#lex-code-explanation)
4. [YACC File (calculator2.y)](#yacc-file-calculator2y)
   - [What is YACC?](#what-is-yacc)
   - [Code Explanation](#yacc-code-explanation)
5. [How It Works](#how-it-works)
6. [How to Run](#how-to-run)
7. [Examples](#examples)
8. [Grammar Rules](#grammar-rules)
9. [Detailed Flow](#detailed-flow)

---

## Overview

This program uses **LEX** (Lexical Analyzer) and **YACC** (Parser Generator) to:
1. Tokenize mathematical expressions containing integers
2. Parse and evaluate expressions with `+` (addition) and `-` (subtraction)
3. Support both:
   - Simple expressions: `33+54`, `77-32`
   - Variable assignment: `x=10` (syntax is recognized but not fully implemented)

**Supported Operations:**
- Addition: `+`
- Subtraction: `-`

**Input:** Multi-digit integers

---

## File Structure

| File | Description |
|------|-------------|
| [`calculator2.l`](calculator2.l) | LEX specification file - tokenizes input |
| [`calculator2.y`](calculator2.y) | YACC specification file - defines grammar and actions |
| [`Makefile3`](Makefile3) | Build file to compile the project |

---

## LEX File (calculator2.l)

### What is LEX?

**LEX** (or Flex) is a lexical analyzer generator. It takes regular expression patterns and generates C code that can scan input and identify tokens.

### LEX Code Explanation

```c
%{
    #include "calculator2.tab.h"
    extern int yylval;
%}
```

This C code block:
- Includes the YACC header file (`calculator2.tab.h`) to share token definitions
- Declares `extern int yylval` to access the value passed from lexer to parser

---

### Pattern Matching Rules

```lex
[0-9]+  { yylval = atoi(yytext); return NUMBER; }
[ \t]   ; /* ignore whitespace */
\n      return 0; /* logical EOF */
.       return yytext[0];
```

| Pattern | Action | Description |
|---------|--------|-------------|
| `[0-9]+` | `yylval = atoi(yytext); return NUMBER;` | Matches one or more digits, converts to integer, stores in `yylval`, returns `NUMBER` token |
| `[ \t]` | `;` | Ignores spaces and tabs |
| `\n` | `return 0;` | Returns 0 on newline (indicates end of logical input) |
| `.` | `return yytext[0];` | Returns any other character as itself (operators like +, -) |

**Key Details:**
- `yytext` contains the matched string (the digits)
- `atoi()` converts string to integer
- `yylval` stores the integer value for the parser to use

---

### `yywrap()` Function

```c
int yywrap()
{
    return 1;
}
```

**What it does:**
- Called when lexer reaches end of input
- Returns `1` to indicate no more input files

---

## YACC File (calculator2.y)

### What is YACC?

**YACC** (or Bison) is a parser generator that creates parsers based on grammar rules with semantic actions.

### YACC Code Explanation

#### Header Section

```c
%{
    #include<stdio.h>
    int yylex();
    void yyerror(const char *s);
%}
```

| Code | Description |
|------|-------------|
| `#include<stdio.h>` | Standard I/O library |
| `int yylex();` | Declares the lexer function |
| `void yyerror(const char *s);` | Declares error handling function |

---

#### Token Declarations

```yacc
%token NAME NUMBER
```

Declares two token types:
- `NUMBER` - integer values
- `NAME` - variable names (for potential assignment)

---

### Grammar Rules

```yacc
statement: NAME '=' expression
         | expression { printf("=%d\n", $1); }
         ;

expression: expression '+' NUMBER { $$ = $1 + $3; }
          | expression '-' NUMBER { $$ = $1 - $3; }
          | NUMBER                { $$ = $1; }
          ;
```

#### Rule 1: `statement`

| Production | Action | Description |
|------------|--------|-------------|
| `statement: NAME '=' expression` | (none) | Recognizes variable assignment (not fully implemented) |
| `statement: expression` | `{ printf("=%d\n", $1); }` | Evaluates expression and prints result |

**Semantic Action:**
- `$1` refers to the value of the first symbol (the expression)
- `printf("=%d\n", $1)` prints the result in format `=result`

---

#### Rule 2: `expression`

| Production | Action | Description |
|------------|--------|-------------|
| `expression: expression '+' NUMBER` | `{ $$ = $1 + $3; }` | Addition: left expression + right NUMBER |
| `expression: expression '-' NUMBER` | `{ $$ = $1 - $3; }` | Subtraction: left expression - right NUMBER |
| `expression: NUMBER` | `{ $$ = $1; }` | Base case: expression is just a number |

**Semantic Action Explanation:**
- `$$` is the result value of this production (the LHS)
- `$1` is the value of the first symbol (left expression)
- `$3` is the value of the third symbol (the NUMBER)
- `$1 + $3` computes the sum, assigned to `$$`

---

### Error Handling

```c
void yyerror(const char *s)
{
    printf("syntax error\n");
}
```

**What it does:**
- Called when YACC encounters a syntax error
- Prints "syntax error" message

---

### Main Function

```c
int main()
{
    yyparse();
    return 0;
}
```

**What it does:**
1. Calls `yyparse()` to start parsing
2. Returns 0 when done

---

## How It Works

### Parsing Approach

The calculator uses **recursive descent parsing** through YACC:
1. LEX tokenizes the input into NUMBERs and operators
2. YACC builds a parse tree using the grammar
3. Semantic actions compute the result bottom-up

### Operator Precedence

The grammar is **left-associative**:
- `5-3-1` is parsed as `(5-3)-1 = 1`
- Operations are performed left to right

### Grammar Structure

```
statement → expression
expression → expression + NUMBER | expression - NUMBER | NUMBER
```

This is a **left-recursive grammar** - expressions can have operators on the left.

---

## How to Run

### Prerequisites

Ensure you have:
- **Flex** (LEX)
- **Bison** (YACC)
- **GCC** (C compiler)

On Linux:
```bash
sudo apt-get update
sudo apt-get install gcc flex bison make -y
```

---

### Build and Run

#### Step 1: Compile using Makefile

```bash
make -f Makefile3
```

This command:
1. Runs **Bison** on [`calculator2.y`](calculator2.y) to generate parser files
2. Runs **Flex** on [`calculator2.l`](calculator2.l) to generate lexer
3. Compiles and links to create executable `calculator2`

#### Step 2: Run the executable

```bash
./calculator2    # Linux/macOS
calculator2.exe  # Windows
```

**Note:** This program reads from stdin. You can either:
- Type expressions directly and press Enter
- Use input redirection: `echo "33+54" | ./calculator2`

---

## Examples

### Example 1: Simple Addition

```
Input: 33+54
Output: =87
```

**Explanation:** 33 + 54 = 87

---

### Example 2: Simple Subtraction

```
Input: 77-32
Output: =45
```

**Explanation:** 77 - 32 = 45

---

### Example 3: Negative Result

```
Input: 9-99
Output: =-90
```

**Explanation:** 9 - 99 = -90

---

### Example 4: Invalid Input (Multiplication)

```
Input: 9*3
Output: syntax error
```

**Explanation:** The grammar only supports `+` and `-` operators. The `*` character triggers a syntax error.

---

### Example 5: Multiple Operations (Left Associative)

```
Input: 10-5-2
Output: =3
```

**Explanation:** Parsed as `(10-5)-2 = 5-2 = 3` (left-to-right evaluation)

---

### Example 6: Chain of Operations

```
Input: 100+50-25
Output: =125
```

**Explanation:** 100 + 50 - 25 = 125

---

## Grammar Rules

### BNF (Backus-Naur Form)

```
<statement>    ::= <expression>
                  | NAME '=' <expression>

<expression>   ::= <expression> '+' NUMBER
                  | <expression> '-' NUMBER
                  | NUMBER

<NAME>         ::= [a-zA-Z_][a-zA-Z0-9_]*
<NUMBER>       ::= [0-9]+
```

### Parse Tree Example

For input `33+54`:

```
       statement
           |
      expression
     /     |     \
expression  '+'  NUMBER
    |             |
  NUMBER        54
    |
   33

Evaluation:
- NUMBER (33) has value 33
- NUMBER (54) has value 54
- expression '+' NUMBER: 33 + 54 = 87
- statement: prints "=87"
```

---

## Detailed Flow

### For Input "33+54":

```
Input: 33+54\n

LEX Tokenization:
1. "33"  → Matches [0-9]+, yylval=33, returns NUMBER
2. "+"   → Matches '.', returns '+'
3. "54"  → Matches [0-9]+, yylval=54, returns NUMBER
4. "\n"  → Returns 0 (logical EOF)

YACC Parsing with Semantic Actions:
1. Sees NUMBER (33), matches expression: NUMBER, $$ = 33
2. Sees '+', continues
3. Sees NUMBER (54), matches expression: expression '+' NUMBER
   - $1 = 33, $3 = 54
   - $$ = $1 + $3 = 33 + 54 = 87
4. Matches statement: expression
   - $1 = 87
   - prints "=87"
```

---

### For Input "9*3":

```
Input: 9*3\n

LEX Tokenization:
1. "9"  → Matches [0-9]+, yylval=9, returns NUMBER
2. "*"  → Matches '.', returns '*'
3. "3"  → Matches [0-9]+, yylval=3, returns NUMBER

YACC Parsing:
1. Sees NUMBER (9), matches expression: NUMBER
2. Sees '*', tries to match expression
   - Grammar expects '+' or '-' after expression
   - '*' doesn't match any production!
3. Calls yyerror("syntax error")
4. Prints "syntax error"
```

---

## Limitations

1. **No multiplication/division**: Only `+` and `-` are supported
2. **No parentheses**: Cannot group expressions like `(5+3)*2`
3. **No operator precedence**: All operations are left-associative and evaluated left-to-right
4. **Variable assignment incomplete**: Syntax is recognized but values aren't stored/used
5. **No negative numbers**: Input like `-5+3` would cause a syntax error
6. **Single expression only**: Program exits after first expression

---

## Comparison with Question 1

| Feature | Question 1 (calculator.c) | Question 3 (calculator2) |
|---------|---------------------------|------------------------|
| Parser Type | Manual recursive descent | YACC-generated |
| Input | Single digits only | Multi-digit integers |
| Operators | `+`, `*` | `+`, `-` |
| Precedence | `*` > `+` | Left-to-right only |
| Error Handling | Basic (exits on error) | Prints "syntax error" |

---

## Files Generated During Compilation

When you run `make -f Makefile3`, these files are created:

| File | Description |
|------|-------------|
| `calculator2.tab.c` | Generated YACC parser C code |
| `calculator2.tab.h` | Generated YACC header with token definitions |
| `lex.yy.c` | Generated LEX lexer C code |
| `calculator2` | Final executable |

To clean up:
```bash
make -f Makefile3 clean
```

---

## Key Concepts

### Token Attributes

In LEX:
- `yylval` stores the semantic value of a token
- For NUMBER tokens, `yylval` holds the integer value

In YACC:
- `$1`, `$2`, `$3` refer to semantic values of symbols on RHS
- `$$` is the semantic value being computed for LHS

### Left Recursion

The grammar uses **left recursion**:
```
expression: expression '+' NUMBER
```

This makes the parser associate operators to the left (left-to-right evaluation).

### Semantic Actions

C code embedded in grammar rules that executes when a rule is recognized. They compute values and perform side effects (like printing results).
