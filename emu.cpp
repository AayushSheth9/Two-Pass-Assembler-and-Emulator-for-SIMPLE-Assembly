// Project of Aayush Sheth:2301CS02
//  Course name: CS2102: Digital Logic and Computer Organisation
#include <bits/stdc++.h>
using namespace std;
#define vi vector<int>
#define fr(i, a, b) for (int i = a; i < b; i++)
struct memdata // recent memory changes format
{
  int address;
  int previousval;
};
memdata memop;                            // stores memory changes
int memory[1 << 25];                      // memory available
vi maccode;                               // stores all the machine codes from the binary file
int rega = 0, regb = 0, pctr = 0, sp = 0; // registers
int line = 0;
map<string, pair<string, int>> instructions; // the given instruction set
// Functions used in this program
void initialise_instruct();                     // Initialise the instruction table
void executecmd(string command);                // Exeecutes the command
void input(string filename);                    // Takes input from the binary file
bool executeinst(int current_line, string cmd); // Executes line by line the command
void runcmd(string cmd);
string dectohex(int number); // helper function

// Commands available:
void isa();
void trace();
void read();
void write();
void before();
void after();
void wipe();

void halt(int value)
{
  return;
}

void br(int offset)
{
  pctr += offset;
  pctr--;
}

void brlz(int offset)
{
  if (rega < 0){
    pctr += offset;
    pctr--;
  }
    
    
}

void brz(int offset)
{
  if (rega == 0){
    pctr += offset;
    pctr--;
  }
    
}

void ret(int value)
{
  pctr = rega;
  rega = regb;
}

void call(int offset)
{
  regb = rega;
  rega = pctr;
  pctr += offset;
  pctr--;
}

void sp2a(int value)
{
  regb = rega;
  rega = sp;
}

void a2sp(int value)
{
  sp = rega;
  rega = regb;
}

void adj(int value)
{
  sp += value;
}

void shr(int value)
{ 

  rega = (regb >> rega);
}

void shl(int value)
{
  rega = (regb << rega);
}

void sub(int value)
{
  rega = regb - rega;
}

void add(int value)
{
  rega += regb;
}

void stnl(int offset)
{
  memop = {sp + offset, memory[sp + offset]};
  memory[rega + offset] = regb;
}

void ldnl(int offset)
{
  rega = memory[rega + offset];
  memop = {sp + offset, 0};
}

void stl(int offset)
{
  memop = {sp + offset, memory[sp + offset]};
  memory[sp + offset] = rega;
  rega = regb;
}

void ldl(int offset)
{
  regb = rega;
  rega = memory[sp + offset];
  memop = {sp + offset, 0};
}

void adc(int value)
{
  rega += value;
}

