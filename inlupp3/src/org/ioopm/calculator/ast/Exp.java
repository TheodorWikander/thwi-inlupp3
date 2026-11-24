package org.ioopm.calculator.ast;

public class Exp extends Unary{
    


    public Exp(SymbolicExpression freakyMath) {
        super(freakyMath);
    }


    @Override
    public String getName() {
        return "exp";
    }

    @Override
    public int getPriority() {
        return 150;
    }

    public boolean equals(Exp other) {
        return this.sameOperands(other);
    }

        @Override
    public SymbolicExpression eval() {
    SymbolicExpression arg = this.getArgument().eval();
    if (arg.isConstant()) {
        return new Constant(Math.exp(arg.getValue()));
    } else {
        return new Exp(arg);
    }
    }  

}
