CC=gcc

%.o: %.c
		$(CC) -c -fpic -o $@ $< $(CFLAGS)

memtracker: memtracker_instrumentation.o bitcode
	$(CC) -shared -o libinstrumentation.so $<

text: textinstrumentation.o bitcode
	$(CC) -shared -o libinstrumentation.so $<

binary: binaryinstrumentation.o bitcode
	$(CC) -shared -o libinstrumentation.so $<

bitcode: memtracker_instrumentation.c
	clang -emit-llvm $< -c -g -o instrumentation.bc

clean:
	rm *.o instrumentation.bc


