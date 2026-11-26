package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression{

    @Override 
    public boolean isCommand() {
        return true;
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands cannot be evaluated!");
    }
}
