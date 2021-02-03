#include <bits/stdc++.h>
using namespace std;

//opcode table
map<string, string> optab;
//symbol table
map<string, string> symtab;

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
                          "RD", "WD"};
    vector<string> code = {"00", "04", "08",
                           "0C", "10", "14",
                           "50", "54", "18",
                           "1C", "20", "24",
                           "28", "3C", "38",
                           "30", "34", "48",
                           "4C", "2C", "E0",
                           "D8", "DC"};

    for (int i = 0; i < mne.size(); i++)
    {
        optab.insert({mne[i], code[i]});
    }

    return;
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
            assert(0);
        }
    }
    return ans;
}

//converting a long long int to a hexadecimal string
string to_hex(long long a)
{
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

    //Pass 1

    //read first line
    string code_line;
    read_line(fin, code_line);
    if (operation == "START")
    {
        prog_name = label;
        start_address = operand;
        loc_ctr = start_address;
        //Write line
        fout << left << setw(10) << loc_ctr << setw(10) << label << setw(10) << operation << setw(30) << operand << endl;
        read_line(fin, code_line);
    }
    else
    {
        loc_ctr = "0000";
    }
    //While opcode ! = END do
    while (operation != "END")
    {
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
            curr_ctr = loc_ctr;
            if (found_opcode == true)
            {
                // Add 3* instruction length to opcode
                loc_ctr = to_hex(from_hex(loc_ctr) + 3);
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
                int constLength = operand.size() - 3;
                if (operand[0] == 'X')
                {
                    constLength = (constLength + 1) / 2;
                }
                //add length to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + constLength);
            }
            else
            {
                //!set error flag (invalid opcode)
                cout << "Error: Operation not found" << code_line << operation;
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
            fout << left << setw(10) << curr_ctr << setw(10) << label << setw(10) << operation << setw(30) << operand << endl;
        }

        read_line(fin, code_line);
    }
    //write last line
    fout << left << setw(10) << loc_ctr << setw(10) << label << setw(10) << operation << setw(30) << operand << endl;

    //update prog_length
    long long prog_length = from_hex(loc_ctr) - from_hex(start_address);
    //End of pass 1
    fout.close();
    fin.close();
    cout << "Intermediate file created...\n";

    //Pass 2
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
        if (iscomment == false)
        {
            bool found_opcode;
            string opcode;
            if (optab.find(operation) != optab.end())
            {
                found_opcode = true;
                opcode = optab[operation];
                string symbol = isSymbol(operand);

                string operand_address;
                if (symbol != "")
                {
                    if (symtab.find(symbol) != symtab.end())
                    {
                        operand_address = symtab[symbol];
                        if (operand.size() >= 2 && operand.substr(operand.size() - 2, 2) == ",X")
                        {
                            operand_address = to_hex(from_hex(operand_address) + 32768);
                        }
                        //cout << operand_address << endl;
                    }
                    else
                    {
                        operand_address = "0";
                        cerr << "Error: Symbol (" << symbol << ") not found\n";
                        exit(0);
                    }
                }
                else
                {
                    operand_address = "0";
                }
                objcode << opcode << pad_zero(operand_address, 4);
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

        read_line2(fin, code_line);
    }
    if (textrecord.str() != "")
    {
        fout << begin_addr << to_hex((textrecord.str().size()) / 2).substr(2, 2) << textrecord.str() << endl;
    }
    listing << code_line << endl;
    fout << "E" << pad_zero(start_address);

    cout << "Listing file generated...\n";
    cout << "Object file generated ...\n";
    cout << "Assembly successful\n";
    return 0;
}
