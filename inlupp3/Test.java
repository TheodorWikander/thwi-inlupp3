import org.ioopm.calculator.ast.*;
import org.ioopm.calculator.parser.*;

import java.io.IOException;

public class Test {

    public static void testPrinting(String expected, SymbolicExpression e) {
        String actual = "" + e;
        if (expected.equals(actual)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
        assert expected.equals(actual) : "Expected '" + expected + "' but got '" + actual + "'";
    }

    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment vars) {
        SymbolicExpression r = e.eval(vars);
        if (r.equals(expected)) {
            System.out.println("Passed: " + e);
        } else {
            System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
        }
    }

    public static void testIllegalAssignment(SymbolicExpression e, Environment vars) {
        try {
            SymbolicExpression r = e.eval(vars);
            System.out.println("Error: expected exception for '" + e + "' but got '" + r + "'");
        } catch (IllegalAssignmentException ex) {
            System.out.println("Passed: " + e + " threw: " + ex.getMessage());
        }
    }

    public static void testParseFails(String input, CalculatorParser parser) throws IOException {
        try {
            SymbolicExpression r = parser.parse(input, new Environment());
            System.out.println("Error: expected syntax error for '" + input + "' but got '" + r + "'");
        } catch (SyntaxErrorException ex) {
            System.out.println("Passed: '" + input + "' gave: " + ex.getMessage());
        }
    }

    public static void main(String[] args) throws IOException {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Constant c3 = new Constant(42);
        Variable vX = new Variable("x");
        Variable vY = new Variable("y");

        // ---- Printing ----
        testPrinting("42.0", c3);
        testPrinting("x", vX);

        Addition a = new Addition(c1, vX);
        Multiplication m = new Multiplication(a, c2);
        testPrinting("(5.0 + x) * 2.0", m);

        SymbolicExpression noParens = new Addition(c1, new Multiplication(vX, c2));
        testPrinting("5.0 + x * 2.0", noParens);

        SymbolicExpression s = new Subtraction(vX, c1);
        SymbolicExpression d = new Division(s, vY);
        testPrinting("(x - 5.0) / y", d);

        SymbolicExpression nestedsub = new Subtraction(vX, new Subtraction(c1, c2));
        testPrinting("x - (5.0 - 2.0)", nestedsub);

        testPrinting("sin(x)", new Sin(vX));
        testPrinting("cos(x + 5.0)", new Cos(new Addition(vX, c1)));
        testPrinting("log(42.0)", new Log(c3));
        testPrinting("-(x + 2.0)", new Negation(new Addition(vX, c2)));

        SymbolicExpression assign = new Assignment(new Addition(c1, c2), vX);
        testPrinting("5.0 + 2.0 = x", assign);

        SymbolicExpression complex = new Multiplication(
            new Sin(new Multiplication(c2, vX)),
            new Negation(new Division(vY, c3)));
        testPrinting("sin(2.0 * x) * -(y / 42.0)", complex);

        // ---- Evaluating ----
        testEvaluating(new Constant(42),
                       new Addition(new Constant(5), new Constant(37)),
                       new Environment());

        testEvaluating(new Addition(new Constant(37), new Variable("x")),
                       new Addition(new Constant(37), new Variable("x")),
                       new Environment());

        Environment vars = new Environment();
        vars.put(new Variable("x"), new Constant(8));
        testEvaluating(new Constant(26),
                       new Multiplication(new Addition(new Constant(5), new Variable("x")),
                                          new Constant(2)),
                       vars);

        Environment vars1 = new Environment();
        testEvaluating(new Constant(3),
                       new Assignment(new Constant(3), new Variable("x")),
                       vars1);
        if (new Constant(3).equals(vars1.get(new Variable("x")))) {
            System.out.println("Passed: x stored in environment");
        } else {
            System.out.println("Error: x not stored correctly");
        }

        SymbolicExpression chain = new Assignment(
            new Assignment(new Constant(2), new Variable("x")),
            new Variable("y"));
        testEvaluating(new Constant(2), chain, new Environment());

        // Values only evaluated once
        Environment env = new Environment();
        testEvaluating(new Addition(new Variable("z"), new Constant(2)),
                       new Assignment(new Addition(new Variable("z"), new Constant(2)), new Variable("y")),
                       env);
        testEvaluating(new Constant(3),
                       new Assignment(new Constant(3), new Variable("z")),
                       env);
        testEvaluating(new Addition(new Variable("z"), new Constant(2)),
                       new Variable("y"),
                       env);

        testEvaluating(new Variable("b"), new Variable("b"), new Environment());

        SymbolicExpression c = new Addition(new Constant(5), new Constant(37));
        testEvaluating(new Constant(42), c, new Environment());

        SymbolicExpression e = new Subtraction(new Constant(10), new Constant(5));
        testEvaluating(new Constant(5), e, new Environment());

        SymbolicExpression f = new Multiplication(new Constant(10), new Constant(5));
        testEvaluating(new Constant(50), f, new Environment());

        SymbolicExpression g = new Division(new Constant(10), new Constant(5));
        testEvaluating(new Constant(2), g, new Environment());

        testEvaluating(new Constant(5), c1, new Environment());
        testEvaluating(new Variable("x"), vX, new Environment());

        // ---- Named constants ----
        NamedConstant pi = new NamedConstant("pi", Math.PI);
        NamedConstant answer = new NamedConstant("Answer", 42);

        testPrinting("pi", pi);
        testPrinting("Answer + x", new Addition(answer, vX));

        testEvaluating(new Constant(42), answer, new Environment());
        testEvaluating(new Constant(Math.PI), pi, new Environment());
        testEvaluating(new Constant(84), new Addition(answer, answer), new Environment());
        testEvaluating(new Addition(new Constant(42), vX),
                       new Addition(answer, vX),
                       new Environment());

        // Otillåten omtilldelning
        testIllegalAssignment(new Assignment(new Constant(43), answer), new Environment());
        testIllegalAssignment(new Assignment(new Constant(3), pi), new Environment());

        // Miljön orörd efter misslyckad tilldelning
        Environment envFail = new Environment();
        testIllegalAssignment(new Assignment(new Constant(43), answer), envFail);
        if (envFail.isEmpty()) {
            System.out.println("Passed: environment unchanged after illegal assignment");
        } else {
            System.out.println("Error: environment was modified: " + envFail);
        }

        // Answer = x
        Environment envA = new Environment();
        testEvaluating(new Constant(42), new Assignment(answer, vX), envA);
        if (new Constant(42).equals(envA.get(new Variable("x")))) {
            System.out.println("Passed: x = 42 stored");
        } else {
            System.out.println("Error: x not stored");
        }

        // ---- Via parsern ----
        CalculatorParser parser = new CalculatorParser();

        testPrinting("Answer + x", parser.parse("Answer + x", new Environment()));
        testEvaluating(new Constant(84), parser.parse("Answer + Answer", new Environment()), new Environment());
        testIllegalAssignment(parser.parse("43 = Answer", new Environment()), new Environment());
        testIllegalAssignment(parser.parse("3 = pi", new Environment()), new Environment());

        testEvaluating(new Variable("answer"), parser.parse("answer", new Environment()), new Environment());

        testParseFails("3 + ", parser);
        testParseFails("(3 + 4", parser);
        testParseFails("3 = 4", parser);

        if (parser.parse("quit", new Environment()).isCommand()) {
            System.out.println("Passed: quit is a command");
        } else {
            System.out.println("Error: quit should be a command");
        }
    }
}


