package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression{
    private SymbolicExpression freakyMath;
    private SymbolicExpression argument;

    public Unary(SymbolicExpression freakyMath) {
        this.freakyMath = freakyMath;
    }

    public String toString() {
    /// Note how the call to toString() is not necessary
    return this.getName() + " " + this.argument.toString();
}
}
