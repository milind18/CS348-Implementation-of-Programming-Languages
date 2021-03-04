

/*
Name: Milind B Prabhu
Roll: 180101091



Compiling and executing assembler.cpp
1. The 180101091_Assign01.cpp program implements the SIC assembler. To compile the program on Ubuntu run the following command.

$ g++ 180101091_Assign01.cpp

2. To assemble a file named code.asm, copy the file to the directory containing the 180101091_Assign01.cpp executable. Then run the following command in the terminal.

$ ./a.out code.asm

This will generate the intermediate file, the listing file and the object code.


Input file format
1. For all lines which are not comments, the first 10 columns are for the label, the next 10 are for the opcode and the last 30 are for the operand.
2. If a label, operand is not required fill those columns with spaces.
3. An example input file and the output generated are given in the Example folder.




*/
#include <bits/stdc++.h>
using namespace std;

//opcode table
map<string, string> optab;
//symbol table
map<string, string> symtab;
vector<int> pc_value;
string start_address = "0000";
string prog_name = "";
string loc_ctr;
string prog_length;
string curr_ctr;

bool iscomment;
string addr;
string label;
string operation;
string operand;
string operand1, operand2;
string operand_address;
string operand_address1, operand_address2;
string base_address;
string real_operand;
//SIC/XE
// n, i, x, b, p, e
int indirect = 0, immediate = 0, indexed = 0,  base_relative = 0, pc_relative = 0, extended = 0;
int instruction_length = 0;
bool wasReserve = false;

void init_opcode()
{
    vector<string> mne = {"LDA", "LDX", "LDL",
                          "STA", "STX", "STL",
                          "LDCH", "STCH", "ADD",
                          "SUB", "MUL", "DIV",
                          "COMP", "J", "JLT",
                          "JEQ", "JGT", "JSUB",
                          "RSUB", "TIX", "TD",
                          "RD", "WD",
                          "LDB", "LDT", "COMPR",//SIC/XE
                          "CLEAR","TIXR"};

    vector<string> code = {"00", "04", "08",
                           "0C", "10", "14",
                           "50", "54", "18",
                           "1C", "20", "24",
                           "28", "3C", "38",
                           "30", "34", "48",
                           "4C", "2C", "E0",
                           "D8", "DC",
                           "68", "74", "A0",//SIC/XE
                           "B4","B8"
                           };

    for (int i = 0; i < mne.size(); i++)
    {
        optab.insert({mne[i], code[i]});
    }

    return;
}
void init_symtab()
{
    vector <string> register_name = {"A","X","L",
                                     "B","S","T",
                                     "F"};
    vector <string> register_number = { "0", "1", "2",
                                        "3", "4", "5",
                                        "6"};
    for(int i = 0; i < register_name.size(); i++)
    {
        symtab.insert({register_name[i],register_number[i]});
    }
}

//Function to strip spaces
string remove_spaces(string a)
{
    string ans;
    int start = 0;
    for (int i = 0; i < a.size(); i++)
    {
        if (a[i] == ' ')
        {
            start++;
        }
        else
        {
            break;
        }
    }

    int end = a.size() - 1;

    for (int i = a.size() - 1; i >= 0; i--)
    {
        if (a[i] == ' ')
        {
            end--;
        }
        else
        {
            break;
        }
    }

    return a.substr(start, end - start + 1);
}

// function to read lines from the .asm file
int read_line(ifstream &fin, string &code_line)
{
    string line;
    if (!getline(fin, line))
    {
        return -1;
    }
    code_line = line;

    label = "";
    operand = "";
    operation = "";
    iscomment = false;

    stringstream checkcomment(line);
    string first_word;
    checkcomment >> first_word;
    if (first_word[0] == '.')
    {
        iscomment = true;
        return 1;
    }
    label = remove_spaces(line.substr(0, 10));
    operation = remove_spaces(line.substr(10, 10));
    operand = remove_spaces(line.substr(20));
    
    //cout << endl << label << " " << operation << " " << operand << endl;
    return 0;
}

