package org.ioopm.calculator.ast;

public class NamedConstant extends Atom {
    private final String name;

    public NamedConstant(String name) {
        this.name = name;
    }

    @Override 
    public boolean isConstant() {
        return true;
    }

    @Override 
    public SymbolicExpression eval(Environment vars) {
        return new Constant(Constants.namedConstants.get(name));
    }

    @Override 
    public String toString() {
        return name; 
    }




    
}
