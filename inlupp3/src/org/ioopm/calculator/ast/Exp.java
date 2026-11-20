package org.ioopm.calculator.ast;

public class Exp extends Unary{
    
    public Exp(SymbolicExpression freakyMath) {
        super(freakyMath);
    }


    @Override
    public String getName() {
        return "exp";
    }

    @Override
    public int getPriority() {
        return 150;
    }
}
