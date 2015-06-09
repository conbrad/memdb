
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


