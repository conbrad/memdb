#### Configuring for Eclipse CDT

	1. 	Clone the repository
	2. 	In Eclipse, create a new Makefile project with Existing Code (File -> New -> Makefile Project with Existing Code)
	3. 	Name the project, and make the Existing Code Location point to the cloned repository
	4. 	Leave C and C++ Languages checked
	5. 	Select Linux GCC as the toolchain
	6. 	Click Finish
	7. 	Once the project is listed in the Projects perspective, right click the project and select Properties
	8. 	Go to C/C++ General -> Preprocessor Includes
	9. 	In the Entries tab, under Languages, select GNU C++
	10. Add CDT GCC Built-in Compiler Settings (this resolves common C library includes)
	11. Go to C/C++ General -> Paths and Symbols
	12. In the Entries tab, under Languages select GNU C++
	13. Click Add and point it to the folder where you extracted Pin (this resolves Pin specific includes)
	14. Click Apply


#### Configuring for Eclipse CDT
	1. Click `Window -> Show View -> Make Target`
	2. Select the project and click the green target with a plus
	3. For `Target Name` put Makefile
	4. Uncheck Same as target name and put `all`