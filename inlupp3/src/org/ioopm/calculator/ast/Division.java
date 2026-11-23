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
    public SymbolicExpression eval() {
        SymbolicExpression lhs = this.getLeft().eval();
        SymbolicExpression rhs = this.getRight().eval();
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() / rhs.getValue());
        }  else {
            return new Division(lhs, rhs);
        }
    }
}
