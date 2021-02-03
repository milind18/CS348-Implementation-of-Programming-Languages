#include <bits/stdc++.h>
using namespace std;

map<string, string> optab;
map<string, string> symtab;

string start_address;
string loc_ctr;
string prog_length;
string curr_ctr;

bool iscomment;
string addr;
string label;
string operation;
string operand;

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

void parse_line(string line, vector<string> &parsed)
{
    stringstream checkcomment(line);
    string first_word;
    checkcomment >> first_word;
    if (first_word[0] == '.')
    {
        parsed[0] = ".";
        return;
    }
    stringstream ss(line);
    string word;
    int i = 1;

    while (ss >> word)
    {
        parsed[i] = word;
        i++;
    }
    if (i == 3)
    {
        parsed[3] = parsed[2];
        parsed[2] = parsed[1];
        parsed[1] = "-";
    }
    else if (i == 2)
    {
        parsed[2] = parsed[1];
        parsed[3] = parsed[1] = "-";
    }
    return;
}

void parse_line2(string line, vector<string> &parsed)
{
    stringstream checkcomment(line);
    string first_word;
    checkcomment >> first_word;
    if (first_word[0] == '.')
    {
        parsed[0] = ".";
        return;
    }
    stringstream ss(line);
    string word;
    int i = 1;

    while (ss >> word)
    {
        parsed[i] = word;
        i++;
    }
    if (parsed[1][0] == '.')
    {
        parsed[0] = ".";
    }

    return;
}

int read_line(ifstream &fin, vector<string> &parsed, string &code_line)
{
    string line;
    if (!getline(fin, line))
    {
        return 0;
    }
    code_line = line;
    for (int i = 0; i < 4; i++)
    {
        parsed[i] = "";
    }
    parse_line(line, parsed);
    return 1;
}

int read_line2(ifstream &fin, vector<string> &parsed, string &code_line)
{
    string line;
    if (!getline(fin, line))
    {
        return 0;
    }
    code_line = line;
    for (int i = 0; i < 5; i++)
    {
        parsed[i] = "";
    }
    parse_line2(line, parsed);
    iscomment = (parsed[0] == ".");
    addr = parsed[1];
    label = parsed[2];
    operation = parsed[3];
    operand = parsed[4];
    return 1;
}
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

