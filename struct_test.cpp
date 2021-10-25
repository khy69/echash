#include <stdio.h>
typedef struct 
    {
        bool is_purging: 1;
        bool is_processing_input: 1;
        bool is_time_for_rebuild: 1;
        bool is_parsing: 1;
        bool is_test:5;
    } state;
int main() {
    state test;
    printf("%d %d",sizeof(bool),sizeof(test));
    return 0;
}