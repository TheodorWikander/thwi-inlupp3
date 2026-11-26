package org.ioopm.calculator.ast;

public class Exp extends Unary{
    


    public Exp(SymbolicExpression identfier) {
        super(identfier);
    }


    @Override
    public String getName() {
        return "exp";
    }

    @Override
    public int getPriority() {
        return 150;
    }

    @Override
    public SymbolicExpression eval(Environment vars) {
    SymbolicExpression arg = this.getArgument().eval(vars);
    if (arg.isConstant()) {
        return new Constant(Math.exp(arg.getValue()));
    } else {
        return new Exp(arg);
    }
    }  

}