// function to read lines from intermediate file
int read_line2(ifstream &fin, string &code_line)
{
    string line;
    if (!getline(fin, line))
    {
        return -1;
    }
    code_line = line;

    addr = "";
    label = "";
    operand = "";
    operation = "";
    iscomment = false;

    stringstream checkcomment(line);
    string first_word;
    checkcomment >> first_word;
    if (first_word[0] == '.')
    {
        iscomment = true;
        return 1;
    }
    addr = remove_spaces(line.substr(0, 10));
    label = remove_spaces(line.substr(10, 10));
    operation = remove_spaces(line.substr(20, 10));
    operand = remove_spaces(line.substr(30));

    return 0;
}

//converting  a hexadecimal string to a long long int
long long from_hex(string a)
{

    int n = a.size();
    long long ans = 0;
    for (int i = 0; i < n; i++)
    {
        ans *= 16;
        if (a[i] >= '0' && a[i] <= '9')
        {
            ans += (a[i] - '0');
        }
        else if (a[i] >= 'a' && a[i] <= 'f')
        {
            ans += (a[i] - 'a' + 10);
        }
        else if (a[i] >= 'A' && a[i] <= 'F')
        {
            ans += (a[i] - 'A' + 10);
        }
        else
        {
            cout << a  << endl;
            assert(0);
        }
    }
    return ans;
}

//converting a long long int to a hexadecimal string
string to_hex(long long a)
{
    //12 bit 2's complement formm
    if(a < 0)
    {
        return to_hex(4096 + a).substr(1,3);
    }
    if (a == 0)
    {
        return "0000";
    }
    string ans;
    while (a > 0)
    {
        int x = a % 16;
        if (x >= 0 && x <= 9)
        {
            ans.push_back('0' + x);
        }
        else
        {
            ans.push_back('A' + x - 10);
        }
        a /= 16;
    }
    int k = ans.size();
    for (int j = 0; j < 4 - k; j++)
    {
        ans.push_back('0');
    }
    reverse(ans.begin(), ans.end());
    return ans;
}

//converting a hexadecimal string to a decimal number
long long num(string a)
{
    long long ans = 0;
    for (int i = 0; i < a.size(); i++)
    {
        ans *= 10;
        ans += (a[i] - '0');
    }
    return ans;
}

// function to add a prefix of zeroes to make the total length of string 'a' =  'length'
string pad_zero(string a, int length = 6)
{
    int k = a.size();
    if (k > length)
    {
        cout << "expected length smaller than real length";
        return a;
    }

    string b;
    for (int i = 0; i < length - k; i++)
    {
        b.push_back('0');
    }
    for (int i = 0; i < a.size(); i++)
    {
        b.push_back(a[i]);
    }
    return b;
}

// Function to process operand and check if it contains a symbol
string isSymbol(string operand)
{
    if (operand == "")
        return "";
    int n = operand.size();
    if (n - 2 >= 0 && operand[n - 2] == ',' && operand[n - 1] == 'X')
    {
        return operand.substr(0, n - 2);
    }
    bool isImmediate = true;
    for (int i = 0; i < operand.size(); i++)
    {
        if (!(operand[i] >= '0' && operand[i] <= '9'))
        {
            isImmediate = false;
        }
    }

    if (isImmediate)
    {
        return "";
    }
    else
    {
        return operand;
    }
}

// Converting immediate operands to object code
string convert_to_objectcode(string operand)
{
    // X' C'
    if (operand.substr(0, 2) == "X'")
    {
        return operand.substr(2, operand.size() - 3);
    }
    if (operand.substr(0, 2) == "C'")
    {
        string ans;

        for (int i = 2; i < operand.size() - 1; i++)
        {
            ans = ans + to_hex((int)(operand[i])).substr(2, 2);
        }
        return ans;
    }

    cout << "Unrecognized constant";
    return "";
}

bool isDecimal(string a)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(a[i] >= '0' && a[i] <= '9')
        {
            continue;
        }
        else
            return false;
    }
    return true;
}

string trim(string a)
{
    for(int i = 0; i < a.size(); i++)
    {
        if(a[i] != '0')
        {
            return a.substr(i);
        }
    }
    return "0";
}

