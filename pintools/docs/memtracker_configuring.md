#### Configuring:

There are two required configuration files that memtracker accepts:

1. Configuration of memory allocation functions (see scripts/alloc.in for an example)
2. Configuration of the scope of tracking memory accesses (see scripts/memtracker.in for an example)

##### Configuration of memory allocation functions

You need to describe to memtracker the prototypes of the memory allocation functions used in your program. Why? Memtracker will need to know the address of the allocated variable, the size of the allocation and the number of allocated items (for calloc-like functions). It takes this information by reading the arguments and return values of the memory allocation function. To interpret these values correctly, it needs to know: is the allocated address returned from the function call or is it written to a memory location pointed to by a pointer argument and, if so, which argument? which argument specifies the size of the allocation? 

By default, memtracker looks for allocation function prototypes in the file alloc.in in the current working directory. Alternatively, you can provide your own file name with the -a option to the pintool as follows:

```
pin.sh -t $CUSTOM_PINTOOLS_HOME/obj-intel64/memtracker.so -a my_alloc_prototypes.in -- <your program with arguments>
```

Memtracker expects to find the prototypes of the allocation functions in the following format (one function per line):

```
<func_name>	   <arg_id_of_number> <arg_id_of_size> <arg_id_of_addr>
```

where:

| Token name | Description |
|------------|-------------|
| func_name> | the function name |
| arg_id_of_number | argument id of the number of allocated items or -1 if your alloc function does not use such an argument |
| arg_id_of_size | argument id of the size of the allocation |
| arg_id_of_addr | argument id of the pointer to the location where the allocated address will be stored or -1 if the address is returned by the function. |

arg_id is the index of the corresponding argument passed into the allocation function. We assume that the very first argument has id 0.

For example, consider the following valid alloc.in file:

```
# func                number   size   addr
#
malloc                  -1       0    -1
__wt_calloc              1       2     3 
```

Here we see two function signatures: the conventional malloc from libc and an application-specific allocation function __wt_calloc. Let's walk through these signatures to understand their specification. Malloc does not use the number of allocations in its arguments, so we put "-1" in the <arg_id_of_number> field (the first value after the function name). The size of the allocation is the first argument to malloc, so we put "0" into the <arg_id_of_size> field. The address of the allocation is returned by malloc, so we put "-1" into the <arg_id_of_addr>.

__wt_calloc, on the other hand, expects to receive the number of allocated items as the second argument, so we put "1" into the <arg_id_of_number> field. The size of each item is provided in the third argument so we put "2" into the <arg_id_of_size>. The allocated address is placed into a memory location pointed to by the fourth argument, so we put "3" into the <arg_id_of_addr> field. 

** What if your allocation functions are wrapped by a macro? 

Real-world code sometimes wraps memory-allocation functions into a macro. That would present a problem for memtracker when it attempts to identify the name of the variable for which the function allocates space. Memtracker parses the source file where the allocation function is called, but if it is wrapped in the macro, it will not find it there, because it will be replaced with the name of the macro. To fix this problem, we ask the user to provide the names of the macros (if any) that might be used as wrappers for memory allocation functions. Suppose that __wt_calloc function from the above example can be wrapped in one of three macros. Then, we would specify their names as follows:

```
# func                number   size   addr
#
malloc                  -1       0    -1
__wt_calloc              1       2     3 
!__wt_calloc_def         1      -1     2
!__wt_block_size_alloc  -1      -1     1 
!__bit_alloc            -1      -1     2
```

You see three additional records here corresponding to the __wt_calloc-wrapping macros. They appear directly under the signature for __wt_calloc (this is required) and are prefixed with an "!". Other than that, they have the same format as the simple function-signature record, except the fields <arg_id_of_number> and <arg_id_of_size> are not used. We only care about <arg_id_of_addr>.