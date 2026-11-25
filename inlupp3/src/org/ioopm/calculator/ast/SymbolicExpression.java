package org.ioopm.calculator.ast;


public abstract class SymbolicExpression {
    
    public boolean isConstant() {
        return false;
    }

    public String getName() {
        throw new RuntimeException("getName() called on expression with no operator");
    }

    public int getPriority() {
        return 100;
    }

    public double getValue() {
        throw new RuntimeException("getValue() did not return a constant");
    }
    
    public abstract SymbolicExpression eval(Environment vars);

    public boolean isCommand() {
    return false;
    }

    
}
