package org.ioopm.calculator;

import java.io.IOException;

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
        final Environment env = new Environment();

        //for statistics over entered commands
        int entered = 0;
        int sucesfullyEvaluated = 0; 
        int fullyEvaluated = 0;

        System.out.println("Welcome to the Symbolic Calculator!");

        while(run) {
            System.out.print("\n");
            System.out.print("? ");
            String input = System.console().readLine();

            Environment backup = new Environment();
            backup.putAll(env);

            try {
                SymbolicExpression currentOp = parser.parse(input, env);
                
                if(currentOp instanceof Clear) {
                    env.clear();
                } else if (currentOp instanceof Vars) {
                    System.out.println(env.size() + " " + env);
                } else if (currentOp instanceof Quit) {
                    run = false;
                } else {
                    entered++;
                    SymbolicExpression result = currentOp.eval(env);
                    sucesfullyEvaluated++;
                    if (result.isConstant()) {
                        fullyEvaluated++;
                    }
                    env.put(new Variable("ans"), result);
                    System.out.println(result);
                }

            } catch (IOException ex) {
                restore(env,backup);
                System.out.println("Error: could not read input");
            } catch (SyntaxErrorException | IllegalExpressionException | IllegalAssignmentException ex) {
                restore(env, backup);
                System.out.println(ex.getMessage());
            } catch (RuntimeException ex) {
                restore(env, backup);
                System.out.println("Error: " + ex.getMessage());
            }
        }
        System.out.println("Goodbye!");
        System.out.println("Expressions entered: " + entered);
        System.out.println("Sucesfully evaluated: " + sucesfullyEvaluated);
        System.out.println("Fully evaluated: " + fullyEvaluated);


    }
    private static void restore(Environment env, Environment backup) {
        env.clear();
        env.putAll(backup);
    }
}
