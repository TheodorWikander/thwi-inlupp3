package org.ioopm.calculator.ast;

public class NamedConstants extends Atom{

    private final String name;
    private final Double value;

    public NamedConstants(String name, double value) {
        this.name = name;
        this.value = value;
    }

    @Override
    public boolean isConstant() { return true; }

    @Override
    public SymbolicExpression eval(Environment vars) {
        return new Constant(Constants.namedConstants.get(name));
    }

    @Override
    public String toString() { return name; }
}
