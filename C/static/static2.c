int g;             // this is available to all the files when compiled
static int g1 = 8; // this is visible to static2.c only
// static is global but with in this file only

int increment()
{
    return ++g;
}

int incrementStatic()
{
    return ++g1;
}

int incstc()
{
    static int c; // This also declared and defined once. Then the state of the variable is maintained until program end
    // c is accessable only inside incstc
    return ++c;
}

int consins()
{
    // int var = 3;
    // static int c1 = var; error: initializer element is not constant (static is initlize only by constant)

    const int var = 3;
    static int c = var; // even though it is initalized every time but is value is not changed
    return ++c;
}

// it is static function it will be access only inside this file
static int statFunction()
{
    printf("ST function");
}