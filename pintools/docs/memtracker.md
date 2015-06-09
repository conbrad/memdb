## memtracker.so 

This tool tracks memory allocations, memory accesses and function entry/exit points for C programs and the associated libraries. It prints out a trace of allocation and access records. 

The tool has some nice features enabling it to work with real production code. 

* It does not require changes to the source code of your program. 
* It can track any memory allocation function or macro (not just malloc). You specify the prototype of your memory allocation functions in a configuration file. 
* It works for multithreaded programs. 
* It tracks source location of memory accesses (for binaries with debug information) and the names of the accessed variables (for variables allocated on the heap via the tracked allocation functions you identified). 

### HOW TO USE THE TOOL:
	1. [Building](docs/memtracker_building.md)
	2. [Configuring](docs/memtracker_configuring.md)
	3. [Running](docs/memtracker_running.md)
	4. [Eclipse set up instructions](docs/memtracker_eclipse.md)
