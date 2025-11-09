#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <x86intrin.h>

#include "Logfile.h"
#include "utils.h"
#include "list.h"
#include "dump.h"

FILE* Logfile = fopen("Logfile.htm", "w");

void Logfile_close(void) {
    fclose(Logfile);
}
 
int main() {
    assert(Logfile);

    unsigned long int start = 0;
    unsigned long int end   = 0;

    List_t* list = List_Ctor();

    ListEl_t* node1 = List_Insert_After(list->fake_el, 10);

    for (int k = 1; k < 1e8; k *= 10) {

        start = __rdtsc();

        for (int i = 0; i < k; i++) {
            ListEl_t* node2 = List_Insert_After(node1, 10);
            List_Delete(list, node2);
        }

        end = __rdtsc();
        printf("%lu\n", end - start);
    }
    
    for (int k = 1; k < 1e8; k *= 10) 
        printf("%d\n", k);

    // ListEl_t* node2 = List_Insert_After(node1, 20);
    // ListEl_t* node3 = List_Insert_After(node2, 30);
    // ListEl_t* node4 = List_Insert_After(node3, 40);
    // ListEl_t* node5 = List_Insert_After(node4, 50);

    Graphic_Dump_List(list);

    List_Dtor(list);

    atexit(Logfile_close);
    return 0;
}