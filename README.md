# Task 2 - Linux Practice
Task 2 consists of three parts: file operations, dynamic libraries, and a basic shell. This task can be completed in pairs (not triples) and involves implementing various programs and libraries to simulate CMD-like tools and a custom shell program.

## Part A - File Operations
In this part, we need to implement two small programs that function like regular CMD tools.

#### Tool 1: "cmp"

The "cmp" tool compares two files and returns "0" if they are equal, and "1" if they are distinct. It supports the "-v" flag for verbose output, which prints "equal" or "distinct" in addition to the return value. It also supports the "-i" flag, which means "ignore case" and treats "AAA" and "aaa" as equal.

#### Usage:

```
cmp <file1> <file2> -v 
```
Output: ` equal `

#### Tool 2: "copy"

The "copy" tool copies a file to another location and/or renames it. It returns "0" on success and "1" on failure. If the target file already exists, it outputs "target file exists" when the "-v" flag is used, and returns "1". It also supports the "-f" flag, which allows overwriting of the target file.

Usage:

```
copy <file1> <file2> -v
```
#### Output:
`success`

Note: If the required parameters (file names) are missing, the tool returns "1" and provides a usage explanation along with optional flag information.

## Part B - Dynamic Libraries
In this part, we need to implement a coding library with two different methods: "codecA" and "codecB".

#### Method a: codecA

This method converts all lowercase characters to uppercase and all uppercase characters to lowercase. All other characters remain unchanged.

#### Method b: codecB

This method converts all characters to the character that comes three positions after them in the ASCII table, by adding 3 to their ASCII value.

The libraries should support "encode" and "decode" methods and should be reversible, meaning that if one uses the "encode" method followed by the "decode" method, they should get the original string.

## Part C - Basic Shell (stshell)
In this part, we need to implement a custom shell program named "stshell" (short for "students"). The shell should have the following features:

Ability to run CMD tools that exist on the system using the "fork + exec + wait" mechanism. This allows the shell to spawn child processes and execute external commands.

Ability to stop a running tool by pressing Ctrl+c, without killing the shell itself. This is achieved by implementing a signal handler that captures the Ctrl+c signal and terminates the child process without affecting the shell.

Ability to redirect output using the ">" and ">>" operators for file redirection, and support for piping using the "|" operator for at least two consecutive pipes. This allows the shell to handle standard input, standard output, and standard error streams of commands, as well as chaining commands together through pipes.

Ability to stop itself by using the "exit" command, which terminates the shell and ends the program execution.

#### Usage:
Compile the "stshell" program using the provided source code and makefile.
```gcc -o stshell stshell.c```
Run the "stshell" program in a Linux environment.
```./stshell```

??? If you want the commands to run in the "stshell" prompt to execute CMD tools or perform other operations. The shell will interpret and execute the commands accordingly.
`Example commands:`

Running a CMD tool:
```
ls

cat Makefile 

pressing Crtl+C
```
file:///home/danielle/Pictures/Screenshots/Screenshot%20from%202023-04-22%2014-58-56.png
file:///home/danielle/Pictures/Screenshots/Screenshot%20from%202023-04-22%2014-59-50.png
Piping commands using "|":
```
cat Makefile | grep: | sort

./decode codecB "zh#zrq" | ./encode codecA  
```
file:///home/danielle/Pictures/Screenshots/Screenshot%20from%202023-04-22%2015-00-40.png
file:///home/danielle/Pictures/Screenshots/Screenshot%20from%202023-04-22%2015-01-21.png
Exiting the "stshell":
```
exit   // Terminates the shell and ends the program execution
```
