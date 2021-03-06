## memtracker2json.py

This script converts the raw trace generated by memtracker to JSON format. JSON format is needed to analyze the memory access pattern and visualize them using our visualization tools. 

Here are the options for running memtracker2json.py:

| Option name | Description |
|-------------|-------------|
|  -h, --help | show this help message and exit |
|  --infile INFILE | Name of the trace file generated by the memtracker pintool.|
|  --keepdots | Do not skip records from .text and .plt when generating trace |


The most common way is to run it like this:

```
./memtracker2json.py --infile <memtracker-raw-trace> > <output-JSON-trace>
```

Alternatively, the script can take the data from stdin:

```
cat memtracker.log | memtracker2json.py > trace.json
```

This is useful if you want to run the memtracker and memtracker2json concurrently. For an example, see scripts/memtracker+m2j.sh.