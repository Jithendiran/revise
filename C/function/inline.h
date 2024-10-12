inline int add(int a, int b) {
    return a + b; 
}

int sub(int a, int b) {
    return a - b;
}

static inline int divide(int a, int b) {
    return a / b;
}

/**
 * Why static inline not needed exten keyword and other non static function needed ?
 * we know static is only for particular file so compiler will use local function
 * 
 * if function is non static, it may be a chance of redeclaring a function in different file 
 * so compiler insists to put extern
*/