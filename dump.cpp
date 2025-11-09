#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "Logfile.h"
#include "utils.h"
#include "list.h"
#include "verify.h"
#include "dump.h"

ListErr_t Graphic_Dump_List(const List_t* list) {
    assert(list);

    static const size_t MAX_DOT_CMD_LEN     = 256;
    char create_image_CMD [MAX_DOT_CMD_LEN] = {0};

    SAFE_FOPEN(graphviz_file, "dump_files/graphviz.txt", "w")

    Print_List_to_Graphviz(list, graphviz_file);

    fprintf(graphviz_file, "\n}\n");

    fclose(graphviz_file);

    SAFE_SNPRINTF(create_image_CMD, MAX_DOT_CMD_LEN, "dot -Tpng dump_files/graphviz.txt -o dump_files/image.png");

    system(create_image_CMD);

    Dump_List_to_HTML("dump_files/image.png");

    return END_WITH_SUC;
}
ListErr_t Print_List_to_Graphviz(const List_t* list, FILE* output_file) {
    assert(output_file);

    fprintf(output_file, "digraph Classic_List {\n");
    fprintf(output_file, "\trankdir = LR;\n");
    fprintf(output_file, "\tnode [" SHAPE STYLE " fillcolor = " BRIGHT_GREEN ", color = darkgreen, width = 1, height = 1];\n");
    fprintf(output_file, "\tedge [color = \"darkgreen\"];\n\n");

    fprintf(output_file, "\tlist_elem_%d [label = \" POISON | %p | {%p | %p}\"]\n", FAKE_EL(list), FAKE_EL(list), get_head( (List_t*) list), get_tail( (List_t*) list));
    
    Print_List_Elem_to_Graphviz(list, get_head( (List_t*) list), output_file);

    return END_WITH_SUC;
}

ListErr_t Print_List_Elem_to_Graphviz(const List_t* list, const ListEl_t* list_elem, FILE* output_file) {
    assert(list_elem);
    assert(output_file);

    fprintf(output_file, "\tlist_elem_%d [label = \" %d | %p | {%p | %p}\"]\n", list_elem, DATA(list_elem), list_elem, PREV(list_elem), NEXT(list_elem));

    if (PREV(NEXT(list_elem)) == list_elem)
        fprintf(output_file, "\tlist_elem_%d -> list_elem_%d [dir = both]\n", list_elem, NEXT(list_elem));

    if (NEXT(list_elem) != FAKE_EL(list))
        Print_List_Elem_to_Graphviz(list, NEXT(list_elem), output_file);

    return END_WITH_SUC;
}

ListErr_t Dump_List_to_HTML(const char* image_file_name) {

    fprintf(Logfile, "<pre>\n");
    fprintf(Logfile, "\t<h2> Dump of tree: </h2>\n");

    fprintf(Logfile, "\n<h3>\tImage:</h3>\n");
    fprintf(Logfile, "\t<img src = %s width = \"750\" height = \"100\">\n\n", image_file_name);

    fprintf(Logfile, "</pre>");

    return END_WITH_SUC;
}