#define NSLOTS 10

struct table_entry
{
    char *a;
    table_entry(char *input)
    {
        this-> a = input;
    }
};

int compute_hash(char *a)
{
    int val = 0;
    for(int i = 0; i < strlen(a); i++)
    {
        val += (a[i] - 'A');
        val = (val%NSLOTS + NSLOTS)%NSLOTS;
    }
    return val;
}
