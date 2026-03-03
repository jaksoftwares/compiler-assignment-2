# Question 1: Recursive Descent Calculator

A simple recursive-descent parser for evaluating mathematical expressions containing addition (`+`) and multiplication (`*`) operators with correct operator precedence.

---

## Table of Contents

1. [Overview](#overview)
2. [What the File Does](#what-the-file-does)
3. [Functions Explained](#functions-explained)
4. [How It Works](#how-it-works)
5. [How to Run](#how-to-run)
6. [Examples](#examples)
7. [Grammar Rules](#grammar-rules)

---

## Overview

[`calculator.c`](calculator.c) implements a **recursive descent parser** - a top-down parsing technique where the parser uses a set of recursive functions to parse the input string. Each function corresponds to a production rule in the grammar.

The calculator evaluates expressions containing:
- **Single-digit integers** (0-9)
- **Addition** (`+`)
- **Multiplication** (`*`)

The parser follows **operator precedence**: multiplication (`*`) has higher precedence than addition (`+`).

---

## What the File Does

The program:
1. Prompts the user to enter a mathematical expression (e.g., `2*3+4`)
2. Reads the input as a string
3. Uses recursive descent parsing to evaluate the expression
4. Outputs the result

The parser implements the following grammar rules:
```
sum     → product ( '+' product )*
product → factor   ( '*'   factor )*
factor  → digit
```

---

## Functions Explained

### `int parseSum()`
**Lines:** [22-32](calculator.c:22)

**Purpose:** Parses addition expressions (sums).

**What it does:**
1. Calls [`parseProduct()`](#int-parseproduct) to get the first operand
2. Uses a while loop to check for `+` operators
3. For each `+` found:
   - Advances the pointer (`++x`)
   - Parses the next product (`parseProduct()`)
   - Adds it to the running result
4. Returns the final sum

**Returns:** The sum of all products separated by `+` operators.

---

### `int parseProduct()`
**Lines:** [34-44](calculator.c:34)

**Purpose:** Parses multiplication expressions (products).

**What it does:**
1. Calls [`parseFactor()`](#int-parsefactor) to get the first operand
2. Uses a while loop to check for `*` operators
3. For each `*` found:
   - Advances the pointer (`++x`)
   - Parses the next factor (`parseFactor()`)
   - Multiplies it with the running result
4. Returns the final product

**Returns:** The product of all factors separated by `*` operators.

---

### `int parseFactor()`
**Lines:** [46-57](calculator.c:46)

**Purpose:** Parses individual digits (factors).

**What it does:**
1. Checks if the current character is a digit (`'0'` to `'9'`)
2. If it is a digit:
   - Converts the character to its integer value (`*x++ - '0'`)
   - Advances the pointer to the next character
   - Returns the integer value
3. If it's not a digit:
   - Prints an error message
   - Exits the program with error code 1

**Returns:** The integer value of the digit.

---

### `int main()`
**Lines:** [59-68](calculator.c:59)

**Purpose:** Entry point of the program.

**What it does:**
1. Declares a character array to store user input
2. Prompts the user with instructions
3. Reads the input using `scanf()`
4. Sets the global pointer `x` to point to the beginning of the input
5. Calls [`parseSum()`](#int-parsesum) to evaluate the expression
6. Prints the result
7. Returns 0 to indicate successful execution

---

### Global Variable: `char *x`
**Line:** [20](calculator.c:20)

**Purpose:** A global pointer that keeps track of the current position in the input string being parsed.

**What it does:** 
- Points to the current character being processed
- Gets incremented as characters are consumed during parsing

---

## How It Works

### Parsing Technique: Recursive Descent

The parser works by:
1. **Starting at `main()`**: Sets up the input and calls `parseSum()`
2. **parseSum()**: Expects at least one product, then looks for `+` operators
3. **parseProduct()**: Expects at least one factor, then looks for `*` operators
4. **parseFactor()**: Expects a single digit and converts it to an integer

### Operator Precedence

The grammar inherently enforces operator precedence:
- `parseProduct()` is called by `parseSum()` - so products are evaluated first
- `parseFactor()` is called by `parseProduct()` - so factors (digits) are evaluated first
- Multiplication binds tighter than addition

**Example:** `2*3+4`
1. `parseSum()` calls `parseProduct()` → gets `2*3 = 6`
2. Sees `+`, advances pointer
3. Calls `parseProduct()` → gets `4`
4. Returns `6 + 4 = 10`

---

## How to Run

### Prerequisites

Ensure you have a C compiler installed. On Windows, you can use MinGW or Visual Studio. On Linux/macOS, use GCC.

### Compilation

Open a terminal and run:

```bash
gcc calculator.c -o calculator
```

This compiles [`calculator.c`](calculator.c) and creates an executable named `calculator`.

### Execution

Run the compiled program:

```bash
./calculator    # On Linux/macOS
calculator.exe  # On Windows
```

---

## Examples

### Example 1: Simple Addition
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 2+3
Result: 5
```

### Example 2: Simple Multiplication
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 2*3
Result: 6
```

### Example 3: Mixed Operations (Multiplication has higher precedence)
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 2*3+4
Result: 10
```

### Example 4: Multiple Operations
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 1+2*3+4
Result: 11
```
Calculation: `1 + (2*3) + 4 = 1 + 6 + 4 = 11`

### Example 5: All Multiplication
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 2*3*4
Result: 24
```

---

## Grammar Rules

The calculator implements the following grammar (in Backus-Naur Form):

```
<sum>     ::= <product> ( '+' <product> )*
<product> ::= <factor>   ( '*'   <factor> )*
<factor>  ::= '0' | '1' | '2' | '3' | '4' | '5' | '6' | '7' | '8' | '9'
```

### Equivalent Grammar Rules:

| Rule | Description |
|------|-------------|
| `sum → product ( '+' product )*` | A sum is a product, optionally followed by more products separated by `+` |
| `product → factor ( '*' factor )*` | A product is a factor, optionally followed by more factors separated by `*` |
| `factor → digit` | A factor is a single digit |

### Parse Tree Example

For input `2*3+4`:

```
       parseSum()
         │
    ┌────┴────┐
    │   '+'   │
 parseProduct() parseProduct()
    │              │
 parseFactor()  parseFactor()
    │              │
    '2'           '4'
         │
    ┌────┴────┐
    │   '*'   │
 parseFactor() parseFactor()
    │              │
    '3'           (nothing)
```

---

## Error Handling

The program handles one type of error:
- **Non-digit input**: If a character other than `0-9` is encountered where a digit is expected, it prints an error message and exits.

Example:
```
Enter an expression (digits with + and * only, e.g. 2*3+4): 2+ab
Expected digit but found a
```

---

## Limitations

1. **Single-digit operands only**: The parser only accepts single digits (0-9), not multi-digit numbers
2. **No parentheses**: Expressions like `(2+3)*4` are not supported
3. **No negative numbers**: The parser doesn't handle unary minus
4. **No whitespace handling**: The input must be a continuous string without spaces

---

## File Information

- **File:** [`calculator.c`](calculator.c)
- **Language:** C
- **Lines:** 68
- **Purpose:** Recursive descent parser for basic arithmetic expressions
