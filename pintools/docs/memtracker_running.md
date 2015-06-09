#### Running:

##### Pre-requisites:

If you want memtracker to be able to know where in your code true or false sharing occurs as well as the names and types of the shared variables, compile your code with debug symbols. Run your program (under the pintool) on the same system where you compiled your program and don't remove or move the source files. (If you are running on a different system, make sure that the source files are in the same absolute path as they were on the system where the program or the library was compiled).

**Read about [configuring](docs/memtracker_configuring) the tool (below) before running it!**

To run:

```
pin.sh -t $CUSTOM_PINTOOLS_HOME/obj-intel64/memtracker.so -- <your program with arguments>
```

For an example of the actual working script that launches this tool with a WiredTiger library running the LevelDB benchmark, take a look at scripts/memtracker.sh. 

##### Command-line options:

| Option name | Description |
|-------------|-------------|
|  -a [file]  | The file containing the signatures of alloc functions and macros (see below for format). Default: alloc.in. |
|  -f [file]  | The file configuring the scope of tracking (see below for format). Default: memtracker.in |
|  -p [32|64] | Application pointer size. Default: 64.|
|  -s         | Output stack addresses into the trace. Default: no. |