
# memdb

This repository contains a family of tools to track memory accesses in applications and to visualize memory access patterns in order to reveal opportunities for program optimization. 

At the moment, the tools allow you to detect sharing (true and false) in multithreaded C applications in order to debug scalability problems. To visualize data sharing in your application, you need to use the following three tools in the order that they are listed:

* [memtracker](pintools/docs/memtracker.md) -- a Pin tool to collect a detailed memory trace of your running application
* [memtracker2json](pintools/docs/memtracker2json.md) -- a script that converts the Pin trace to JSON format
* [memvis](pintools/docs/memvis.md) -- a tool that analyzes the JSON trace and produces HTML and JavaScript to visualize it.

You can run them sequentially or as a pipeline. If you run them sequentially, what needs to happen is this:

 1. memtracker will spit log records to stdout. Redirect this output to a file, say log.txt. (See scripts/memtracker.sh for an example.) 
 2. Run memtracker2json.py -- supply log.txt as the input. Memtracker2json will output the JSON trace to the stdout. Save this trace to a file. (See the documentation below for an example). 
 3. Run memvis, providing the JSON trace generated above as an input. See the memvis documentation (below) to visualize the output in the browser. 

An alternative to running the tools sequentially is to pipe the output of each tool into the input of the next tool. This removes the need to save the traces. For an example, take a look at scripts/memtracker+m2j.sh.


##### Limiting the scope of tracking memory accesses

Tracking every memory access in the entire program is very expensive. It will produce very large traces (roughly 1GB for every second of single-threaded execution) and will significantly slow down the program (more than 10,000 times). To limit these effects, you may opt to track memory accesses only within a function of interest (and its descendants). For instance, if you determined by profiling your code that function foo() slows down when run with multiple threads and you want to see whether there is some true or false sharing that is responsible for the slowdown, you can tell the memtracker to only track memory accesses in foo() and its descendants. 

To do so, you use the -f option to the pintool and provide the file name that has the names of the functions of interest. For example, suppose you put your problematic functions in the file funcs.in. Then you would invoke the tool as follows: 

```
pin.sh -t $CUSTOM_PINTOOLS_HOME/obj-intel64/memtracker.so -f funcs.in -- <your program with arguments>
```

By default, memtracker looks for the scope-limiting functions in the file memtracker.in (located in the working directory) even if you don't use the -f option. 

For an example of a valid configuration file, take a look at scripts/memtracker.in.


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


