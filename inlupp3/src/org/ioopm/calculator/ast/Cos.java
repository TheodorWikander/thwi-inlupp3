package org.ioopm.calculator.ast;

public class Cos extends Unary{
    
    public Cos(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "cos";
    }

    public boolean equals(Cos other) {
        return this.sameOperands(other);
    }
}
