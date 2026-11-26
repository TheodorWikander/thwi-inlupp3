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

    @Override 
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.getLeft().eval(vars);
        SymbolicExpression rhs = this.getRight().eval(vars);
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() + rhs.getValue());
        }  else {
            return new Addition(lhs, rhs);
        }
    }

    
    
}

