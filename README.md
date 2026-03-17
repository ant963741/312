# SIC/XE Assembler

A two-pass assembler for the SIC (Simplified Instructional Computer) and SIC/XE architecture, implemented in C++. This project was developed as a coursework project for the **Systems Programming** course at Chung Yuan Christian University.

## Features

- **Instruction Table Loading** - Loads and manages opcode/instruction tables used during assembly
- **Lexical Analysis (Tokenizer)** - Breaks assembly source code into tokens and classifies them using symbol/literal tables with hash-based lookup
- **SIC Assembler (Two-Pass)**
  - Pass 1: Assigns addresses, builds the symbol table
  - Pass 2: Generates object code and produces the final listing
- **SIC/XE Assembler (Two-Pass)**
  - Supports Format 1, 2, 3, and 4 instructions
  - Handles addressing modes: immediate (`#`), indirect (`@`), indexed (`,X`)
  - Supports extended format (`+`) for 4-byte instructions
  - Generates relocatable object code

## Project Structure

```
.
├── main.cpp            # Main source code (assembler implementation)
├── SIC_input.txt       # Sample SIC assembly source
├── SIC_output.txt      # Expected SIC assembler output
├── SICXE_input.txt     # Sample SIC/XE assembly source
├── SICXE_output.txt    # Expected SIC/XE assembler output
├── Table*.table        # Instruction tables (loaded at runtime)
├── Makefile            # Build script
└── README.md
```

## Build & Run

### Prerequisites

- A C++ compiler supporting C++11 or later (g++, MinGW, MSVC, etc.)

### Compile

```bash
# Using Make
make

# Or compile directly with g++
g++ -o assembler main.cpp -std=c++11
```

### Run

```bash
./assembler
```

You will see an interactive menu:

```
***************************************
 0. QUIT                              *
 1: open table                        *
 2: Lexical Analysis                  *
 3: SIC                               *
 4: SICXE                             *
***************************************
Input a choice(0, 1, 2, 3, 4):
```

### Usage Example

**Step 1:** Load instruction tables first (option `1`). Enter the table number when prompted.

**Step 2:** Choose an operation:
- Option `2` - Perform lexical analysis on an input file
- Option `3` - Assemble a SIC program (e.g., enter `SIC_input`)
- Option `4` - Assemble a SIC/XE program (e.g., enter `SICXE_input`)

### Sample Output (SIC)

```
Line   Loc    Source statement          Object code

  5   1000   COPY    START   1000
 10   1000   FIRST   STL     RETADR      141033
 15   1003   CLOOP   JSUB    RDREC       482039
 20   1006           LDA     LENGTH      001036
 25   1009           COMP    ZERO        281030
 30   100C           JEQ     ENDFIL      301015
 ...
```

### Sample Output (SIC/XE)

```
Line  Location  Source code                              Object code
----  --------  -------------------------                -----------
  25  000F      add       var1                           1B2FEE
  30  0012      +addf     var2                           5B100003
  35  0016      addr      a,b                            9003
  40  0018      +and      var1                           43100000
  ...
```

## Architecture Overview

The assembler is built around a `Token` class that manages the entire assembly pipeline:

1. **Table Management** - Opcodes and directives are stored in indexed tables with hash-based insertion for O(1) lookup
2. **Tokenization** - Source lines are split into labels, instructions, and operands
3. **Symbol Resolution** - A symbol table tracks label addresses across two passes
4. **Code Generation** - Object codes are computed based on opcode format, addressing mode, and operand values

## Technical Details

| Item | Detail |
|------|--------|
| Language | C++ |
| Standard | C++11 |
| Lines of Code | ~1900 |
| Development Time | 1 month |
| Developer | TSAI (solo project) |

## License

This project is provided for educational and portfolio purposes.

## Contact

- Email: grant@gmail.com
