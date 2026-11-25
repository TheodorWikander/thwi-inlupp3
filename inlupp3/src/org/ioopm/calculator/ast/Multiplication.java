package org.ioopm.calculator.ast;

import javax.swing.plaf.multi.MultiButtonUI;

public class Multiplication extends Binary{
    public Multiplication(SymbolicExpression lhs, SymbolicExpression rhs) {
        super(lhs, rhs);   
    }


    @Override
    public String getName() {
        return "*";
    }

     @Override 
    public int getPriority() {
        return 75;
    }

    public boolean equals(Multiplication other) {
        return this.sameOperands(other);
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
        SymbolicExpression lhs = this.getLeft().eval(vars);
        SymbolicExpression rhs = this.getRight().eval(vars);
        if(lhs.isConstant() && rhs.isConstant()) {
           return new Constant(lhs.getValue() * rhs.getValue());
        }  else {
            return new Multiplication(lhs, rhs);
        }
    }
}
