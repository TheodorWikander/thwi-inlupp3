package org.ioopm.calculator.ast;

public class Variable extends Atom{
    private String identifier;


    @Override
    public boolean isConstant(){
        return true;
    }
}
