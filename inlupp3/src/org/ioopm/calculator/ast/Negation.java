package org.ioopm.calculator.ast;

public class Negation extends Unary{
    
    public Negation(SymbolicExpression freakyMath) {
        super(freakyMath);
    }

    @Override
    public String getName() {
        return "negation";
    }

    //Kanske sätta en priority för just den här, om det är typ -10 * -10.
}
