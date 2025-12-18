# What is this program?
This program takes input from either a file or std input and scans the input for valid tokens using those tokens it then Parses through the input making sure the program follows the programming langauges BNF. As it is validating that the input program matches the BNF. It will build a Parse Tree
After having done that it will then check the static semantics of the program specifically it will check whether a variable gets used in the program if not it will print out a warning.
Also if there is a static semantic error like using a variable that hasn't been defined than an error will be output and the program will exit
After running the static semantics it will look at each of the program files statements and turn them into the target language and output a file called [yourFile.asm] or [a.asm] you would then need to run these files into a virtual machine.
# How to run
in the directory the makefile is in enter make. This should compile the program.
The executables name is compile. Use ./compile and enter any arguments you'd like to make.
If you wish to pass your own file into the program be sure it has the .fs25s1 extension this is required if you intend for the program to use a file as it's input.
## Argument options
No arguments:
	Gives you space to write whatever you wish. You can even press enter to create a new line. When you're done press ctrl+d (sometimes you'll need to do it twice) it will use those words and the output file names will have the base name "a"
one argument:
	This program will only take in one argument and that is the basefile name not including the required .fs25s1 file extension the program will append that to the file name
# testing
A good way to test the No arguments option is to still use your input file with the .fs25s1 extension. Some thing like ./compile < P4test.fs25s1 this just redirects the input to the programs stdin
