#include <psia.h>
#include <stdio.h>

int main(int argc, const char **argv) {
    psia_process_t process = create_psia_process("a", "");
    if (process == NULL) {
        printf ("Process Creation Failed!\n");
        return -1;
    }
    psia_stdin_t proc_stdin = get_psia_process_stdin(process);
    psia_stdout_t proc_stdout = get_psia_process_stdout(process);

    psia_printf(proc_stdin, "2 3\n");

    char buffer[1024];
    psia_getline(proc_stdout, buffer);

    puts (buffer);
    return 0;
}