string to_hex(long long a)
{
    if (a == 0)
    {
        return "0";
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

string isSymbol(string operand)
{
    if (operand == "-")
        return "";
    int n = operand.size();
    if (n - 2 >= 0 && operand[n - 2] == ',')
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

string convert_to_objectcode(string operand)
{
    // X' C'
    if (operand.substr(0, 2) == "X'")
    {
        return operand.substr(2, operand.size() - 2);
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
int main()
{
    //Open the file containing the code
    ifstream fin;
    ofstream fout;
    fin.open("code.in");
    fout.open("inter.out");
    init_opcode();

    //Pass 1
    vector<string> parsed(4);

    //read first line
    string code_line;
    read_line(fin, parsed, code_line);
    if (parsed[2] == "START")
    {
        start_address = parsed[3];
        loc_ctr = start_address;
        //Write line
        fout << left << setw(8) << loc_ctr << setw(8) << parsed[1] << setw(8) << parsed[2] << setw(8) << parsed[3] << endl;
        read_line(fin, parsed, code_line);
    }
    else
    {
        loc_ctr = "0000";
    }
    //While opcode ! = END do
    while (parsed[2] != "END")
    {
        //If this is not a comment line
        if (parsed[0] != ".")
        {
            //If there is a symbol in the label field
            if (parsed[1] != "")
            {
                bool found_sym;
                //Search for symbol
                if (symtab.find(parsed[1]) != symtab.end())
                {
                    found_sym = true;
                    //cout << "Symbol found: " << parsed[1] << endl;
                }
                else
                {
                    found_sym = false;
                    //cout << "Symbol not found: " << parsed[1] << endl;
                }

                if (found_sym)
                {
                    //!Set error flag
                }
                else
                {
                    symtab.insert({parsed[1], loc_ctr});
                }
            }
            //Search optab for opcode

            bool found_opcode;
            string opcode;
            if (optab.find(parsed[2]) != optab.end())
            {
                found_opcode = true;
                opcode = optab[parsed[2]];
                //cout << "Opcode found: " << parsed[2] << endl;
            }
            else
            {
                found_opcode = false;
                //cout << "Opcode not found: " << parsed[2] << endl;
            }
            curr_ctr = loc_ctr;
            if (found_opcode == true)
            {
                // Add 3* instruction length to opcode
                loc_ctr = to_hex(from_hex(loc_ctr) + 3);
            }
            else if (parsed[2] == "WORD")
            {
                // Add 3 to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + 3);
            }
            else if (parsed[2] == "RESW")
            {
                //Add 3*#[Operand] to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + num(parsed[3]));
            }
            else if (parsed[2] == "BYTE")
            {
                //find length of constant in bytes
                int constLength = parsed[3].size();
                //add length to loc_ctr
                loc_ctr = to_hex(from_hex(loc_ctr) + constLength);
            }
            else
            {
                //!set error flag (invalid opcode)
            }
        }
        //write line to intermediate file
        if (parsed[0] == ".")
        {
            fout << code_line << endl;
        }
        else
        {
            fout << left << setw(8) << curr_ctr << setw(8) << parsed[1] << setw(8) << parsed[2] << setw(8) << parsed[3] << endl;
        }

        read_line(fin, parsed, code_line);
    }
    //write last line
    fout << left << setw(8) << loc_ctr << setw(8) << parsed[1] << setw(8) << parsed[2] << setw(8) << parsed[3] << endl;

    //update prog_length
    long long prog_length = from_hex(loc_ctr) - from_hex(start_address);
    //End of pass 1
    fout.close();
    fin.close();

    //Pass 2
    fin.open("inter.out");
    fout.open("final.out");
    parsed.resize(5);
    read_line2(fin, parsed, code_line);

    if (parsed[3] == "START")
    {
        //Write listing line
    }
    //Need to fix this if START does not exist
    fout << left << "H" << setw(6) << parsed[2] << pad_zero(parsed[4]) << pad_zero(to_hex(prog_length));

    read_line2(fin, parsed, code_line);

    stringstream textrecord;
    textrecord.str("");
    stringstream objcode;
    string begin_addr;
    exit(0);
    while (operation != "END")
    {
        //cout << iscomment;
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
                        //cout << operand_address << endl;
                    }
                    else
                    {
                        operand_address = "0";
                        cout << "symbol not found: " << symbol;
                        //set error flag
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
                    if (newoperand.size() > 6)
                    {
                        cout << "Size of integer constant too large to fit in a word\n";
                        //error flag
                    }
                    objcode << newoperand;
                }
                else if (operation == "BYTE")
                {
                    // string newoperand = to_hex(stoi(operand));
                    string newoperand = convert_to_objectcode(operand);
                    objcode << newoperand;
                }
            }
            if (operation == "RESW" || "RESB")
            {
                if(textrecord.str() == "")
                {
                    begin_addr = "T" + addr;
                }
                fin >> begin_addr >> to_hex(text_record.str().size()).substr(2,2) >> textrecord.str();
                textrecord.str("");
            }
            else
            {
                if (textrecord.str().size() > 63)
                {
                    fin >> begin_addr >> to_hex(text_record.str().size()).substr(2,2) >> textrecord.str();
                    begin_addr = "T" + addr;
                    textrecord.str("");
                    textrecord >> objcode.str();
                }
                else if (textrecord.str() == "")
                {
                    begin_addr = "T" + addr;
                    textrecord >> objcode.str();
                }
                else
                {
                    textrecord >> objcode.str();
                }
                
            }
        }

        read_line2(fin, parsed, code_line);
    }
    return 0;
}
