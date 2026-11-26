all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

run:
	java -cp classes org.ioopm.calculator.Calculator

test: 
	javac -sourcepath src Test.java

runtest: 
	java -cp .:src Test

runtest-assert: 
	java -enableassertions -cp .:src Test
	
clean:
	find . -name "*.class" -type f -delete


 