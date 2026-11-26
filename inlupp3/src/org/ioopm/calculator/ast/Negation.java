package org.ioopm.calculator.ast;

public class Negation extends Unary{
    
    public Negation(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    public String getName() {
        return "negation";
    }
}
