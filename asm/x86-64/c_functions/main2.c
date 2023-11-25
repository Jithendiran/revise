/**
 * arguments of function is pushed to stack
 * address are incremented by 4
 * 1st base pointer address will be stored 
 * 2nd return address will be stored
 * then followed by aguments 3rd 1st argument and last 2 nd arguments
*/
int add(int a, int b) {
    int aa = 27;
    int sum = 0;
    sum = a + b;
    return sum;
}

void main() {
    int a = 2;
    add(a, 4);
}