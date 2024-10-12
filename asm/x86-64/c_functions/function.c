int custom_function(int a, int b) {
    return a+b;
}

/**
 * How this function is going to translate
 * push rbp
 * mov rbp, rsp
 * mov DWORD PTR [rbp-0x4], edi
 * mov DWORD PTR [rbp-0x8], esi
 * mov edx, DWORD PTR [rbp-0x4]
 * mov eax, DWORD PTR [rbp-0x8]
 * add eax, edx
 * pop rbp
 * ret
 * 
 ref: https://blog.holbertonschool.com/hack-virtual-memory-stack-registers-assembly-code/
*/