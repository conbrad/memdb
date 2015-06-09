### UNDERSTANDING MEMTRACKER TRACES

You don't have to understand memtracker traces if you use memtracker2json and memvis to visualize them. This information is intended for those who want to do some else with the traces. 

Here is an excerpt from a trace that memtracker collects:

```
read: 0 0x00007fff802a1bc0 8 calloc <unknown>
function-end: 0 calloc
alloc: 0 0x0000000001ccef10 __wt_calloc 3776 1 /cs/systems/home/fedorova/Work/WiredTiger/wt-dev/build_posix/../src/conn/conn_api.c:1216 conn
function-end: 0 __wt_calloc
read: 0 0x00007f771eefcf38 8 wiredtiger_open /cs/systems/home/fedorova/Work/WiredTiger/wt-dev/build_posix/../src/include/mutex.i:172
write: 0 0x0000000001ccef10 8 wiredtiger_open /cs/systems/home/fedorova/Work/WiredTiger/wt-dev/build_posix/../src/conn/conn_api.c:1217 /cs/systems/home/fedorova/Work/WiredTiger/wt-dev/build_posix/../src/conn/conn_api.c:1216 conn
```

In this example we see five different record types:

1. Allocation record
2. Function delimiter record
3. Memory access record without call-site information 
4. Memory access record with call-site information, but without data-source information
5. Memory access record with call-site information and with data-source information

Let's go into detail of what these records show.

**Allocation record**: This record type is prefixed with "alloc:" and has the following fields:

* thread id
* allocated address
* size of the allocation
* number of items
* source file and line from which the allocation was made
* the name of the variable for which we allocated space.

**Function delimiter record**: Prefixed with "function-begin:" or "function-end:". The fields are:

* thread id
* function name

**Memory access record without call-site information**: Prefixed with "read:" or "write:". The fields are:

* thread id
* memory address 
* size of the accessed data
* function from which the access is made
* <unknown> to indicate that we could not obtain the source file/line information

An example of such a record is the first one in the above trace excerpt.

**Memory access record with call-site information, but without data-source information**: This record has the same format as the type 3 above, but the last field contains the source file/line of the access instead of <unknown>. The fifth record in the above excerpt is an example of such a record. 

**Memory access record with call-site information and with data-source information:** This record is the same as the type 4 above, but we have two additional fields at the end:

* the source code location of the dynamic memory allocation corresponding to this access
* the name of the variable to which this access is made. 