package org.ioopm.calculator.ast;

public class Assignment extends Binary {
    
    public Assignment(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);
    }

    @Override
    public String getName() {
        return "=";
    }

    @Override 
    public int getPriority() {
        return 25;
    }


    public boolean equals(Assignment other) {
        return this.sameOperands(other);
    }
}
