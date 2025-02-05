/*
 * debug.h - Header for some debugging macros.
 *
 * Author: Philip R. Simonson
 * Date  : 06/20/2022
 *
 ****************************************************************************
 */

#ifndef _DEBUG_H_
#define _DEBUG_H_

#include <stdio.h>
#include <assert.h>
#include <errno.h>

/* Message subsitutions */
#define DEBUG_INFO "INFO"
#define DEBUG_WARN "WARNING"
#define DEBUG_ERR  "ERROR"

/* Debug info macros */
#define DEBUG_PRINT(T, M, ...) fprintf(stderr, "[" T ":%d]: " M "\n", __LINE__, ##__VA_ARGS__)
#define DEBUG_PRINTERR(M, ...) DEBUG_PRINT(DEBUG_ERR, M, ##__VA_ARGS__)
#define DEBUG_PRINTWARN(M, ...) DEBUG_PRINT(DEBUG_WARN, M, ##__VA_ARGS__)

/* Debug macros */
#define DEBUG_WARNING(A, B, ...) if(!(A)) { DEBUG_PRINTWARN(B, ##__VA_ARGS__); }
#define DEBUG_ERROR(A, B) if(!(B)) { goto error_##A; }

#define DEBUG_ERRDEF(A, B, C, D, ...) error_##A: DEBUG_PRINT(DEBUG_INFO, D, ##__VA_ARGS__); C; exit(B);
#define DEBUG_ERRDEF2(A, B, C, ...) error_##A: DEBUG_PRINT(DEBUG_INFO, C, ##__VA_ARGS__); exit(B);

#endif /* _DEBUG_H_ */
