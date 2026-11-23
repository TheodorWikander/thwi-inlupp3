package org.ioopm.calculator.ast;

public class Sin extends Unary{


    public Sin(SymbolicExpression identifier) {
        super(identifier);
    }

    @Override
    public String getName() {
        return "sin";
    }


    public boolean equals(Sin other) {
        return this.sameOperands(other);
    }

    @Override
    public SymbolicExpression eval() {
    SymbolicExpression arg = this.getArgument().eval();
    if (arg.isConstant()) {
        return new Constant(Math.sin(arg.getValue()));
    } else {
        return new Sin(arg);
    }
    }   
}
