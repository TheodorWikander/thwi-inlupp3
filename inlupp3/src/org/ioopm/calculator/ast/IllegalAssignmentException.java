package org.ioopm.calculator.ast;

//kunna förklara vilken klass vi extendar och VARFÖR
public class IllegalAssignmentException extends RuntimeException {
    
    public IllegalAssignmentException(String message) {
        super(message);
    }
}
