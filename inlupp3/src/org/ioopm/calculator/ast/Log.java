package org.ioopm.calculator.ast;

public class Log extends Unary{
    
    public Log(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override 
    public String getName() {
        return "log";
    }
}
