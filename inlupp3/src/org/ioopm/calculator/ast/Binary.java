package org.ioopm.calculator.ast;

public abstract class Binary extends SymbolicExpression {
    private SymbolicExpression lhs = null;
    private SymbolicExpression rhs = null;

    public Binary(SymbolicExpression lhs, SymbolicExpression rhs) {
        this.lhs = lhs;
        this.rhs = rhs;
    }

    @Override 
    public boolean equals(Object other) {
        if(other instanceof Binary) {
            Binary tmp = (Binary) other;
            return this.getName().equals(tmp.getName()) && this.sameOperands(tmp);
        } else {
            return false;
        }
    }

    protected boolean sameOperands(Binary other) {
        return this.lhs.equals(other.lhs) && this.rhs.equals(other.rhs);
    }

    public SymbolicExpression getLeft() {
        return this.lhs;
    }

    public SymbolicExpression getRight() {
        return this.rhs;
    }

    @Override 
    public String toString() {
        String s = "";
        if (this.lhs.getPriority() < this.getPriority()) {
            s = "(" + this.lhs.toString() + ")";
        } else {
            s = this.lhs.toString();
        }

        s = s + " " + this.getName() + " ";

        if (this.rhs.getPriority() < this.getPriority()) {
            s = s + "(" + this.rhs.toString() + ")";
        } else {
            s = s + this.rhs.toString();
        }

        return s.toString();
    }

}
