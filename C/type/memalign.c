#include<stdio.h>

int main(){

    int z; // start address 0x7ffcbd8f370c, end address 0x7ffcbd8f370f
    int a; // start 0x7ffcbd8f3710
    char c; // padding
    float f;
    char cc1,cc2,cc3, cc4;
    

    printf("sizeof(int): %d, sizeof(char): %d, sizeof(float): %d\n",sizeof(int), sizeof(char), sizeof(float)); // sizeof(int): 4, sizeof(char): 1, sizeof(float): 4
    // stack grows down so mem address is decreasing 
    // x86_64 is little-endian 

    // usually to find end of var, we do (((int*)&z) + 1), but the actual end address is -1 of caluculated one
    printf("Start address of z: %p, %ld, End: %p, %ld \n", &z, &z, (&z + 1), (&z + 1)); // it's start is next one end
    printf("Start address of a: %p, %ld, End: %p, %ld \n", &a, &a, (&a + 1), (&a + 1));
    printf("Start address of c: %p, %ld, End: %p, %ld \n", &c, &c, (&c + 1), (&c + 1)); // it's start is not match with next end, because of padding
    printf("Start address of f: %p, %ld, End: %p, %ld \n", &f, &f, (&f + 1), (&f + 1));
    printf("Start address of cc1: %p, %ld, End: %p, %ld \n", &cc1, &cc1, (&cc1 + 1), (&cc1 + 1));
    printf("Start address of cc2: %p, %ld, End: %p, %ld \n", &cc2, &cc2, (&cc2 + 1), (&cc2 + 1));
    printf("Start address of cc3: %p, %ld, End: %p, %ld \n", &cc3, &cc3, (&cc3 + 1), (&cc3 + 1));
    printf("Start address of cc4: %p, %ld, End: %p, %ld \n", &cc4, &cc4, (&cc4 + 1), (&cc4 + 1));
    

    printf("Diff address of z - a: %d\n", (int)(&z) - (int)(&a)); // Diff address of z - a: 4 // size of a is 4
    printf("Diff address of a - c: %d\n", (int)(&a) - (int)(&c)); // Diff address of a - c: 1 // size of c is 1
    printf("Diff address of c - f: %d\n", (int)(&c) - (int)(&f)); // Diff address of c - f: 7 // size of f is 7, because padding
    printf("Diff address of f - cc1: %d\n", (int)(&f) - (int)(&cc1));
    printf("Diff address of cc1 - cc2: %d\n", (int)(&cc1) - (int)(&cc2));
    printf("Diff address of cc2 - cc3: %d\n", (int)(&cc2) - (int)(&cc3));
    printf("Diff address of cc3 - cc3: %d\n", (int)(&cc3) - (int)(&cc4));
    

    printf("Address divisible of it's size should be 0: %d, %d, %d, %d, %d, %d, %d\n",
        ((int)(&z) % sizeof(int)) , ((int)(&a) % sizeof(int)),  ((int)(&c) % sizeof(char)),  
        ((int)(&f) % sizeof(float)), ((int)(&cc1) % sizeof(char)), ((int)(&cc2) % sizeof(char)), ((int)(&cc3) % sizeof(char)));


    printf("\n\n");
    char c1; // padding
    int z1; // address should be divisible of 4, so padding
    int a1;
    float f1;
    char c11, c12, c13; 
    char c14;
    int x1;
    printf("Start address of c: %p, %ld, End: %p, %ld \n", &c1, &c1, (&c1 + 1), (&c1 + 1));
    printf("Start address of z: %p, %ld, End: %p, %ld \n", &z1, &z1, (&z1 + 1), (&z1 + 1));
    printf("Start address of a: %p, %ld, End: %p, %ld \n", &a1, &a1, (&a1 + 1), (&a1 + 1));
    printf("Start address of f: %p, %ld, End: %p, %ld \n", &f1, &f1, (&f1 + 1), (&f1 + 1));
    printf("Start address of c11: %p, %ld, End: %p, %ld \n", &c11, &c11, (&c11 + 1), (&c11 + 1));
    printf("Start address of c12: %p, %ld, End: %p, %ld \n", &c12, &c12, (&c12 + 1), (&c12 + 1));
    printf("Start address of c13: %p, %ld, End: %p, %ld \n", &c13, &c13, (&c13 + 1), (&c13 + 1));
    printf("Start address of c14: %p, %ld, End: %p, %ld \n", &c14, &c14, (&c14 + 1), (&c14 + 1));
    printf("Start address of x1: %p, %ld, End: %p, %ld \n", &x1, &x1, (&x1 + 1), (&x1 + 1));

    printf("Diff address of c1 - z1: %d\n", (int)(&c1) - (int)(&z1)); 
    printf("Diff address of z1 - a1: %d\n", (int)(&z1) - (int)(&a1)); 
    printf("Diff address of a1 - f1: %d\n", (int)(&a1) - (int)(&f1)); 
    printf("Diff address of f1 - c11: %d\n", (int)(&f1) - (int)(&c11));
    printf("Diff address of c11 - c12: %d\n", (int)(&c11) - (int)(&c12));
    printf("Diff address of c12 - c13: %d\n", (int)(&c12) - (int)(&c13));

    // with c14
    printf("Diff address of c13 - c14: %d\n", (int)(&c13) - (int)(&c14));
    printf("Diff address of c14 - x1: %d\n", (int)(&c14) - (int)(&x1));
    // without c14
    // printf("Diff address of c13 - x1: %d\n", (int)(&c13) - (int)(&x1));
    

    printf("\n\n");

    char c2, c21, c22, c23;
    // char c24; // should not be here, else padding will be added
    int z2; 
    int a2;
    float f2;
    char c24;

    printf("Start address of c2: %p, %ld, End: %p, %ld \n", &c2, &c2, (&c2 + 1), (&c2 + 1));
    printf("Start address of c21: %p, %ld, End: %p, %ld \n", &c21, &c21, (&c21 + 1), (&c21 + 1));
    printf("Start address of c22: %p, %ld, End: %p, %ld \n", &c22, &c22, (&c22 + 1), (&c22 + 1));
    printf("Start address of c23: %p, %ld, End: %p, %ld \n", &c23, &c23, (&c23 + 1), (&c23 + 1));

    // printf("Start address of c24: %p, %ld, End: %p, %ld \n", &c24, &c24, (&c24 + 1), (&c24 + 1));

    printf("Start address of z2: %p, %ld, End: %p, %ld \n", &z2, &z2, (&z2 + 1), (&z2 + 1));
    printf("Start address of a2: %p, %ld, End: %p, %ld \n", &a2, &a2, (&a2 + 1), (&a2 + 1));
    printf("Start address of f2: %p, %ld, End: %p, %ld \n", &f2, &f2, (&f2 + 1), (&f2 + 1));
    printf("Start address of c24: %p, %ld, End: %p, %ld \n", &c24, &c24, (&c24 + 1), (&c24 + 1));

    
    
    printf("Diff address of c2 - c21: %d\n", (int)(&c2) - (int)(&c21));
    printf("Diff address of c21 - c22: %d\n", (int)(&c21) - (int)(&c22));
    printf("Diff address of c22 - c23: %d\n", (int)(&c22) - (int)(&c23)); 

    // printf("Diff address of c23 - c24: %d\n", (int)(&c23) - (int)(&c24)); 
    // printf("Diff address of c24 - z2: %d\n", (int)(&c24) - (int)(&z2)); 

    printf("Diff address of c23 - z2: %d\n", (int)(&c23) - (int)(&z2)); 
    printf("Diff address of z2 - a2: %d\n", (int)(&z2) - (int)(&a2)); 
    printf("Diff address of a2 - f2: %d\n", (int)(&a2) - (int)(&f2)); 
    printf("Diff address of f2 - c24: %d\n", (int)(&f2) - (int)(&c24)); 
}

