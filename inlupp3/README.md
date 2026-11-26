# Symbolic Calculator (Inlupp 3)

# Nova Edlund 
# Theodor Wikander

# Build and run
make all                # build all files
make run                # run the program
make test               # build unit tests 
make runtest            # Run unit tests 
make runtest-assert:    # Run tests with enabled asserts  
make clean      # removes files built by makefile commands

# Usage
Prints one expression per line, for example "2*(2*3) or cos(pi) = x. Variable names are always written to th right of "=". 

Valid commands are "vars" (lists variables currenty in the program), "clear" (removes all varaibles), and "quit" (quits the program and show statistics from recent run). The value of the lastest expression is saved in "ans". Named constants built into the program are "pi", "e", "Answer" and "L" and theese annot be re-assigned. 

# Design 
- org.ioopm.calculator.ast: All AST classes. "SymbolicExpression" is the root class and "Binary", "Unary", "Atom", and "Command" are subclasses containing the "leaves". Exceptions classes are also found here. 

- org.ioopm.calculator.parser: CalculatorParser
- org.ioopm.calculator: Calculator (Main program containing the event loop that runs the program with user input)
- Test.java: All unit tests

# Decisions and known limitations

- Exceptions: "IllegalAssignmentException", "SyntaxErrorException", "IllegalExpressionException" all extend "RuntimeException". Theese are errors that can be caused by the user and are caught by the event loop. We  to extend RuntimeExceptions ... //TODO:

- Commands: "quit", "vars" and "clear" are singeltons. The event loop 



