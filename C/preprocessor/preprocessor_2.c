/**
 * Author: E.K. Jithendiran
*/
void init();
void __attribute__((destructor)) end();
int returnval();
// #pragma startup init
// #pragma exit end

// the above line wil not work in GCC

int main()
{
    /**
     * #pragma is used to turn on/off certain feature
     * this macro will varies compiler to compiler
     * this macro can alter the behaviour of the c program
     *
     * Using pragms we can supress warning
     */

    // in this program printf will raise warning
    // to supress this warning #pragma will be used
    // warning not properly works
    #pragma warning(suppress: 24; suppress: 25; suppress: 29)
    #pragma warning(disable:4700)
    // many more pragma is there
    int c = 0;
    printf("in Main method\n");
    printf("C = %d\n", c);
    returnval();
    return 0;
    c++;
    printf("C = %d\n", c);
}

int returnval() {
    printf("inside return val\n");
}

void __attribute__((constructor)) init()
{
    printf("Init method\n");
}

void end()
{
    printf("end method\n");
}

/**
 * Op
    Init method
    in Main method
    C = 0
    end method
*/

// the below code for other than GCC
// void init() {
//     printf("Init method\n");
// }

// void end() {
//     printf("end method");
// }