#include <psia.h>
#include <stdio.h>
#include <time.h>

int main(int argc, const char **argv) {
    psia_process_t process = create_psia_process("a", "dir", 1);
    if (process == NULL) {
        printf ("Process Creation Failed!\n");
        return -1;
    }
    psia_stdin_t proc_stdin = get_psia_process_stdin(process);
    psia_stdout_t proc_stdout = get_psia_process_stdout(process);

    char buffer[1024];
    psia_getline(proc_stdout, buffer);

    puts (buffer);
    
    int start = clock();
    while (clock() - start < 5000);
    destroy_psia_process(process);

    return 0;
}