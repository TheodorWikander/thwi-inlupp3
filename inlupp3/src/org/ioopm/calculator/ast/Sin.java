package org.ioopm.calculator.ast;

public class Sin extends Unary{
    

    public Sin(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "sin";
    }
}
