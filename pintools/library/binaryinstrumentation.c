#ifndef INSTRUMENTATION_H
#define INSTRUMENTATION_H

#include <stdint.h>
#include <stdio.h>

#include "binaryinstrumentation.h"

#define likely(x)       __builtin_expect(!!(x), 1)
#define unlikely(x)     __builtin_expect(!!(x), 0)

const char *filename = "trace.bin";
static FILE *out = NULL;

#define OPEN_LOG() if (unlikely(out == NULL)) { printf("opening file\n"); out = fopen(filename,"wb"); }

inline void fillFnLog(fnlog *fnl, char fn_event_type, int functionId) {
    fnl->fn_event_type = fn_event_type;
    fnl->functionId = functionId;
}

inline void fillAccessLog(accesslog *acl, void *ptr, char value_type, value_store value, int type, int file, int line, int col, int typeId, int varId) {
    acl->ptr = ptr;
    acl->value_type = value_type;
    acl->value = value;
    acl->type = type;
    acl->file = file;
    acl->line = line;
    acl->col = col;
    acl->typeId = typeId;
    acl->varId = varId;
}

inline void fillAllocLog(alloclog *all, void *addr, uint64_t size, int type, int file, int line, int col) {
    all->addr = addr;
    all->size = size;
    all->type = type;
    all->file = file;
    all->line = line;
    all->col = col;
}

void logFnBegin(int functionId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_FN;
    fillFnLog(&(le.entry.fn), FN_BEGIN, functionId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);
}

void logFnEnd(int functionId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_FN;
    fillFnLog(&(le.entry.fn), FN_END, functionId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);
}

void logAlloc(void *addr, uint64_t size, int type, int file, int line, int col) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ALLOC;
    fillAllocLog(&(le.entry.alloc), addr, size, type, file, line, col);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);
}

void logAccessPtr(void *ptr, void *value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();

    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.ptr = value;
    fillAccessLog(&(le.entry.access), ptr, PTR, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);

    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessI8(void *ptr, uint8_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.i8 = value;
    fillAccessLog(&(le.entry.access), ptr, I8, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessI16(void *ptr, uint16_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.i16 = value;
    fillAccessLog(&(le.entry.access), ptr, I16, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessI32(void *ptr, uint32_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.i32 = value;
    fillAccessLog(&(le.entry.access), ptr, I32, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessI64(void *ptr, uint64_t value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.i64 = value;
    fillAccessLog(&(le.entry.access), ptr, I64, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

/* =============================
   These don't exist: */

void logAccessF8(void *ptr, uint8_t value, int type, int file, int line, int col, int typeId, int varId) {
    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessF16(void *ptr, uint16_t value, int type, int file, int line, int col, int typeId, int varId) {
    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

/* ============================= */

void logAccessF32(void *ptr, float value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.f32 = value;
    fillAccessLog(&(le.entry.access), ptr, F32, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}

void logAccessF64(void *ptr, double value, int type, int file, int line, int col, int typeId, int varId) {
    OPEN_LOG();
    logentry le;
    le.entry_type = LOG_ACCESS;
    value_store vs;
    vs.f64 = value;
    fillAccessLog(&(le.entry.access), ptr, F64, vs, type, file, line, col, typeId, varId);
    fwrite(&le, sizeof(logentry), 1, out);
    fflush(out);

    /*fprintf(out, "%p %llu %c %d %d %d %d %d\n", ptr,[> value, <]type, file, line, col, typeId, varId);*/
}
#endif
