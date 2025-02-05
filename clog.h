/*
 * clog - Simple program logging system.
 *
 * Author: Philip R. Simonson
 * Date:   06/22/2024
 *
 */

#define MAXBUF 256

#define ERROPEN "Error: Open failed!"
#define ERRNMEM "Error: Out of memory!"
#define ERRWRITE "Error: Write failed!"
#define ERRREAD "Error: Read failed!"

#define MSGERROR(A,M,...) log_printf(A, "[CLOG:ERROR]: " M, ##__VA_ARGS__)
#define MSGWARN(A,M,...) log_printf(A, "[CLOG:WARN]: " M, ##__VA_ARGS__)
#define MSGINFO(A,M,...) log_printf(A, "[CLOG:INFO]: " M, ##__VA_ARGS__)

struct Log {
	FILE *fp;
	char name[MAXBUF];
	char data[MAXBUF];
	int length;
	size_t size;
	int err;
	char *msg;
};

void log_init(struct Log *l, const char *name);
void log_open(struct Log *l, int mode);
void log_close(struct Log *l);
void log_read(struct Log *l);
void log_printf(struct Log *l, const char *msg, ...);

