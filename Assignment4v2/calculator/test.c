#include<stdio.h>

union test_union
{
    struct
    {
        int x;
        char * y;
    }element;
    int z;
};

int main()
{
    union test_union instance;
    instance.element.x = 100;
    instance.element.y = "Milind\n";
    instance.element.y[0] = '0';

    printf("%s",instance.element.y);
    return 0;
}