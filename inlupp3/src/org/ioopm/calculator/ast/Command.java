package org.ioopm.calculator.ast;

public abstract class Command extends SymbolicExpression{
    private SymbolicExpression string = null;

    public Command(SymbolicExpression string) {
        this.string = string;
    }
}
