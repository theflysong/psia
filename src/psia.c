#include <psia.h>
#include <arch/general_api.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>

psia_process_t create_psia_process(const char *image_name, char *argument, int redirect) {
    return (psia_process_t)__psia_arch_create_process(image_name, argument, redirect);
}

int destroy_psia_process(psia_process_t process) {
    return __psia_arch_destroy_process((void *)process);
}

psia_stdin_t get_psia_process_stdin(psia_process_t process) {
    return (psia_stdin_t)__psia_arch_get_stdin((void *)process);
}

psia_stdout_t get_psia_process_stdout(psia_process_t process) {
    return (psia_stdout_t)__psia_arch_get_stdout((void *)process);
}

int psia_writes(psia_stdin_t process_stdin, const char *data, int len) {
    return __psia_arch_writes((void *)process_stdin, data, len);
}

int psia_write_str(psia_stdin_t process_stdin, const char *str) {
    psia_writes(process_stdin, str, strlen(str));
}

int psia_reads(psia_stdout_t process_stdout, char *buffer, int len)  {
    return __psia_arch_reads((void *)process_stdout, buffer, len);
}

char psia_getchar(psia_stdout_t process_stdout) {
    char ch;
    psia_reads(process_stdout, &ch, sizeof(char));
    return ch;
}

int psia_printf(psia_stdin_t process_stdin, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    
    char buffer[1024] = "";
    
    vsprintf(buffer, fmt, args);
    int ret = psia_write_str(process_stdin, buffer);

    va_end(args);
    
    return ret;
}

char *psia_getline(psia_stdout_t process_stdout, char *buffer) {
    char *_buffer = buffer;
    char ch = psia_getchar(process_stdout);
    
    do {
        if (ch == '\0') {
            break;
        }
        *_buffer = ch;
        _buffer ++;
        ch = psia_getchar(process_stdout);
    } while (ch != '\n');

    *_buffer = '\0';

    return buffer;
}