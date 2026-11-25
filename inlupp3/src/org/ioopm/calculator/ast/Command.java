package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression{
    private SymbolicExpression string = null;

    public Command() {
        
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        throw new RuntimeException("Commands cannot be evaluated!");
    }
}
