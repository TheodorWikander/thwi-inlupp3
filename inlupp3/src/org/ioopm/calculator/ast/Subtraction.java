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

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.getLeft().eval(vars);
        SymbolicExpression rhs = this.getRight().eval(vars);
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() - rhs.getValue());
        }  else {
            return new Subtraction(lhs, rhs);
        }
    }
}
