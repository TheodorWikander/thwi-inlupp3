package org.ioopm.calculator.ast;

public class Cos extends Unary{

    
    public Cos(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "cos";
    }

    public boolean equals(Cos other) {
        return this.sameOperands(other);
    }

     @Override
    public SymbolicExpression eval() {
    SymbolicExpression arg = this.getArgument().eval();
    if (arg.isConstant()) {
        return new Constant(Math.cos(arg.getValue()));
    } else {
        return new Cos(arg);
    }
}


}