int main(int argc, char *argv[])
{
    //Open the file containing the code
    if (argc <= 1)
    {
        cout << "Provide assembly file name as command line argument\n";
        exit(0);
    }

    ifstream fin;
    ofstream fout;
    ofstream listing;
    fin.open(argv[1]);
    if (fin.is_open() == false)
    {
        cout << "File not found in directory\n";
        exit(0);
    }
    fout.open("intermediate.txt");
    listing.open("listing.txt");
    init_opcode();
    init_symtab();

    //Pass 1

    //read first line
    string code_line;
    read_line(fin, code_line);
    if (operation == "START")
    {
        prog_name = label;
        start_address = operand;
        loc_ctr = pad_zero(operand,4);
        //Write line
        fout << left << setw(10) << " "
             << setw(10) << label
             << setw(10) << operation 
             << setw(30) << operand 
             << endl;
        read_line(fin, code_line);
    }
    else
    {
        loc_ctr = "0000";
    }
    //While opcode ! = END do
    while (operation != "END")
    {
        //cout << operation << " " << loc_ctr << endl;
        if(operation == "BASE")
        {
            //pc_value.push_back(loc_ctr);
            fout<< left 
                << setw(10) << " "
                << setw(10) << " "
                << setw(10) << operation 
                << setw(30) << operand 
                << endl;
            read_line(fin,code_line);
            continue;
        }
        //If this is not a comment line
        if (!iscomment)
        {
            //If there is a symbol in the label field
            if (label != "")
            {
                bool found_sym;
                //Search for symbol
                if (symtab.find(label) != symtab.end())
                {
                    found_sym = true;
                    //cout << "Symbol found: " << label << endl;
                }
                else
                {
                    found_sym = false;
                    //cout << "Symbol not found: " << label << endl;
                }

                if (found_sym)
                {
                    //!Set error flag
                    cerr << "Error: Multiple definitions of symbol: " << label << endl;
                    //cout << code_line;
                }
                else
                {
                    symtab.insert({label, loc_ctr});
                }
            }
            //Search optab for opcode

            bool found_opcode;
            string opcode;
            instruction_length = 3;
            
            if(operation == "CLEAR" || operation == "TIXR" || operation == "COMPR")
            {
                instruction_length = 2;
            }
            if (optab.find(operation) != optab.end())
            {
                found_opcode = true;
                opcode = optab[operation];
                //cout << "Opcode found: " << operation << endl;
            }
            else
            {
                found_opcode = false;
                //cout << "Opcode not found: " << operation << endl;
            }

            if(operation[0] == '+')
            {   
                instruction_length = 4;
                if(optab.find(operation.substr(1)) != optab.end())
                {
                    found_opcode = true;
                    opcode = optab[operation.substr(1)];
                }
            }

            curr_ctr = loc_ctr;
            if (found_opcode == true)
            {
                // Add 3* instruction length to opcode
                loc_ctr = to_hex(from_hex(loc_ctr) + instruction_length);
            }
            else if (operation == "WORD")
            {
                // Add 3 to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + 3);
            }
            else if (operation == "RESW")
            {
                //Add 3*#[Operand] to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + 3 * num(operand));
            }
            else if (operation == "RESB")
            {
                loc_ctr = to_hex(from_hex(loc_ctr) + num(operand));
            }
            else if (operation == "BYTE")
            {
                //find length of constant in bytes
                //cout << operand.substr(0,4) << endl;
                //cout << operand.size() << endl;
                int constLength = operand.size() - 3;
                //cout << "const_length " << constLength << endl;
                if (operand[0] == 'X')
                {
                    constLength = (constLength + 1) / 2;
                }
                //add length to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + constLength);
                //cout << loc_ctr << endl;
            }
            else
            {
                //!set error flag (invalid opcode)
                cout << "Error: Operation not found: " << code_line << operation << endl;
                exit(0);
            }
        }
        //write line to intermediate file
        if (iscomment)
        {
            fout << code_line << endl;
        }
        else
        {
            fout << left 
            << setw(10) << curr_ctr
            << setw(10) << label
            << setw(10) << operation 
            << setw(30) << operand 
            << endl;
        }

        read_line(fin, code_line);
    }
    //write last line
    fout << left << setw(10) << " "
     << setw(10) << label 
     << setw(10) << operation 
     << setw(30) << operand 
     << endl;

    //update prog_length
    long long prog_length = from_hex(loc_ctr) - from_hex(start_address);
    //End of pass 1
    fout.close();
    fin.close();
    cout << "Intermediate file created...\n";
    /* ************************************************************


                                Pass 2


     **************************************************************/


    fin.open("intermediate.txt");
    fout.open("objectcode.txt");
    read_line2(fin, code_line);

    //Printing header
    fout << left << "H" << setw(6) << prog_name << pad_zero(start_address) << pad_zero(to_hex(prog_length)) << endl;
    if (operand == "START")
    {
        //Write listing line
        listing << code_line << endl;
        read_line2(fin, code_line);
    }

    stringstream textrecord;
    textrecord.str("");
    stringstream objcode;
    string begin_addr;

    while (operation != "END")
    {
        extended = base_relative = pc_relative = 0;
        indirect = 1;
        immediate = 1;
        //Check if operand is passed immediately or indirectly
        if (iscomment == false)
        {
           
            if(operation == "BASE")
            {
                if(symtab.find(operand) == symtab.end())
                {
                    cout << operand << " " ;
                    cout << "Base address not found. " << endl;
                    exit(0);
                }
                base_address = symtab[operand];
            }
        
            bool found_opcode;
            string opcode;
            if (optab.find(operation) != optab.end())
            {
                found_opcode = true;
                opcode = optab[operation];


                //Register, register operations
                if(operation == "TIXR" || operation == "CLEAR")
                {
                    operand_address1 = symtab[operand];
                    operand_address2 = "0";
                    wasReserve = false;
                    objcode << opcode << operand_address1 << operand_address2;
                    listing << code_line  << " " << objcode.str() << endl;
                    cout << objcode.str() << endl;
                    objcode.str("");
                    goto end_of_loop;
                }
                else  if(operation == "COMPR")
                {
                    wasReserve = false;
                    if(operand.size() != 3)
                        cout << "Error: Invalid operand " << operand << endl;
                    operand1 = operand.substr(0,1);
                    operand2 = operand.substr(2,1);
                    if(symtab.find(operand1) == symtab.end() || symtab.find(operand2) == symtab.end())
                        cout << "Error: Invalid operand " << operand << endl;
                    operand_address1 = symtab[operand1];
                    operand_address2 = symtab[operand2];

                    objcode << opcode << operand_address1 << operand_address2;
                    goto end_of_loop;
                }
                string final = operand;
                instruction_length = 3;
                cout << "check1" << endl;

                cout << operation << " " << operand << endl;
                if(operand.size() > 0 && operand.find(",X") < operand.size())
                {
                    indexed = 1;
                    final = operand.substr(0,operand.size()-2);
                    if(symtab.find(final) == symtab.end())
                    {
                        cout << "Error: Symbol not found" << endl;
                        exit(0);
                    }
                    final = symtab[final];
                }
                else if(operand.size() > 1 && operand[0] == '@')
                {
                    indirect = 1;
                    immediate = 0;
                    final = operand.substr(1,operand.size()-1);
                    if(symtab.find(final) == symtab.end())
                    {
                        cout << "Error: Symbol not found" << endl;
                        exit(0);
                    }
                    final = symtab[final];
                }
                else if(operand.size() > 1 && operand[0] == '#')
                {
                    cout << "imm" << endl;
                    immediate = 1;
                    indirect = 0;
                    //string real_operand = operand.substr(1,operand.size()-1);
                    real_operand = operand.substr(1);
                    if(isDecimal(real_operand))
                    {
                        cout <<" is decimal";
                        final = to_hex((long long)atoi(real_operand.c_str()));
                        goto end_of_cases;
                    }
                    else
                    {
                        cout << "not decimal" << endl;
                        if(symtab.find(real_operand) == symtab.end())
                        {
                            cout << "Error: Symbol not found " << real_operand << endl;
                            exit(0);
                        }
                        final = symtab[real_operand];
                    }
                }
                else
                {
                    if(symtab.find(final) == symtab.end())
                    {
                        cout << "Error: Symbol not found " << real_operand << endl;
                        exit(0);
                    }
                    final = symtab[final];
                }

                cout << "check2";

                if(operation[0] == '+')
                {
                    instruction_length = 4;
                    extended = 1;
                    
                }
                else
                {
                    long long diff = from_hex(final) - (from_hex(addr)+instruction_length);
                    long long diff2 = from_hex(final) - from_hex(base_address);
                    if(diff >= -2048 && diff <= 2047)
                    {
                        pc_relative = 1;
                        final = to_hex(diff);
                        if(final.size() == 4) final = final.substr(1);
                    }
                    else if(diff2 >= 0 && diff2 <= 4097)
                    {
                        base_relative = 1;
                        final = to_hex(diff2);
                    }
                    else
                    {
                        cout << "Error: Mistake in addressing format." << endl;
                        exit(0);
                    }
                }
                end_of_cases:
                cout << "check3" << endl;
                objcode << pad_zero(trim(to_hex(from_hex(opcode) + 2*indirect + 1*immediate)),2) <<   pad_zero(trim(to_hex( (long long)(indexed*8 + base_relative*4 + pc_relative*2 + extended))),1) <<  pad_zero(trim(final),3);
                cout << objcode.str() << endl;
                // string symbol = isSymbol(operand);

                // if (symbol != "")
                // {
                //     if (symtab.find(symbol) != symtab.end())
                //     {
                //         operand_address = symtab[symbol];
                //         if (operand.size() >= 2 && operand.substr(operand.size() - 2, 2) == ",X")
                //         {
                //             operand_address = to_hex(from_hex(operand_address) + 32768);
                //         }
                //         //cout << operand_address << endl;
                //     }
                //     else
                //     {
                //         operand_address = "0";
                //         cerr << "Error: Symbol " << symbol << " not found\n";
                //         exit(0);
                //     }
                //}
                // else
                // {
                //     operand_address = "0";
                // }
                //objcode << opcode << pad_zero(operand_address, 4);
                //cout << "Opcode found: " << opcode << endl;
            }
            else
            {
                found_opcode = false;
                //cout << "Opcode not found corresponding to: " << operation << endl;

                if (operation == "WORD")
                {
                    string newoperand = pad_zero(to_hex(stoi(operand)));
                    objcode << newoperand;
                }
                else if (operation == "BYTE")
                {
                    // string newoperand = to_hex(stoi(operand));
                    string newoperand = convert_to_objectcode(operand);
                    objcode << newoperand;
                }
            }

            if (operation == "RESW" || operation == "RESB")
            {
                listing << code_line << endl;
                if (wasReserve == false)
                {
                    wasReserve = true;
                    if (textrecord.str() == "")
                    {
                        begin_addr = "T" + pad_zero(addr);
                    }
                    fout << begin_addr << to_hex((textrecord.str().size()) / 2).substr(2, 2) << textrecord.str() << endl;
                    //cout<< to_hex(textrecord.str().size())<<" byte"<<endl;

                    textrecord.str("");
                }
            }
            else
            {
                wasReserve = false;
                if (textrecord.str().size() > 54)
                {
                    listing << code_line << " " << objcode.str() << endl;
                    fout << begin_addr << to_hex((textrecord.str().size()) / 2).substr(2, 2) << textrecord.str() << endl;
                    begin_addr = "T" + pad_zero(addr);
                    textrecord.str("");
                    textrecord << objcode.str();
                    objcode.str("");
                }
                else if (textrecord.str() == "")
                {
                    listing << code_line << " " << objcode.str() << endl;
                    begin_addr = "T" + pad_zero(addr);
                    textrecord << objcode.str();
                    objcode.str("");
                }
                else
                {
                    listing << code_line << " " << objcode.str() << endl;
                    textrecord << objcode.str();
                    objcode.str("");
                }
            }
        }
        else
        {
            listing << code_line << endl;
        }
        end_of_loop:
        read_line2(fin, code_line);
    }
    if (textrecord.str() != "")
    {
        fout << begin_addr << to_hex((textrecord.str().size()) / 2).substr(2, 2) << textrecord.str() << endl;
    }
    listing << code_line << endl;
    fout << "E" << pad_zero(start_address);

    listing.close();
    cout << "Listing file generated...\n";
    fout.close();
    cout << "Object file generated ...\n";
    cout << "Assembly successful\n";
    return 0;
}
