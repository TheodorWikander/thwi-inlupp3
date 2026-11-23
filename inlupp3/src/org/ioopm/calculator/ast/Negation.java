package org.ioopm.calculator.ast;

public class Negation extends Unary{
    
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
