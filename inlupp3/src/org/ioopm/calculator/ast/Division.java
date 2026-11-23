package org.ioopm.calculator.ast;

public class Division extends Binary{
    public Division(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);   // anropar Binary(lhs, rhs)
    }


    @Override
    public String getName() {
        return "/";
    }

    public boolean equals(Division other) {
        return this.sameOperands(other);
    }
}
