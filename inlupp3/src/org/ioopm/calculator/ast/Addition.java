package org.ioopm.calculator.ast;

public class Addition extends Binary{
    public Addition(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);   // anropar Binary(lhs, rhs)
    }

    @Override
    public String getName() {
        return "+";
    }

    @Override
    public int getPriority() {
        return 50;
    }

    public boolean equals(Addition other) {
        return this.sameOperands(other);
    }

    @Override 
    public SymbolicExpression eval() {
        SymbolicExpression lhs = this.getLeft().eval();
        SymbolicExpression rhs = this.getRight().eval();
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() + rhs.getValue());
        }  else {
            return new Addition(lhs, rhs);
        }
    }
    
}

