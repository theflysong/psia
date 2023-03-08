#ifndef __PSIA_HEADER__
#define __PSIA_HEADER__

typedef void *psia_process_t;
typedef void *psia_stdin_t;
typedef void *psia_stdout_t;

// redirect=1 => redirect the stdio of process
psia_process_t create_psia_process(const char *image_name, char *argument, int redirect);
int destroy_psia_process(psia_process_t process);
psia_stdin_t get_psia_process_stdin(psia_process_t process);
psia_stdout_t get_psia_process_stdout(psia_process_t process);

int psia_writes(psia_stdin_t process_stdin, const char *data, int len);
int psia_write_str(psia_stdin_t process_stdin, const char *str);
int psia_reads(psia_stdout_t process_stdout, char *buffer, int len);
char psia_getchar(psia_stdout_t process_stdout);

int psia_printf(psia_stdin_t process_stdin, const char *fmt, ...);
char *psia_getline(psia_stdout_t process_stdout, char *buffer);

#endif