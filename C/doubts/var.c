// gcc -S -masm=intel var.c  

int bb; // it is global, bss section

char ga = 'a'; // it is global, data section

static int sgi = 2; // it is non global, data section

int gi = 3; //it is global, data section

const int gci = 27; // it is global, rodata

static int sibs;    // it is non global, it has .local


void main() {
    int a = 25;                     // stored in stack 
    const int bc = 34;              //  stored in stack
    static int si = 12;         // It is not stored in stack, data section
    
    ga = 'b';
    gi = 2000;
    sgi = 12;

    a = 27;
    si = 369;
    sibs =2;
}

// see how data are stored in which segment in var_bak.s