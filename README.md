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
 <table>
        <tr>
            <th>Mnemonic</th>
            <th>Opcode</th>
            <th>Operand</th>
            <th>Description</th>
        </tr>
        <tr><td>ldc</td><td>0</td><td>value</td><td>Load accumulator with value</td></tr>
        <tr><td>adc</td><td>1</td><td>value</td><td>Add value to accumulator</td></tr>
        <tr><td>ldl</td><td>2</td><td>offset</td><td>Load local</td></tr>
        <tr><td>stl</td><td>3</td><td>offset</td><td>Store local</td></tr>
        <tr><td>ldnl</td><td>4</td><td>offset</td><td>Load non-local</td></tr>
        <tr><td>stnl</td><td>5</td><td>offset</td><td>Store non-local</td></tr>
        <tr><td>add</td><td>6</td><td>-</td><td>Addition</td></tr>
        <tr><td>sub</td><td>7</td><td>-</td><td>Subtraction</td></tr>
        <tr><td>shl</td><td>8</td><td>-</td><td>Shift left</td></tr>
        <tr><td>shr</td><td>9</td><td>-</td><td>Shift right</td></tr>
        <tr><td>adj</td><td>10</td><td>value</td><td>Adjust stack pointer (SP)</td></tr>
        <tr><td>a2sp</td><td>11</td><td>-</td><td>Transfer A to SP</td></tr>
        <tr><td>sp2a</td><td>12</td><td>-</td><td>Transfer SP to A</td></tr>
        <tr><td>call</td><td>13</td><td>offset</td><td>Call procedure</td></tr>
        <tr><td>return</td><td>14</td><td>-</td><td>Return from procedure</td></tr>
        <tr><td>brz</td><td>15</td><td>offset</td><td>Branch if zero</td></tr>
        <tr><td>brlz</td><td>16</td><td>offset</td><td>Branch if less than zero</td></tr>
        <tr><td>br</td><td>17</td><td>offset</td><td>Unconditional branch</td></tr>
        <tr><td>HALT</td><td>18</td><td>-</td><td>Stop execution</td></tr>
        <tr><td>SET</td><td>-</td><td>value</td><td>Set label to a specific value (optional)</td></tr>
    </table>
    
<h1>Listing File Format</h1>
    <p>The assembler produces a listing file, showing the values stored at each address. The format consists of an address followed by zero or one 32-bit values represented as 8 hex characters.</p>
</body>
