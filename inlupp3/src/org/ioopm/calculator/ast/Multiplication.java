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

    public boolean equals(Multiplication other) {
        return this.sameOperands(other);
    }
}
