# Question 2: LEX and YACC 'a' Counter

A LEX and YACC (Flex and Bison) based interpreter that counts the number of 'a' characters in an input string using a specific grammar.

---

## Table of Contents

1. [Overview](#overview)
2. [File Structure](#file-structure)
3. [LEX File (assignment2.l)](#lex-file-assignment2l)
   - [What is LEX?](#what-is-lex)
   - [Code Explanation](#lex-code-explanation)
4. [YACC File (assignment2.y)](#yacc-file-assignment2y)
   - [What is YACC?](#what-is-yacc)
   - [Code Explanation](#yacc-code-explanation)
5. [How It Works](#how-it-works)
6. [How to Run](#how-to-run)
7. [Examples](#examples)
8. [Grammar Rules](#grammar-rules)
9. [Detailed Flow](#detailed-flow)

---

## Overview

This program uses **LEX** (Lexical Analyzer) and **YACC** (Yet Another Compiler Compiler) to:
1. Tokenize an input string containing only 'a' and 'b' characters
2. Validate the string against a specific grammar
3. Count the number of 'a' characters in the string

**Grammar:**
```
S → aS | b
```

This grammar means:
- An 'S' can be: one or more 'a' characters followed by a single 'b'
- The string must end with exactly one 'b'
- Any number of 'a's can precede the 'b'

---

## File Structure

| File | Description |
|------|-------------|
| [`assignment2.l`](assignment2.l) | LEX specification file - tokenizes input |
| [`assignment2.y`](assignment2.y) | YACC specification file - defines grammar and actions |
| [`Makefile2`](Makefile2) | Build file to compile the project |

---

## LEX File (assignment2.l)

### What is LEX?

**LEX** (or Flex) is a lexical analyzer generator. It takes a set of regular expression patterns and generates C code that can scan input and identify tokens matching those patterns.

### LEX Code Explanation

```c
%{
    #include "assignment2.tab.h"
%}
```

This C code block is copied directly to the generated lexer. It includes the YACC header file to share token definitions.

---

### Pattern Matching Rules

```lex
[a]     { yylval='a'; return a; }
[b]     { yylval='b'; return b; }
\n      { return NL; }
[ \t]   { /* ignore whitespace */ }
.       { return yytext[0]; }
```

| Pattern | Action | Description |
|---------|--------|-------------|
| `[a]` | `yylval='a'; return a;` | Matches character 'a', stores it in `yylval`, returns token `a` |
| `[b]` | `yylval='b'; return b;` | Matches character 'b', stores it in `yylval`, returns token `b` |
| `\n` | `return NL;` | Matches newline, returns token `NL` (end of line) |
| `[ \t]` | `/* ignore whitespace */` | Ignores spaces and tabs |
| `.` | `return yytext[0];` | Matches any other character and returns it |

---

### `yywrap()` Function

```c
int yywrap()
{
    return 1;
}
```

**What it does:**
- Called when the lexer reaches end of input (EOF)
- Returns `1` to indicate there are no more input files to process
- Default implementation returns 1 to signal end of input

---

## YACC File (assignment2.y)

### What is YACC?

**YACC** (or Bison) is a parser generator. It takes a grammar specification and generates a C parser that can recognize sentences in that grammar.

### YACC Code Explanation

#### Header Section

```c
%{
    #include<stdio.h>
    #include<stdlib.h>

    extern int yylex();
    int yyerror(char *error_msg);
    int a_counter = 0;
%}
```

| Code | Description |
|------|-------------|
| `#include<stdio.h>` | Standard input/output library |
| `#include<stdlib.h>` | Standard library for `exit()` |
| `extern int yylex();` | Declares the lexer function (defined in LEX) |
| `int yyerror(char *error_msg);` | Declares error handling function |
| `int a_counter = 0;` | Global variable to count 'a' characters |

---

#### Token Declarations

```yacc
%token a b NL
```

Declares three token types:
- `a` - the character 'a'
- `b` - the character 'b'  
- `NL` - newline (end of line)

---

### Grammar Rules

```yacc
program: stmt
       ;

stmt: S NL
        {
            printf("No of a's: %d\n", a_counter);
            exit(0);
        }
    ;

S: a S  { ++a_counter; }
 | b    
 ;
```

#### Rule 1: `program`
```yacc
program: stmt
       ;
```
- A program consists of one statement (`stmt`)
- Empty production (can be nothing)

#### Rule 2: `stmt` (Statement)
```yacc
stmt: S NL
        {
            printf("No of a's: %d\n", a_counter);
            exit(0);
        }
    ;
```
- A statement is an 'S' followed by a newline
- **Action:** When newline is reached, print the count of 'a's and exit

#### Rule 3: `S` (The main non-terminal)
```yacc
S: a S  { ++a_counter; }
 | b    
 ;
```

| Production | Action | Description |
|------------|--------|-------------|
| `S: a S` | `{ ++a_counter; }` | If 'S' is 'a' followed by another 'S', increment counter |
| `S: b` | (none) | If 'S' is just 'b', do nothing |

This implements the grammar `S → aS | b`:
- `a S` - an 'a' followed by another valid string
- `b` - just the character 'b' (base case)

---

### Error Handling

```c
int yyerror(char *msg)
{
    printf("invalid string\n");
    exit(0);
}
```

**What it does:**
- Called by YACC when a syntax error is encountered
- Prints "invalid string" message
- Exits gracefully (with code 0)

---

### Main Function

```c
int main()
{
    printf("Enter the string\n");
    yyparse();
    return 0;
}
```

**What it does:**
1. Prints prompt "Enter the string"
2. Calls `yyparse()` to start parsing
3. Returns 0 (though program usually exits inside `stmt` rule)

---

## How It Works

### Step-by-Step Execution Flow

1. **User Input**: User types a string ending with newline (e.g., `aab<Enter>`)

2. **LEX (Lexical Analysis)**: 
   - Scans the input character by character
   - Identifies tokens: `a`, `a`, `b`, `NL`
   - Returns each token to YACC

3. **YACC (Syntax Analysis)**:
   - Receives tokens from LEX
   - Tries to match them against grammar rules
   - For each 'a' token matched with `a S` rule, increments `a_counter`
   - When `b` is matched with the `b` rule, it completes successfully
   - When `NL` is matched with `stmt: S NL`, prints the count

4. **If Invalid**: If the string doesn't match `a*b` pattern (e.g., `aabb`), YACC calls `yyerror()` which prints "invalid string"

---

## How to Run

### Prerequisites

Ensure you have the following installed:
- **Flex** (LEX lexer generator)
- **Bison** (YACC parser generator)
- **GCC** (C compiler)

On Linux (Ubuntu/Debian):
```bash
sudo apt-get update
sudo apt-get install gcc flex bison make -y
```

On Windows:
- Install MinGW or use WSL (Windows Subsystem for Linux)

---

### Build and Run

#### Step 1: Compile using Makefile

```bash
make -f Makefile2
```

This command:
1. Runs **Bison** on [`assignment2.y`](assignment2.y) to generate `assignment2.tab.c` and `assignment2.tab.h`
2. Runs **Flex** on [`assignment2.l`](assignment2.l) to generate `lex.yy.c`
3. Compiles both to object files
4. Links them together with the Flex library (`-lfl`) to create the executable `assignment2`

#### Step 2: Run the executable

```bash
./assignment2    # Linux/macOS
assignment2.exe   # Windows
```

---

### Manual Compilation (Step by Step)

If you want to see what's generated:

```bash
# Generate parser from YACC
bison -d assignment2.y

# Generate lexer from LEX
flex assignment2.l

# Compile
gcc -c assignment2.tab.c -w
gcc -c lex.yy.c

# Link
gcc -lfl -o assignment2 assignment2.tab.o lex.yy.o
```

---

## Examples

### Example 1: No 'a's (just 'b')
```
Enter the string
b
No of a's: 0
```

**Explanation:** String "b" matches grammar `S → b`, no 'a' found.

---

### Example 2: One 'a'
```
Enter the string
ab
No of a's: 1
```

**Explanation:** String "ab" matches grammar `S → aS` where S = "b", one 'a' found.

---

### Example 3: Two 'a's
```
Enter the string
aab
No of a's: 2
```

**Explanation:** String "aab" matches grammar `S → aS` where S = "ab", two 'a's found.

---

### Example 4: Invalid - Multiple 'b's
```
Enter the string
aabb
invalid string
```

**Explanation:** String "aabb" doesn't match grammar. After parsing "aab", the second 'b' cannot be matched - the grammar only allows one 'b' at the end.

---

### Example 5: Invalid - Only 'a's
```
Enter the string
aaa
invalid string
```

**Explanation:** String "aaa" doesn't match grammar because there's no 'b' at the end. The grammar requires exactly one 'b'.

---

## Grammar Rules

### BNF (Backus-Naur Form)

```
<S>     ::= <a> <S> | <b>
<a>     ::= 'a'
<b>     ::= 'b'
<NL>    ::= newline
<stmt>  ::= <S> <NL>
<program> ::= <stmt>
```

### Meaning

- **S → aS**: A valid string can be 'a' followed by another valid string
- **S → b**: A valid string can be just 'b' (the terminal case)
- This means valid strings are: `b`, `ab`, `aab`, `aaab`, `aaa...b` (any number of 'a's followed by exactly one 'b')

### Derivation Examples

**Input: `ab`**
```
S → aS    (using S → aS)
  → a b   (using S → b)
```

**Input: `aab`**
```
S → aS    (using S → aS)
  → a aS  (using S → aS)
  → a a b (using S → b)
```

---

## Detailed Flow

### For Input "aab":

```
Input: aab\n

LEX Token Stream:
1. 'a' → token a
2. 'a' → token a
3. 'b' → token b
4. '\n' → token NL

YACC Parsing:
1. Sees token 'a', tries rule S → a S, increments a_counter (now 1)
2. Sees token 'a', tries rule S → a S, increments a_counter (now 2)
3. Sees token 'b', tries rule S → b, completes
4. Sees token NL, matches stmt: S NL, prints "No of a's: 2", exits
```

### For Input "aabb" (Invalid):

```
Input: aabb\n

YACC Parsing:
1. Sees token 'a', tries rule S → a S, increments a_counter (now 1)
2. Sees token 'a', tries rule S → a S, increments a_counter (now 2)
3. Sees token 'b', tries rule S → b, completes S
4. Next token is 'b' - but parser expects NL!
5. Syntax error! Calls yyerror()
6. Prints "invalid string", exits
```

---

## Limitations

1. **Single line input only**: The program expects input ending with newline
2. **No error recovery**: After an error, the program exits immediately
3. **Case sensitive**: Only lowercase 'a' and 'b' are accepted
4. **No spaces in middle**: While whitespace is ignored, having spaces between 'a' and 'b' may cause issues
5. **Must end with 'b'**: The grammar requires exactly one 'b' at the end

---

## Files Generated During Compilation

When you run `make -f Makefile2`, these files are created:

| File | Description |
|------|-------------|
| `assignment2.tab.c` | Generated YACC parser C code |
| `assignment2.tab.h` | Generated YACC header with token definitions |
| `lex.yy.c` | Generated LEX lexer C code |
| `assignment2` | Final executable |

To clean up generated files:
```bash
make -f Makefile2 clean
```

---

## Key Concepts

### Token
A token is a meaningful element in a programming language (like a keyword, identifier, or symbol). In this program:
- Token `a` represents the character 'a'
- Token `b` represents the character 'b'
- Token `NL` represents newline

### Non-terminal
A non-terminal is a grammar symbol that can be replaced by other symbols. In this program:
- `program`, `stmt`, and `S` are non-terminals

### Terminal
A terminal is a symbol that cannot be broken down further. In this program:
- `a`, `b`, and `NL` are terminals

### Semantic Action
A semantic action is C code that executes when a particular grammar rule is recognized. In this program:
- `{ ++a_counter; }` is a semantic action that increments the counter
- `{ printf("No of a's: %d\n", a_counter); exit(0); }` prints and exits
