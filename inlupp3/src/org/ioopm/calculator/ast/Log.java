package org.ioopm.calculator.ast;

public class Log extends Unary{


    
    public Log(SymbolicExpression identifier) {
        super(identifier);
    }

    @Override
    public String getName() {
        return "log";
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
    SymbolicExpression arg = this.getArgument().eval(vars);
    if (arg.isConstant()) {
        return new Constant(Math.log(arg.getValue()));
    } else {
        return new Log(arg);
    }
    }  
}
