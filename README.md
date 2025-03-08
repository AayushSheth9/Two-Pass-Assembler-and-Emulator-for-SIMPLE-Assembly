# Two-Pass-Assembler-and-Emulator-for-SIMPLE-Assembly
The aim of this project is to write a two pass assembler and emulator for an extended SIMPLE instruction set.
Assembly Language
This assembly language is for a machine with four registers,
 Two registers, A & B, arranged as an internal stack.
 A program counter, PC
 A stack pointer, SP
These registers are 32 bits in size. Instructions have either no operands or a single operand. The
operand is a signed 2's complement value. The encoding uses the bottom 8 bits for opcode and
the upper 24 bits for operand.
As with most assembly languages, this is line based (one statement per line). Comments begin
with a ';' and anything on the line after the ';' is ignored. Blank lines and lines containing only a
comment are permitted (and ignored). White space (' ' and tabs) are permitted at the beginning of
a line (and ignored). Label definitions consist of the label name followed by a ':', and an optional
statement (there is not necessarily a space between the ':' and the statement). A label use is just
the label name. For branch instructions label use should calculate the branch displacement. For
non-branch instructions, the label value should be used directly. A valid label name is an
alphanumeric string beginning with a letter . An operand is either a label or a number, the
number can be decimal, hex or octal.
