#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void not_called() {
    printf("Enjoy your shell!\n");
    system("/bin/bash");
}
/*

gdb ./app_32
break main
break vulnerable_function
run AAAA
list
info args
bt
continue
list
info args
info locals
print &buffer
x/16bx &buffer
next
x/16bx &buffer
run AAAAAAAAAAAAAAAA
continue
x/32bx &buffer
x/32xw $esp
disassemble vulnerable_function
quit
*/
// amennyiben a bemenet hosszabb mint 8 byte, akkor kalap, mert a strcpy() nem véd az overflow ellen.
void vulnerable_function(char* string) {
    char buffer[8];
    strcpy(buffer, string);
}

int main(int argc, char** argv) {

    if (argc != 2) {
	   printf("Please specify one argument!\n");
	   return 0;
    }

    vulnerable_function(argv[1]);

    return 0;
}
