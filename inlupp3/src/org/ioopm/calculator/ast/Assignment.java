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

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.getLeft().eval(vars);
        SymbolicExpression rhs = this.getRight();

        if (rhs instanceof Variable) {
            vars.put((Variable) rhs, lhs);
            return lhs;
        } else {
            throw new RuntimeException("Right hand side of assignment must be a variable");
        }
    }
}