/**
sizeof(int): 4, sizeof(char): 1, sizeof(float): 4
Start address of z: 0x7ffcbd8f370c, 140723488765708, End: 0x7ffcbd8f3710, 140723488765712 
Start address of a: 0x7ffcbd8f3708, 140723488765704, End: 0x7ffcbd8f370c, 140723488765708 
Start address of c: 0x7ffcbd8f3707, 140723488765703, End: 0x7ffcbd8f3708, 140723488765704 // padding
Start address of f: 0x7ffcbd8f3700, 140723488765696, End: 0x7ffcbd8f3704, 140723488765700 
Start address of cc1: 0x7ffcbd8f36ff, 140723488765695, End: 0x7ffcbd8f3700, 140723488765696 
Start address of cc2: 0x7ffcbd8f36fe, 140723488765694, End: 0x7ffcbd8f36ff, 140723488765695 
Start address of cc3: 0x7ffcbd8f36fd, 140723488765693, End: 0x7ffcbd8f36fe, 140723488765694 
Start address of cc4: 0x7ffcbd8f36fc, 140723488765692, End: 0x7ffcbd8f36fd, 140723488765693 
Diff address of z - a: 4
Diff address of a - c: 1
Diff address of c - f: 7
Diff address of f - cc1: 1
Diff address of cc1 - cc2: 1
Diff address of cc2 - cc3: 1
Diff address of cc3 - cc3: 1
Address divisible of it's size should be 0: 0, 0, 0, 0, 0, 0, 0


Start address of c: 0x7ffcbd8f36fb, 140723488765691, End: 0x7ffcbd8f36fc, 140723488765692  // padding
Start address of z: 0x7ffcbd8f36f4, 140723488765684, End: 0x7ffcbd8f36f8, 140723488765688 
Start address of a: 0x7ffcbd8f36f0, 140723488765680, End: 0x7ffcbd8f36f4, 140723488765684 
Start address of f: 0x7ffcbd8f36ec, 140723488765676, End: 0x7ffcbd8f36f0, 140723488765680 
Start address of c11: 0x7ffcbd8f36eb, 140723488765675, End: 0x7ffcbd8f36ec, 140723488765676 
Start address of c12: 0x7ffcbd8f36ea, 140723488765674, End: 0x7ffcbd8f36eb, 140723488765675 
Start address of c13: 0x7ffcbd8f36e9, 140723488765673, End: 0x7ffcbd8f36ea, 140723488765674 
Start address of c14: 0x7ffcbd8f36e8, 140723488765672, End: 0x7ffcbd8f36e9, 140723488765673 
Start address of x1: 0x7ffcbd8f36e4, 140723488765668, End: 0x7ffcbd8f36e8, 140723488765672 
Diff address of c1 - z1: 7
Diff address of z1 - a1: 4
Diff address of a1 - f1: 4
Diff address of f1 - c11: 1
Diff address of c11 - c12: 1
Diff address of c12 - c13: 1
Diff address of c13 - c14: 1
Diff address of c14 - x1: 4


Start address of c2: 0x7ffcbd8f36e3, 140723488765667, End: 0x7ffcbd8f36e4, 140723488765668 
Start address of c21: 0x7ffcbd8f36e2, 140723488765666, End: 0x7ffcbd8f36e3, 140723488765667 
Start address of c22: 0x7ffcbd8f36e1, 140723488765665, End: 0x7ffcbd8f36e2, 140723488765666 
Start address of c23: 0x7ffcbd8f36e0, 140723488765664, End: 0x7ffcbd8f36e1, 140723488765665 
Start address of z2: 0x7ffcbd8f36dc, 140723488765660, End: 0x7ffcbd8f36e0, 140723488765664 
Start address of a2: 0x7ffcbd8f36d8, 140723488765656, End: 0x7ffcbd8f36dc, 140723488765660 
Start address of f2: 0x7ffcbd8f36d4, 140723488765652, End: 0x7ffcbd8f36d8, 140723488765656 
Start address of c24: 0x7ffcbd8f36d3, 140723488765651, End: 0x7ffcbd8f36d4, 140723488765652 
Diff address of c2 - c21: 1
Diff address of c21 - c22: 1
Diff address of c22 - c23: 1
Diff address of c23 - z2: 4
Diff address of z2 - a2: 4
Diff address of a2 - f2: 4
Diff address of f2 - c24: 1
 */