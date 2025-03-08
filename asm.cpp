// Project of 2301CS02: Aayush Sheth
// Course name: CS2102: Digital Logic and Computer Organisation
#include <bits/stdc++.h>
using namespace std;
#define fr(i, a, b) for (int i = a; i < b; i++)
#define vi vector<int>
#define vs vector<string>
// Different functions used in 2 pass assembler:
struct line
{
  int pctr;
  string label, instruct, operand, original_op;
};
struct List
{
  string add, statement, machine;
};
map<string, pair<string, int>> instructions; // stores mnemonic,opcode and operand type
vs all_lines;                                // stores all lines of the input file
map<int, string> comments;                   // stores all the comments with its line number
map<string, pair<int, int>> label_table;     // stores all labels with adress and line number
vector<pair<int, string>> error_table;       // stores the error description and line number the error is found in
map<string, vi> label_location;
vector<pair<int, string>> warning_table;
vector<line> line_info;
vector<List> list_info;
vector<string> maccode;
map<string, string> var_set;

void initialise_instruct();  // initialise the OP table
void first_pass(const vector<string> &all_lines); // first pass of the assembler
void second_pass();
void show_err_warn(bool &flag);
vs splitIntoWords(string curr_line, int line_number);                // splits the line into diff words omitting the blank space in between
void labelcheck(string label, int line_number, int program_counter); // Checks the error assoicated with label and deals with it correspondingly
bool labelvalidity(string label);                                    // checks if label is valid or not
bool alphabetchecker(char c);                                        // check whether the character is alphabet or not
bool digitchecker(char c);                                           // check whether the given character is a digit or not
void errors_insert(int line_number, string error);                   // fills the error table
void warnings_insert(int line_number, string warning);
void validateMnemonic(string instname, int line_counter, int _program_counter, string &operand, bool &checker, bool flag);
string validateOperand(string operand, int line_number);
bool octalcheck(string number);
bool hexcheck(string number);
bool deccheck(string number);
string octaltodec(string number);
string hextodec(string number);
vs splitIntoWords(string curr_line, int line_number);
void macforoffset(string mnemonic, string operand, int program_counter, string &maccode);
void macforvaluecode(string mnemonic, string operand, int program_counter, string &maccode);
void macfordataorset(string operand, string &maccode);
void macfornooperand(string mnemonic, string &maccode);
string dectohex(int number);
long long power(int a, int b);
char CaptoSm(char capital)
{
  return capital + 32;
}

vs splitIntoWords(string curr_line, int line_number)
{
  if (curr_line.empty())
    return {};
  vector<string> words;
  stringstream lineStream(curr_line);
  string curr_word;
  while (lineStream >> curr_word)
  {
    if (curr_word.empty())
      continue;
    if (curr_word[0] == ';')
      break;
    size_t index1 = curr_word.find(':');
    size_t index2 = curr_word.find(';');
    if (index1 != string::npos)
    {
      if (index2 != string::npos)
      {
        if (index1 < index2)
        {
          words.push_back(curr_word.substr(0, index1 + 1));
          curr_word = curr_word.substr(index1 + 1);
        }
      }
      else if (curr_word[curr_word.size() - 1] != ':')
      {
        words.push_back(curr_word.substr(0, index1 + 1));
        curr_word = curr_word.substr(index1 + 1);
      }
    }
    if (curr_word.empty())
      continue;
    if (index2 != string::npos)
    {
      words.push_back(curr_word.substr(0, index2));
      break;
    }
    words.push_back(curr_word);
  }
  string comment;
  size_t pos = curr_line.find(';');
  if (pos != std::string::npos)
  {
    pos++;
    while (pos < curr_line.size() && curr_line[pos] == ' ')
    {
      pos++;
    }
    comment = curr_line.substr(pos);
  }
  if (!comment.empty())
    comments[line_number] = comment;
  return words;
}

