all:
	javac -d classes -sourcepath src src/org/ioopm/calculator/Calculator.java

run:
	java -cp classes org.ioopm.calculator.Calculator

comptest:
	javac -cp classes -d classes Test.java

runtest:
	java -cp classes Test

clean:
	rm -rf classes