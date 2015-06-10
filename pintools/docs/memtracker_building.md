#### Pre-requisites to building:

The first pre-requisite for building the pintool is to have the Intel Pin toolkit installed on your machine. Download and unpack the pin toolkit from the Intel's website: 
http://software.intel.com/en-us/articles/pin-a-dynamic-binary-instrumentation-tool

**Note: the MemDB tool is optimized to run with Intel Pin version 2.13 revision 65163, correct work with newer versions of Pin is not guaranteed**

Set the PIN_ROOT environmental variable to point to the root  directory of the toolkit. 

The second prerequisite is to have libelf and libdwarf libraries installed. Those are required by the custom debug_info library used by the pintool to find the types of the allocated variables and the names of the fields within large structures. If you are using Ubuntu, you will be able to run the install.sh script from within memdb/pintools directory (see below) to have them installed automatically. Otherwise, follow the installation instruction for your particular Linux system. 

#### Building:

```
% git clone https://github.com/conbrad/memdb.git
% cd memdb/pintools
% make
```