#include<bits/stdc++.h>
using namespace std;

map<string,string> optab;
map<string,string> symtab;

string start_address;
string loc_ctr;
string prog_length;
void init_opcode()
{
    vector<string>mne = {"LDA", "LDX", "LDL" ,
                        "STA", "STX", "STL",
                        "LDCH", "STCH", "ADD",
                        "SUB", "MUL", "DIV",
                         "COMP", "J", "JLT",
                         "JEQ", "JGT", "JSUB",
                         "RSUB", "TIX", "TD",
                          "RD", "WD"};
    vector<string>code = {"00", "04", "08",
                          "0C", "10", "14",
                          "50", "54", "18",
                          "1C", "20", "24",
                          "28", "3C", "38",
                          "30", "34", "48",
                          "4C", "2C", "E0",
                          "D8", "DC" };

    for(int i=0; i < mne.size(); i++)
    {
        optab.insert({mne[i],code[i]});
    }

    return;

}


void parse_line(string line, vector <string> &parsed)
{
    stringstream ss(line);
    string word;
    int i = 1;
    while (ss>>word)
    {
        parsed[i] = word;
        i++;
    }
    if(parsed[1][0] == '.')
    {
        parsed[0] = ".";
    }
    if(i == 3)
    {
        parsed[3] = parsed[2];
        parsed[2] = parsed[1];
        parsed[1] = "";
    }
    else if(i == 2)
    {
        parsed[2] = parsed[1];
        parsed[3] = parsed[1] = "";
    }
    return;
}

int read_line(ifstream &fin, vector<string>&parsed,string &code_line)
{
    string line;
    if(!getline(fin,line))
    {
       return 0;
    }
    code_line = line;
    for(int i=0;i<4;i++)
    {
        parsed[i] = "";
    }
    parse_line(line,parsed);
    return 1;
}
int main()
{
    ifstream fin;
    fin.open("code.in");


    init_opcode();

    //Pass 1
    vector <string> parsed(4);

    //read first line
    string code_line;
    read_line(fin,parsed,code_line);
    if(parsed[2] == "START")
    {
            start_address = parsed[3];
            loc_ctr = start_address;
            //Write line
            read_line(fin,parsed,code_line);
    }
    else
    {
        loc_ctr = "0000";
    }
    //While opcode ! = END do
    while(parsed[2] != "END")
    {
        if(parsed[0] != ".")
        {
            if(parsed[1] != "")
            {
                bool found_sym;
                //Search for symbol
                if(symtab.find(parsed[1]) != symtab.end())
                {
                    found_sym = true;
                }
                else
                {
                    found_sym = false;
                }

                if(found_sym)
                {
                    //Set error flag
                }
                else
                {
                    symtab.insert({parsed[1],loc_ctr});
                }

                //Search optab for opcode

                bool found_opcode;
                string opcode;
                if(optab.find(parsed[2]) != optab.end())
                {
                    found_opcode = true;
                    opcode = optab[parsed[2]];
                }
                else
                {
                    found_opcode = false;
                }

                if(found_opcode == true)
                {
                    // Add 3* instruction length to opcode
                }
                else if(parsed[2] == "WORD")
                {
                    // Add 3 to loc_ctr
                }
                else if(parsed[2] == "RESW")
                {
                    //Add 3*#[Operand] to loc_ctr
                }
                else if(parsed[2] == "BYTE")
                {
                    //find length of constant in bytes
                    //add length to loc_ctr
                }
                else
                {
                    //set error flag (invalid opcode)
                }
            }
        }
        //write line to intermediate file
        read_line(fin,parsed,code_line);
    }
    //write last line
    //update prog_length


    //End of pass 1



    //Pass 2



return 0;
}
