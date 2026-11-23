package org.ioopm.calculator.ast;

public class Log extends Unary{


    
    public Log(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "log";
    }


    public boolean equals(Log other) {
        return this.sameOperands(other);
    }

    @Override
    public SymbolicExpression eval() {
    SymbolicExpression arg = this.getArgument().eval();
    if (arg.isConstant()) {
        return new Constant(Math.log(arg.getValue()));
    } else {
        return new Log(arg);
    }
    }  
}
