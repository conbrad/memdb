#ifndef BINARY_INSTRUMENTATION_H
#define BINARY_INSTRUMENTATION_H

#include <stdint.h>

enum fn_events {
    FN_BEGIN, FN_END
};

typedef struct _fnlog {
    char fn_event_type;
    int functionId;
} fnlog;

enum value_type {
    I8, I16, I32, I64,
    F32, F64, PTR
};

typedef union {
    void *ptr;
    uint8_t i8;
    uint16_t i16;
    uint32_t i32;
    uint64_t i64;
    float f32;
    double f64;
} value_store;

typedef struct _accesslog {
    void *ptr;
    char value_type;
    value_store value;
    int type;
    uint16_t file; // these are originally 32bit, but it's probably safe to reduce them for now
    uint16_t line;
    uint16_t col;
    uint16_t typeId;
    uint16_t varId;
} accesslog;

typedef struct _alloclog {
    void *addr;
    uint64_t size;
    uint64_t num;
    int type;
    int file;
    int line;
    int col;
} alloclog;

enum entry_types {
    LOG_FN, LOG_ALLOC, LOG_ACCESS
};

typedef struct _logentry {
    char entry_type;
    union {
        fnlog fn;
        accesslog access;
        alloclog alloc;
    } entry;
} logentry;

#endif
