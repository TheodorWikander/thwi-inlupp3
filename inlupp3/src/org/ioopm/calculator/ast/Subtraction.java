package org.ioopm.calculator.ast;

public class Subtraction extends Binary{
    public Subtraction(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);   
    }


    @Override
    public String getName() {
        return "-";
    }

    @Override
    public int getPriority() {
        return 50;
    }

    public boolean equals(Subtraction other) {
        return this.sameOperands(other);
    }
}
