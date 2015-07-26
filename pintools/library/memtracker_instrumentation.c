#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H

#include <stdint.h>
#include <stdio.h>

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

const char *filename = "access.trace";
static FILE *out = NULL;

// Default thread ID expected by memtracker format,
// currently unused
const int TID = 0;

// Simple array-based stack for keeping trace
// of functions
#define MAX_SIZE 100
#define EMPTY -1
#define OPEN_LOG() if (unlikely(out == NULL)) { printf("opening file\n"); out = fopen(filename,"w"); }
int top = EMPTY;
int items[MAX_SIZE];

void push(int functionId) {
   items[++top] = functionId;
}

int pop() {
   return items[top--];
}

int peek() {
	return items[top];
}

int full()  {
   return top+1 == MAX_SIZE;
}

int empty()  {
   return top == EMPTY;
}

typedef enum
{
  FALSE,
  TRUE
} boolean;

boolean debugInfoAvailable(int line, int col) {
	if(line == -1 || col == -1) {
		return FALSE;
	}
	return TRUE;
}

void logFnBegin(int functionId) {
	push(functionId);
}

void logFnEnd(int functionId) {
	pop();
}

void logAlloc(void *addr, uint64_t size, int type, int file, int line, int col) {
    OPEN_LOG();
    if(debugInfoAvailable(line, col)) {
    	fprintf(out, "%p %lu %d %d %d %d\n", addr, size, type, file, line, col);
    	fflush(out);
    }
}

void logAccessPtr(void *ptr, void *value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%lu ", sizeof(ptr));
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

        fflush(out);
    }
}

void logAccessI8(void *ptr, uint8_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 1);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

        fflush(out);
    }
}

void logAccessI16(void *ptr, uint16_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 2);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

    	//fprintf(out, "%p %hu %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
        fflush(out);
    }
}

void logAccessI32(void *ptr, uint32_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 4);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);
        //	fprintf(out, "%p %u %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
        fflush(out);
    }
}

void logAccessI64(void *ptr, uint64_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 8);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

    //	fprintf(out, "%p %lu %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
        fflush(out);
    }
}

/* =============================
 These don't exist: */

void logAccessF8(void *ptr, uint8_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
	fprintf(out, "%p %c %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
    fflush(out);

	/*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessF16(void *ptr, uint16_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
	fprintf(out, "%p %hu %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
    fflush(out);

	/*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

/* ============================= */

void logAccessF32(void *ptr, float value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 4);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

    //	fprintf(out, "%p %f %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
        fflush(out);
    }
}

void logAccessF64(void *ptr, double value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    if(debugInfoAvailable(line, col)) {
        fprintf(out, "%c ", type);
        fprintf(out, "%d ", TID);
        fprintf(out, "%p ", ptr);
        fprintf(out, "%d ", 8);
        fprintf(out, "%d ", peek());
        fprintf(out, "%d:%d:%d ", file, line, col);
        fprintf(out, "%d ", varId);
        fprintf(out, "%d\n", typeId);

    //	fprintf(out, "%p %lf %c %d %d %d %d %d\n", ptr, value, type, file, line, col, typeId, varId);
        fflush(out);
    }
}
#endif

