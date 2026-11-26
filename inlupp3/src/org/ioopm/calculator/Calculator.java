package org.ioopm.calculator;

import java.io.IOException;
import java.io.BufferedReader;
import java.io.InputStreamReader;

import org.ioopm.calculator.ast.Clear;
import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.SyntaxErrorException;
import org.ioopm.calculator.ast.Variable;
import org.ioopm.calculator.ast.Vars;
import org.ioopm.calculator.parser.CalculatorParser;
import org.ioopm.calculator.ast.IllegalAssignmentException;
import org.ioopm.calculator.ast.IllegalExpressionException;
import org.ioopm.calculator.ast.Quit;



public class Calculator {
    static boolean run = true;

    public static void main(String [] args) {
        final CalculatorParser parser = new CalculatorParser();
        final Environment e = new Environment();

        while(run) {
            String input = System.console().readLine();
            try {
                SymbolicExpression currentOp = parser.parse(input, e);
                
                if(currentOp instanceof Clear) {
                        e.clear();
                } else if (currentOp instanceof Vars) {
                    System.out.println(e);
                } else if (currentOp instanceof Quit) {
                    run = false;
                } else {
                    SymbolicExpression result = currentOp.eval(e);
                    e.put(new Variable("ans"), result);
                    System.out.println(result);
                }

            } catch (IOException ex) {
                System.out.println("Error: could not read input");
            } catch (SyntaxErrorException | IllegalExpressionException | IllegalAssignmentException ex) {
                System.out.println(ex.getMessage());
            }
        }

    }
}
