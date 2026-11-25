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

    @Override
    public int getPriority () {
    return 75;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.getLeft().eval(vars);
        SymbolicExpression rhs = this.getRight().eval(vars);
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() / rhs.getValue());
        }  else {
            return new Division(lhs, rhs);
        }
    }
}
