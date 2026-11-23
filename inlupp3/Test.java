import org.ioopm.calculator.ast.*;

public class Test{

    public static void testPrinting(String expected, SymbolicExpression e) {
        String actual = "" + e;
        if (expected.equals("" + e)) {
        System.out.println("Passed: " + e);
    } else {
        System.out.println("Error: expected '" + expected + "' but got '" + e + "'");
        }
        assert expected.equals(actual) : "Expected '" + expected + "'but got '" + actual + "'";
    }

    public static void main(String[] args) {
        Constant c1 = new Constant(5);
        Constant c2 = new Constant(2);
        Constant c3 = new Constant(42);
        Variable vX = new Variable("x");
        Variable vY = new Variable("y");

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

        SymbolicExpression sinExpr = new Sin(vX);
        SymbolicExpression cosExpr = new Cos(new Addition(vX, c1));
        SymbolicExpression logExpr = new Log(c3);
        SymbolicExpression negExpr = new Negation(new Addition(vX, c2)); 

        testPrinting("sin(x)", sinExpr);
        testPrinting("cos(x + 5.0)", cosExpr);
        testPrinting("log(42.0)", logExpr);
        testPrinting("-(x + 2.0)", negExpr);

        SymbolicExpression assign = new Assignment(new Addition(c1, c2), vX);
        testPrinting("5.0 + 2.0 = x", assign);

        SymbolicExpression complex = new Multiplication(
            new Sin(new Multiplication(c2, vX)),
            new Negation(new Division(vY, c3))
        );
        testPrinting("sin(2.0 * x) * -(y / 42.0)", complex);

    }

}


