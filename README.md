# ICS 2401 Compiler Construction - Assignment II



## Prerequisites
Before running any code, make sure you have the following installed:
```bash
sudo apt-get update
sudo apt-get install gcc flex bison make -y
```

---

## Question 1 - Recursive Descent Calculator

A simple calculator using recursive descent parsing that handles `+` and `*` with correct operator precedence.

### Run it
```bash
gcc calculator.c -o calculator
./calculator
```

### Example
```
Enter an expression: 2*3+4
Result: 10
```

---

## Question 2 - LEX and YACC 'a' Counter

An interpreter that counts the number of `a`'s in an input string using the grammar:
```
S → aS | b
```

### Run it
```bash
make -f Makefile2
./assignment2
```

### Examples
```
Enter the string: b       → No of a's: 0
Enter the string: ab      → No of a's: 1
Enter the string: aab     → No of a's: 2
Enter the string: aabb    → invalid string
```

---

## Question 3 - LEX and YACC Calculator

A calculator that handles addition and subtraction using LEX and YACC specification files.

### Run it
```bash
make -f Makefile3
./calculator2
```

### Examples
```
33+54   → =87
77-32   → =45
9-99    → =-90
9*3     → syntax error
```

---

## Project Structure
```
compiler_assignment/
├── calculator.c      # Question 1 - Recursive descent parser
├── assignment2.l     # Question 2 - LEX file
├── assignment2.y     # Question 2 - YACC file
├── Makefile2         # Question 2 - Build file
├── calculator2.l     # Question 3 - LEX file
├── calculator2.y     # Question 3 - YACC file
├── Makefile3         # Question 3 - Build file
└── README.md         # This file
```