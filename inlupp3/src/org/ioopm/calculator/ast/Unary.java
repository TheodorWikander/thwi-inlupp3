package org.ioopm.calculator.ast;

public abstract class Unary extends SymbolicExpression{
    
    private SymbolicExpression argument;

    public Unary(SymbolicExpression argument) {
       this.argument = argument;
    }

    public SymbolicExpression getArgument() {
        return this.argument;
    }

    public String toString() {
    /// Note how the call to toString() is not necessary
    return this.getName() + "(" + this.argument + ")";
    }

    protected boolean sameOperands(Unary other) {
        return this.argument.equals(other.argument);
    }

    @Override 
    public boolean equals(Object other) {
        if(other instanceof Unary) {
            Unary tmp = (Unary) other;
            return this.getName().equals(tmp.getName()) && this.sameOperands(tmp);
        } else {
            return false;
        }
    }
}
