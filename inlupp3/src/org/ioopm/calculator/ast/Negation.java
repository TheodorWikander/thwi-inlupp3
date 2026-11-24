package org.ioopm.calculator.ast;

public class Negation extends Unary{



    @Override
    public SymbolicExpression eval(Environment vars) {
    SymbolicExpression arg = this.getArgument().eval(vars);
    if (arg.isConstant()) {
        return new Constant(Math.negateExact((int)arg.getValue()));
    } else {
        return new Negation(arg);
    }
    }  
    
    public Negation(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "-";
    }


    public boolean equals(Negation other) {
        return this.sameOperands(other);
    }

    //Kanske sätta en priority för just den här, om det är typ -10 * -10.
}
