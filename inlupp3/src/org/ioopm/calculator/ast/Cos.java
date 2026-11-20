package org.ioopm.calculator.ast;

public class Cos extends Unary{
    
    public Cos(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "cos";
    }
}
