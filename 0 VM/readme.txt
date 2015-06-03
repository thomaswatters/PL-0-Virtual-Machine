// INDIVIDUAL

// Name: Darian Smalley

PM\0 Virtual Machine

File: vm.c
Other required files: mcode.txt

GCC required

To compile:

Ensure mcode.txt is in the same directory as vm.c. 
mcode.txt should contain one instruction per line.
Each instruction consists of three integers: op code, lexicographical level and modifier, separated by spaces as shown.
	//op l m
	7 0 2

Navigate to the directory where vm.c and mcode.txt are located in the system’s terminal or console and enter the following commands to compile and execute the virtual machine:

gcc vm.c -o vm
./vm

A “|” in the stack represents a division between Activation Records


		