package org.ioopm.calculator;

import java.io.IOException;
import org.ioopm.calculator.ast.Clear;

import org.ioopm.calculator.ast.Environment;
import org.ioopm.calculator.ast.SymbolicExpression;
import org.ioopm.calculator.ast.SyntaxErrorException;
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
                SymbolicExpression result = currentOp.eval(e);
                
                if(currentOp instanceof Clear) {
                        e.clear();
                } else if (currentOp instanceof Vars) {
                    System.out.println(e);
                } else if (currentOp instanceof Quit) {
                    run = false;
                } else {
                    System.out.println(currentOp.eval(e));
                }

            } catch (IOException ex) {
                System.out.println("Error: could not read input");
            } catch (SyntaxErrorException | IllegalExpressionException | IllegalAssignmentException ex) {
                System.out.println(ex.getMessage());
            }
        }

    }
}
