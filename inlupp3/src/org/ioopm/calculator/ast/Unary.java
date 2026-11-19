package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression{
    private SymbolicExpression freakyMath = null;

    public Unary(SymbolicExpression freakyMath) {
        this.freakyMath = freakyMath;
    }
}
