#ifndef __ARCH_GENERAL_API_HEADER__
#define __ARCH_GENERAL_API_HEADER__

// redirect=1 => redirect the stdio of process
void *__psia_arch_create_process(const char *image_name, char *argument, int redirect);
int __psia_arch_destroy_process(void *__psia_arch_process);
void *__psia_arch_get_stdin(void *__psia_arch_process);
void *__psia_arch_get_stdout(void *__psia_arch_process);
int __psia_arch_writes(void *__psia_arch_stdin, const char *data, int len);
int __psia_arch_reads(void *__psia_arch_stdout, char *buffer, int len);

#endif