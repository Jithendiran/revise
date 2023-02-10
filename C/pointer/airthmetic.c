#include <stdio.h>
void main()
{
    // possible airthmetic operation in pointer is +/-
    // pointer airthmetic is based on " address operation (number)*(type of pointer)  "
    // for + -> address + (number) * (type of pointer)
    // for - -> address - (number) * (type of pointer)

    int a[] = {1, 2, 3};
    int *p = a;

    p = a;
    int *p1 = &a[2];
    printf("p1 - p : %d, %p\n", p1 - p, p1 - p); // p1 - p : 2, 0x2

    for (int i = 0; i < 3; i++)
    {
        // add
        printf("%d ", *(a + i)); // 1 2 3
    }
    printf("\n");
    for (int i = 2; i > -1; i--) // 1 2 3
    {
        // subract
        // (a+2) is used for force to point address of last array value
        printf("%d ", *((a + 2) - i));
    }
    printf("\n");
    p++;                              // increment pointer by 1
    printf("p points to : %d\n", *p); // p points to : 2
    p--;
    printf("p points to : %d\n", *p); // p points to : 1

    ++p;                              // increment pointer by 1
    printf("p points to : %d\n", *p); // p points to : 2
    --p;
    printf("p points to : %d\n", *p); // p points to : 1

    // p = p * 2; // error: invalid operands to binary * (have 'int *' and 'int')
    int *q = a + 1;
    // p = p + q; //error: invalid operands to binary + (have 'int *' and 'int *')
    p = p - q;
    printf("p points to : %d\n", *p); // Segmentation fault (undefined behaviour)
}