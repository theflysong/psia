#include <arch/general_api.h>
#include <windows.h>
#include <stdlib.h>

typedef struct {
    // stdio of process
    HANDLE stdin;
    HANDLE stdout;
    HANDLE stderr;
    // handles to operate stdio
    HANDLE write_stdin;
    HANDLE read_stdout;
    HANDLE read_stderr;
    // process infomation
    PROCESS_INFORMATION process;
} __psia_arch_windows_process_t;

void *__psia_arch_create_process(const char *image_name, char *argument) {
    __psia_arch_windows_process_t *process = (__psia_arch_windows_process_t *)malloc(sizeof(__psia_arch_windows_process_t));
    memset(process, 0, sizeof(__psia_arch_windows_process_t));

    char _image_name[512];
    if (strlen(image_name) + 4 > 512) {
        return NULL;
    }
    strcpy(_image_name, image_name);
    strcat(_image_name, ".exe");

    SECURITY_ATTRIBUTES se_attr = {
        .nLength = sizeof(SECURITY_ATTRIBUTES),
        .bInheritHandle = TRUE,
        .lpSecurityDescriptor = NULL
    };

    if(! CreatePipe(&process->stdin, &process->write_stdin, &se_attr, 0)) {
        free(process);
        return NULL;
    }

    if(! CreatePipe(&process->read_stdout, &process->stdout, &se_attr, 0)) {
        free(process);
        return NULL;
    }

    if(! CreatePipe(&process->read_stderr, &process->stderr, &se_attr, 0)) {
        free(process);
        return NULL;
    }

    PROCESS_INFORMATION process_info = {
    };

    STARTUPINFO startup_info = {
        .cb = sizeof(STARTUPINFO),
        .dwFlags = STARTF_USESTDHANDLES,
        .hStdInput = process->stdin,
        .hStdOutput = process->stdout,
        .hStdError = process->stderr
    };

    if(! CreateProcess(
        _image_name, argument,
        NULL, NULL, TRUE,
        0, NULL, NULL,
        &startup_info, &process_info
    )) {
        __psia_arch_destroy_process((void *)process);
        return NULL;
    }

    process->process = process_info;
    return (void *)process;
}

static int destroy_handle(HANDLE handle) {
    if (handle != NULL) {
        if (CloseHandle(handle)) {
            return 1;
        }
    }
    return 0;
}

int __psia_arch_destroy_process(void *__psia_arch_process) {
    __psia_arch_windows_process_t *process = (__psia_arch_windows_process_t *)__psia_arch_process;
    int failed_count = 0;
    if (! destroy_handle(process->stdin)) {failed_count ++;}
    if (! destroy_handle(process->stdout)) {failed_count ++;}
    if (! destroy_handle(process->stderr)) {failed_count ++;}
    if (! destroy_handle(process->write_stdin)) {failed_count ++;}
    if (! destroy_handle(process->read_stdout)) {failed_count ++;}
    if (! destroy_handle(process->read_stderr)) {failed_count ++;}
    if (! destroy_handle(process->process.hProcess)) {failed_count ++;}
    if (! destroy_handle(process->process.hThread)) {failed_count ++;}
    free(process);
    return failed_count == 0;
}

void *__psia_arch_get_stdin(void *__psia_arch_process) {
    __psia_arch_windows_process_t *process = (__psia_arch_windows_process_t *)__psia_arch_process;
    return (void *)(&process->write_stdin);
}

void *__psia_arch_get_stdout(void *__psia_arch_process) {
    __psia_arch_windows_process_t *process = (__psia_arch_windows_process_t *)__psia_arch_process;
    return (void *)(&process->read_stdout);
}

int __psia_arch_writes(void *__psia_arch_stdin, const char *data, int len) {
    HANDLE *write_stdin = (HANDLE *)__psia_arch_stdin;
    DWORD write_len = 0;

    if (! WriteFile(*write_stdin, data, len, &write_len, (LPOVERLAPPED)NULL)) {
        return 0;
    }

    return write_len;
}

int __psia_arch_reads(void *__psia_arch_stdout, char *buffer, int len) {
    HANDLE *read_stdout = (HANDLE *)__psia_arch_stdout;
    DWORD read_len = 0;

    if (! ReadFile(*read_stdout, buffer, len, &read_len, (LPOVERLAPPED)NULL)) {
        return 0;
    }

    return read_len;
}