
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


    public static void testEvaluating(SymbolicExpression expected, SymbolicExpression e, Environment vars) {
    SymbolicExpression r = e.eval(vars);
    if (r.equals(expected)) {
        System.out.println("Passed: " + e);
    } else {
        System.out.println("Error: expected '" + expected + "' but got '" + r + "'");
        }
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

    //Values uónly evaluated once:
        Environment env = new Environment();

        // z + 2 = y --> y = z + 2)
        testEvaluating(new Addition(new Variable("z"), new Constant(2)),
            new Assignment(new Addition(new Variable("z"), new Constant(2)), new Variable("y")),
            env);

        // 3 = z
        testEvaluating(new Constant(3),
            new Assignment(new Constant(3), new Variable("z")),
            env);

        // y ska fortfarande vara z + 2, inte 5
        testEvaluating(new Addition(new Variable("z"), new Constant(2)),
            new Variable("y"),
            env);

        testEvaluating(new Variable("b"), new Variable("b"), new Environment());

        SymbolicExpression c = new Addition(new Constant(5), new Constant(37));
        testEvaluating(new Constant(42), c, new Environment()); /// Tests if reducing a returns b (it should!)

        //fler tester skulle nog behövas
        SymbolicExpression e = new Subtraction(new Constant(10), new Constant(5));
        testEvaluating(new Constant(5), e, new Environment());

        SymbolicExpression f = new Multiplication(new Constant(10), new Constant(5));
        testEvaluating(new Constant(50), f, new Environment());

        SymbolicExpression g = new Division(new Constant(10), new Constant(5));
        testEvaluating(new Constant(2), g, new Environment());

        testEvaluating(new Constant(5), c1, new Environment());

        testEvaluating(new Variable("x"), vX, new Environment());


    }

}