void initialise_instruct()
{
  // operand type are of 3 types:
  // 0 operand type means instruction takes no operand
  // 1 operand type means instruction takes 1 operand and is value
  // -1 operand type means instruction takes 1 operand and is offset
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

void first_pass(const vector<string> &all_lines)
{
  
  int program_counter = 0;
  fr(i, 0, all_lines.size())
  {
    string curr_line = all_lines[i];
    auto curr = splitIntoWords(curr_line, i + 1);
    if (!curr.empty())
    {
      string label = "", instruction_name = "", operand = "",op_copy=operand;
      int position = 0;
      auto it = curr.begin();
      if (it != curr.end() && it->back() == ':')
      {
        label = it->substr(0, it->size() - 1);
        it++;
        position++;
      }
      if (it != curr.end())
      {
        instruction_name = *it;
        it++;
        position++;
      }
      if (it != curr.end())
      {
        operand = *it;
        position++;
      }
      labelcheck(label, i + 1, program_counter);
      bool incrementflag = false;
      // string original_op = operand;
      bool flag=false;
      if(curr.size()-position>0)
      flag=true;
      validateMnemonic(instruction_name, i + 1, program_counter, operand, incrementflag, flag);
      op_copy=operand;
      line_info.push_back(line{program_counter, label, instruction_name,operand,op_copy});
      if (incrementflag && instruction_name == "SET")
      {
        if (!label.empty())
        {
          var_set[label] = operand;
        }
        else
          errors_insert(i + 1, "Label name is missing.");
      }
      if (incrementflag)
      program_counter++;
    }
  }
  for (const auto &symbol : label_table)
  {
    const string &symName = symbol.first;
    const auto &symDetails = symbol.second;

    if (symDetails.first == -1)
    {
      // If the symbol was used but not declared, push an error for each line it's referenced
      for (const auto &refLine : label_location[symName])
      {
        errors_insert(refLine, "Undefined label");
      }
    }
    else if (label_location.find(symName) == label_location.end())
    {
      // If the symbol was declared but never used, push a warning at the declaration location
      warnings_insert(symDetails.second, "Label declared but unused");
    }
  }
  return ;
}

void second_pass()
{
  fr(i, 0, line_info.size())
  {
    auto curr_line = line_info[i];
    string label = curr_line.label, instruction = curr_line.instruct, operand = curr_line.operand, original_op = curr_line.original_op, mac_code = "";
    int program_counter = curr_line.pctr;
    int op_type = 2;
    if (!instruction.empty())
    {
      op_type = instructions[instruction].second;
    }
    if (op_type == 0)
    {
      macfornooperand(instruction, mac_code);
    }
    else if (op_type == -1)
    {
      macforoffset(instruction, operand, program_counter, mac_code);
    }
    else if (op_type == 1)
    {
      if (instruction == "data" || instruction == "SET")
      {
        macfordataorset(operand, mac_code);
      }
      else
        macforvaluecode(instruction, operand, program_counter, mac_code);
    }
    maccode.push_back(mac_code);
    string instruction_c = instruction, label_c = label;
    if (!instruction.empty())
      instruction_c += " ";
    if (!label.empty())
      label_c += ": ";
    string statement ="";
    statement+= label_c;
    statement+= instruction_c;
    statement+=original_op;
    list_info.push_back(List{dectohex(program_counter), statement, mac_code});
  }
}

void macfornooperand(string mnemonic, string &maccode)
{
  maccode = "000000" + instructions[mnemonic].first;
}

void macfordataorset(string operand, string &maccode)
{
  maccode = dectohex(stoi(operand));
}

void macforoffset(string mnemonic, string operand, int program_counter, string &maccode)
{
  int offset;
  if (label_table.find(operand) != label_table.end())
  {
    offset = label_table[operand].first - (program_counter );
  }
  else
    offset = (int)stoi(operand);
  maccode = dectohex(offset).substr(2) + instructions[mnemonic].first;
}

void macforvaluecode(string mnemonic, string operand, int program_counter, string &maccode)
{
  int value;
  if (label_table.find(operand) != label_table.end())
  {
    value = label_table[operand].first;
  }
  else
    value = (int)stoi(operand);
  if (var_set.find(operand) != var_set.end())
    value = stoi(var_set[operand]);
  maccode = dectohex(value).substr(2) + instructions[mnemonic].first;
}

void show_err_warn(bool &flag)
{
  ofstream errcout("logfile.log");
  cout << "File for errors created." << endl;
  if (error_table.empty())
  {
    cout << "The code does not contain any error!" << endl;
    fr(i, 0, warning_table.size())
    {
      errcout << "Warning on Line: " << warning_table[i].first << endl
              << warning_table[i].second << endl;
    }
    flag = true;
  }
  else
  {
    fr(i, 0, error_table.size())
    {
      errcout << "Error on Line: " << error_table[i].first << endl
              << error_table[i].second << endl;
    }
  }
  errcout.close();
  return;
}

void labelcheck(string label, int line_number, int program_counter)
{
  if (!label.empty())
  {
    if (labelvalidity(label))
    {
      if (label_table.find(label) != label_table.end() && label_table[label].first != -1)
      {
        errors_insert(line_number, "Duplicate label defined");
      }
      else
        label_table[label] = {program_counter, line_number};
    }
    else
    {
      errors_insert(line_number, "Label name is invalid. Follow the rules of naming of labels.");
    }
  }
  else
    return;
}

bool labelvalidity(string label)
{
  bool checker = true;
  checker &= alphabetchecker(label[0]);
  for (int i = 1; i < label.size(); i++)
  {
    checker &= (alphabetchecker(label[i]) || label[i] == '_' || digitchecker(label[i]));
  }
  return checker;
}

bool alphabetchecker(char c)
{
  if ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'))
  {
    return true;
  }
  return false;
}

bool digitchecker(char c)
{
  if (c >= '0' && c <= '9')
    return true;
  return false;
}

void warnings_insert(int line_number, string warning)
{
  warning_table.push_back({line_number, warning});
}

void errors_insert(int line_number, string error)
{
  error_table.push_back({line_number, error});
}

void validateMnemonic(string instname, int line_number, int program_counter, string &operand, bool &checker, bool flag)
{
  if (instname.empty())
    return;
  if (instructions.find(instname) != instructions.end())
  {
    int operandtype = instructions[instname].second;
    if (operandtype == 0)
    {
      if (operand.empty())
        checker = true;
      else
        errors_insert(line_number, "Unexpected operand detected.");
    }
    else
    {
      if (flag)
      {
        errors_insert(line_number, "Invalid extra information in the line.");
      }
      else if (operand.empty())
      {
        errors_insert(line_number, "Operand not specified.");
      }
      else
      {
        string new_operand = validateOperand(operand, line_number);
        if (!new_operand.empty())
        {
          operand = new_operand;
          checker = true;
        }
        else
          errors_insert(line_number, "Not a valid number/operand.");
      }
    }
  }
  else
    errors_insert(line_number, "Invalid Mnemonic.");
}

string validateOperand(string operand, int line_number)
{
  // if operand is label
  if (labelvalidity(operand))
  {
    if (label_table.find(operand) == label_table.end())
    {
      label_table[operand] = {-1, line_number};
    }
    label_location[operand].push_back(line_number);
    return operand;
  }
  // if operand is numeric value
  string op_copy = operand;
  string sign="";
  if (operand[0] == '-')
  {
    sign = operand[0];
    op_copy = op_copy.substr(1);
  }
  else if(operand[0]=='+')
  {
    sign = '+';
    if (operand[0] == '+')
    {
      op_copy = op_copy.substr(1);
    }
  }
  string result;
  result += sign;
  if (hexcheck(op_copy))
  {
    result += hextodec(op_copy);
  }
  else if (octalcheck(op_copy))
  {
    result += octaltodec(op_copy);
  }
  else if (deccheck(op_copy))
  {
    result += op_copy;
  }
  else
  {
    result = "";
  }
  return result;
}

bool octalcheck(string number)
{
  if (number.size() <= 2)
    return false;
  if (number[0] != '0' || (number[1] != 'o' && CaptoSm(number[1]) != 'o'))
    return false;
  fr(i, 2, number.size())
  {
    if (number[i] < '0' || number[i] > '7')
      return false;
  }
  return true;
}

bool hexcheck(string number)
{
  if (number.size() <= 2)
    return false;
  if (number[0] != '0' || (number[1] != 'x' && CaptoSm(number[1]) != 'x'))
    return false;
  fr(i, 2, number.size())
  {
    if (!digitchecker(number[i]))
    {
      if (!((number[i] >= 'a' && number[i] <= 'f') || (CaptoSm(number[i]) >= 'a' && CaptoSm(number[i]) <= 'f')))
        return false;
    }
  }
  return true;
}

bool deccheck(string number)
{
  if (number.empty())
    return false;
  fr(i, 0, number.size())
  {
    if (!digitchecker(number[i]))
      return false;
  }
  return true;
}

string octaltodec(string number)
{
  int value = 0;
  int pow = 0;
  for (int i = number.size() - 1; i >= 2; i--)
  {
    value += (number[i] - '0') * (power(8, pow));
    pow++;
  }
  return to_string(value);
}

string hextodec(string number)
{
  int value = 0;
  int pow = 0;
  for (int i = number.size() - 1; i >= 2; i--)
  {
    if (digitchecker(number[i]))
    {
      value += (number[i] - '0') * (power(16, pow));
    }
    else
    {
      char c = number[i];
      if (c >= 'a' && c <= 'f')
      {
        value += (c - 'a' + 10) * (power(16, pow));
      }
      else
      {
        c = CaptoSm(c);
        value += (c - 'a' + 10) * (power(16, pow));
      }
    }
    pow++;
  }
  return to_string(value);
}

string dectohex(int number)
{
  stringstream ss;
  ss << setw(8) << setfill('0') << hex << number;
  return ss.str();
}

long long power(int a, int b)
{
  long long result = 1;
  while (b)
  {
    if (b & 1)
      result = result * a;
    a = a * a;
    b >>= 1;
  }
  return result;
}

int main()
{
  // Reading from the given input file
  string current_line;
  ifstream input_file;
  input_file.open("input.txt");
  if (input_file.fail())
  {
    cout << "Input file doesn't exist in the same directory. Check the file location." << endl;
    exit(0);
  }
  while (getline(input_file, current_line))
  {
    all_lines.push_back(current_line);
  }
  input_file.close();
  // Now initialise the OP table
  initialise_instruct();
  // After initialising the OP table, run the first pass on the input code;
  first_pass(all_lines);
  sort(error_table.begin(), error_table.end());
  sort(warning_table.begin(), warning_table.end());
  bool flag = false;
  show_err_warn(flag);
  if (flag)
  {
    second_pass();
    ofstream listcout("listfile.lst");
    for (const auto &entry : list_info)
    {
      listcout << entry.add << " " << entry.machine << " " << entry.statement << endl;
    }
    listcout.close();
    cout << "Listing file is ready." << endl;
    ofstream maccodefile("machineCode.o", ios::binary);
    for (const auto &curr_code : maccode)
    {
      if (curr_code.empty() || curr_code.find_first_not_of(' ') == string::npos)
        continue;
      try
      {
        unsigned int machinevalue = static_cast<unsigned int>(stoi(hextodec(curr_code))); // casting to unsigned int in a safe way
        maccodefile.write(reinterpret_cast<const char *>(&machinevalue), sizeof(machinevalue));
      }
      catch (const std::invalid_argument &e)
      {
        continue;
      }
    }
    maccodefile.close();
    cout << "Machine code object file is ready." << endl;
    // for(auto x:line_info){
    //   cout<<x.original_op<<endl;
    // }
  }
  return 0;
}