void ldc(int value)
{
  regb = rega;
  rega = value;
}
void initialise_instruct()
{
  // operand type are of 3 types:
  // 0 operand type means instruction takes no operand
  // 1 operand type means instruction takes 1 operand and is value
  // -1 operand type means instruction takes 1 operand and is offset
  // -1 OPCODE refers that the instruction has no OPCODE
  instructions.insert({"data", {"", 1}});     /*Reserve a memory location, initialized to the value specified*/
  instructions.insert({"ldc", {"00", 1}});    // Load accumulator with the value specified
  instructions.insert({"adc", {"01", 1}});    // Add the value specified to theaccumulator
  instructions.insert({"ldl", {"02", -1}});   // Load local
  instructions.insert({"stl", {"03", -1}});   // Store local
  instructions.insert({"ldnl", {"04", -1}});  // Load non-local
  instructions.insert({"stnl", {"05", -1}});  // Store non-local
  instructions.insert({"add", {"06", 0}});    // Addition
  instructions.insert({"sub", {"07", 0}});    // Subtraction
  instructions.insert({"shl", {"08", 0}});    // Shift left
  instructions.insert({"shr", {"09", 0}});    // Shift right
  instructions.insert({"adj", {"0a", 1}});    // Adjust SP
  instructions.insert({"a2sp", {"0b", 0}});   // Transfer A to SP;
  instructions.insert({"sp2a", {"0c", 0}});   // Transfer SP to A
  instructions.insert({"call", {"0d", -1}});  // Call procedure
  instructions.insert({"return", {"0e", 0}}); // Return from procedure
  instructions.insert({"brz", {"0f", -1}});   // If accumulator is zero, branch to specified offset
  instructions.insert({"brlz", {"10", -1}});  // If accumulator is less than zero, branch to specified offset
  instructions.insert({"br", {"11", -1}});    // Branch to specified offset
  instructions.insert({"HALT", {"12", 0}});   // Stop the emulator.
  instructions.insert({"SET", {"", 1}});      // Set the label on this line to the specified value
}
void input(string filename)
{
  ifstream file(filename, ios::in | ios::binary);
  if (!file)
  {
    cout << "Error opening the file: " << filename << endl;
    return;
  }
  unsigned int curr;
  int ct = 0;
  while (file.read((char *)&curr, sizeof(int)))
  { // reads data(4 bytes) from the binary file and stores it as an integer in curr.
    memory[ct] = curr;
    maccode.push_back(curr);
    ct++;
  }
}
bool executeinst(int current_line, string cmd)
{ 
  int opcode = (0x000000FF & current_line);
  int operand = (0xFFFFFF00 & current_line);
  operand >>= 8;
  int temp = operand;
  if(temp & 0x8000){
    temp |= 0xffff0000;
  }
  operand = temp; 
  line++;
  string inst = "";
  if (opcode == 0)
  {
    ldc(operand);
    inst = "ldc";
  }
  else if (opcode == 1)
  {
    adc(operand);
    inst = "adc";
  }
  else if (opcode == 2)
  {
    ldl(operand);
    inst = "ldl";
  }
  else if (opcode == 3)
  {
    stl(operand);
    inst = "stl";
  }
  else if (opcode == 4)
  {
    ldnl(operand);
    inst = "ldnl";
  }
  else if (opcode == 5)
  {
    stnl(operand);
    inst = "stnl";
  }
  else if (opcode == 6)
  {
    add(operand);
    inst = "add";
  }
  else if (opcode == 7)
  {
    sub(operand);
    inst = "sub";
  }
  else if (opcode == 8)
  {
    shl(operand);
    inst = "shl";
  }
  else if (opcode == 9)
  {
    shr(operand);
    inst = "shr";
  }
  else if (opcode == 10)
  {
    adj(operand);
    inst = "adj";
  }
  else if (opcode == 11)
  {
    a2sp(operand);
    inst = "a2sp";
  }
  else if (opcode == 12)
  {
    sp2a(operand);
    inst = "sp2a";
  }
  else if (opcode == 13)
  {
    call(operand);
    inst = "call";
  }
  else if (opcode == 14)
  {
    ret(operand);
    inst = "ret";
  }
  else if (opcode == 15)
  {
    brz(operand);
    inst = "brz";
  }
  else if (opcode == 16)
  {
    brlz(operand);
    inst = "brlz";
  }
  else if (opcode == 17)
  {
    br(operand);
    inst = "br";
  }
  else if (opcode == 18)
  {
    halt(operand);
    inst = "halt";
  }
  if (pctr < 0)
  {
    cout << "Error detected. Check your codeh" << endl;
    exit(0);
  }
  if (pctr > maccode.size())
  {
    cout << "Error detected. Check your codef" << endl;
    exit(0);
  }
  if(line>(1<<25)){
    cout<<"Error detected. Segmentation fault. There exists an infinite loop."<<endl;
  }
  if (cmd == "-read")
  {
    if (opcode == 2 || opcode == 4)
      read();
  }
  else if (cmd == "-write")
  {
    if (opcode == 3 || opcode == 5)
      write();
  }
  else if (cmd == "-trace")
  { 
    trace();
    if (instructions[inst].second == 1 || instructions[inst].second == -1)
    {
      cout << inst << " " << dectohex(operand) << endl;
      ;
    }
    else
    { 
      cout << inst << endl;
    }
  }
  else if (opcode == 18)
  {
    return true;
  }
  return false;
}
void runcmd(string cmd)
{
  for (; pctr < maccode.size(); pctr++)
  {
    int current_line = maccode[pctr];
    if (executeinst(current_line, cmd))
    {
      break;
    }
  }
}
void read()
{
  cout << "Reading of memory detected at:" << endl;
  cout << "Memory address: " << dectohex(memop.address) << endl;
  cout << "Value accessed: " << dectohex(rega) << endl;
}
void write()
{
  cout << "Writing of memory detected at:" << endl;
  cout << "Memory address: " << dectohex(memop.address) << endl;
  cout << "Previous value: " << dectohex(memop.previousval) << endl;
  cout << "New value: " << dectohex(memory[memop.address]) << endl;
}
void trace()
{
  cout << "A = " << dectohex(rega) << ", B = " << dectohex(regb) << ", PC = " << dectohex(pctr) << ", SP = " << dectohex(sp) << " ";
}
void wipe()
{
  regb = 0;
  rega = 0;
  sp = 0;
  pctr = 0;
  cout << "Registers resetted" << endl;
}
void isa()
{
  cout << "Each instruction is represented in the following format:" << endl;
  cout << "Mnemonic -> Opcode -> Operand" << endl;
  int ct = 0;
  auto it = instructions.begin();
  while (it != instructions.end())
  {
    cout << ct + 1 << ". " << it->first << " -> " << (it->second).first << " -> ";
    if ((it->second).second == 0)
    {
      cout << "none" << endl;
    }
    else if ((it->second).second == 1)
    {
      cout << "value" << endl;
    }
    else
    {
      cout << "offset" << endl;
    }
    it++;
    ct++;
  }
}
void before()
{
  cout << "Memory dump before execution." << endl;
  fr(i, 0, maccode.size())
  {
    cout << dectohex(i) << " " << dectohex(maccode[i]) << endl;
    ;
  }
}
void after()
{
  cout << "Memory dump after execution." << endl;
  fr(i, 0, maccode.size())
  {
    cout << dectohex(i) << " " << dectohex(memory[i]) << endl;
    
  }
}
void executecmd(string command)
{
  if (command == "-read")
  {
    runcmd(command);
  }
  else if (command == "-write")
  {
    runcmd(command);
  }
  else if (command == "-trace")
  {
    runcmd(command);
  }
  else if (command == "-wipe")
  {
    wipe();
  }
  else if (command == "-isa")
  {
    isa();
  }
  else if (command == "-before")
  {
    runcmd(command);
    before();
  }
  else if (command == "-after")
  {
    runcmd(command);
    after();
  }
  else
  {
    cout << "Invalid command. Check the command list and choose one amongst them." << endl;
    exit(0);
  }
  cout << "Program executed!" << endl;
}

string dectohex(int number)
{
  stringstream ss;
  ss << uppercase << setw(8) << setfill('0') << hex << number;
  return ss.str();
}
int main(int argc, char *argv[])
{
  if (argc <= 2)
  {
    cout << "Format is incorrect." << endl;
    cout << "Follow the format as given below: " << endl;
    cout << "./emu 'command' filename.o" << endl;
    cout << "Choose one among the following commands:" << endl;
    cout << "-read   shows memory reads" << endl;
    cout << "-write  shows memory writes" << endl;
    cout << "-trace  shows instruction trace" << endl;
    cout << "-wipe   wipes written flags before execution" << endl;
    cout << "-isa    displays ISA" << endl;
    cout << "-before shows memory dump before execution" << endl;
    cout << "-after  shows memory dump after execution" << endl;
    return 0;
  }
  string filename = argv[2];
  input(filename);
  initialise_instruct();
  executecmd(argv[1]);
  return 0;
}