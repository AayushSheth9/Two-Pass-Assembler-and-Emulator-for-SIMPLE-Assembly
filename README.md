# Two-Pass-Assembler-and-Emulator-for-SIMPLE-Assembly
The aim of this project is to write a two pass assembler and emulator for an extended SIMPLE instruction set.
 <h1>SIMPLE Assembly Language</h1>
    <p>This assembly language is for a machine with four registers:</p>
    <ul>
        <li>Two registers, A & B, arranged as an internal stack.</li>
        <li>A program counter (PC).</li>
        <li>A stack pointer (SP).</li>
    </ul>
    <p>These registers are 32 bits in size. Instructions have either no operands or a single operand.</p>
    <h2>Syntax and Labels</h2>
    <p>Each statement consists of a mnemonic (instruction name) and an optional operand (number or label).</p>
    <p>Comments start with a semicolon (<code>;</code>) and anything after it is ignored.</p>
    <p>Valid labels are alphanumeric and start with a letter.</p>
    
<h1>Instruction Set </h1>
Mnemonic   | Opcode | Operand | Description
---------------------------------------------
ldc        | 0      | value   | Load accumulator with value
adc        | 1      | value   | Add value to accumulator
ldl        | 2      | offset  | Load local
stl        | 3      | offset  | Store local
ldnl       | 4      | offset  | Load non-local
stnl       | 5      | offset  | Store non-local
add        | 6      |         | Addition
sub        | 7      |         | Subtraction
shl        | 8      |         | Shift left
shr        | 9      |         | Shift right
adj        | 10     | value   | Adjust stack pointer (SP)
a2sp       | 11     |         | Transfer A to SP
sp2a       | 12     |         | Transfer SP to A
call       | 13     | offset  | Call procedure
return     | 14     |         | Return from procedure
brz        | 15     | offset  | Branch if zero
brlz       | 16     | offset  | Branch if less than zero
br         | 17     | offset  | Unconditional branch
HALT       | 18     |         | Stop execution
SET        |        | value   | Set label to a specific value (optional)
    </pre>
    
<h1>Listing File Format</h1>
    <p>The assembler produces a listing file, showing the values stored at each address. The format consists of an address followed by zero or one 32-bit values represented as 8 hex characters.</p>
</body>
