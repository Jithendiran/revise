// gcc -S -masm=intel fun.c  

static int b;  // local, comm

int func(){  // global
    static int aa; // named as aa.2
    aa++;
    b++;
    return aa;
}

int func2( int c){// global
// c is taken from stack
    static int aa = 1;  // named as aa.1, it is stored in data segment, it has no .local, .comm
    aa++;
    return aa + c;
}

static int fun3(int a){ // non global
// a is taken from stack
    static int aa;      // named as aa.0
    aa++;
    int ab = 2;         // stack avriable
    return aa + a + ab; 
}

// details see in fun_bak.