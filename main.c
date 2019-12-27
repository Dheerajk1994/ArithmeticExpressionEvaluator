#include <stdio.h>
#include <stdlib.h>
#include "customStack.h"

#define MAX_EXPRESSION_LENGTH 50
#define MAX_EXPRESSION_LENGTH_S "50"
#define MAX_LITERAL_LENGTH 10

int getPrecedence(char c){
  if(c == '('){
    return 0;
  }
  else if(c == '+' || c == '-'){
    return 1;
  }
  else {
    return 2;
  }
}

bool isOperand(char* expressionIterator){
  return(
	 *expressionIterator == '*' ||
	 *expressionIterator == '/' ||
	 *expressionIterator == '+' ||
	 *expressionIterator == '-' ||
	 *expressionIterator == '^'
	 );
}

int binaryOperateToInt(char* operator, int operand1, int operand2){
  //TODO: prone to error
  switch(*operator){
    case '+':
      return  operand1 + operand2;
    case '-':
      return  operand1 - operand2;
    case'*':
      return  operand1 * operand2;
    case'/':
      return  operand1 / operand2;
  default:
    printf("error in binaryoperatetoint unknown operator %s\n", operator);
  }
}
void solveExpression(ExprStack* outPutStack,int expressionStringLength, int maxLiteralLength){
  //REVERSE THE ORDER OF THE OUTPUT STACK
  ExprStack* postFixStack = newExprStack(expressionStringLength, maxLiteralLength);
  char* top;
  while(exprStackIsEmpty(outPutStack) == false){
    top = peekExprStack(outPutStack);
    pushExprStack(postFixStack, top);
    popExprStack(outPutStack);
  }

  //WHILE THERE ARE ITEMS IN POSTFIX STACK - GO THROUGH EACH ITEM 
  //IF THE ITEM IS AN OPERAND THEN ADD THE ITEM TO THE SOLUTIONS STACK
  //IF THE ITEM IS AN OPERATOR THEN CALCULATE THE RESULT WITH THE OPERAND AND TOP TWO
  //OPERANDS IN THE SOLUTIONS STACK - PUSH THE RESULT BACK ON TOP OF THE SOLUTIONS STACK
  ExprStack* solutionStack = newExprStack(expressionStringLength, maxLiteralLength);
  char* intString = NULL;
  char* end = NULL;
  int operand1 = 0, operand2 = 0, result = 0;
  printExprStack(postFixStack);
  while(exprStackIsEmpty(postFixStack) == false){
    top = peekExprStack(postFixStack);
    popExprStack(postFixStack);
    if(isOperand(top) == false){
      pushExprStack(solutionStack, top);
    }
    else{
      //operand2
      operand2 = strtol(peekExprStack(solutionStack), &end, 10);
      popExprStack(solutionStack);
      //operand1
      operand1 = strtol(peekExprStack(solutionStack), &end, 10);
      popExprStack(solutionStack);

      result = binaryOperateToInt(top, operand1, operand2);

      intString = malloc(sizeof(char) * 10);
      sprintf(intString, "%d", result);
      pushExprStack(solutionStack, intString);
    }
  }
  printf("Solution: %s\n", peekExprStack(solutionStack));
}

void evaluateExpression(char* expressionString, int expressionStringLength, int maxLiteralLength){
  ExprStack* symbolStack = newExprStack(expressionStringLength, 1);
  ExprStack* outPutStack = newExprStack(expressionStringLength, maxLiteralLength);
  char* expressionIterator = expressionString;
  char* literal = malloc(sizeof(char) * MAX_LITERAL_LENGTH);
  int literalIndex = 0;
  while(*expressionIterator != '\0'){
    if(*expressionIterator == ' '){
      //skip spaces
    }
    else if(*expressionIterator == '('){
      *literal = *expressionIterator;
      pushExprStack(symbolStack, literal);
      literal = malloc(sizeof(char) * maxLiteralLength);
    }
    else if(*expressionIterator == ')'){
      if(literalIndex > 0){
	pushExprStack(outPutStack, literal);
	literalIndex = 0;
      }
      literal = malloc(sizeof(char) );
      char* topChar = peekExprStack(symbolStack);
      while(exprStackIsEmpty(symbolStack) == false && *topChar != '('){
	pushExprStack(outPutStack, topChar);
	popExprStack(symbolStack);
	topChar = peekExprStack(symbolStack);
      }
      popExprStack(symbolStack);
    }
    else if(isOperand(expressionIterator) == true){
      //push anything that was in the literal to the output stack
      //must be numbers
      if(literalIndex > 0){
	pushExprStack(outPutStack, literal);
	literalIndex = 0;
	literal = malloc(sizeof(char));
      }
      *literal = *expressionIterator;
      if(exprStackIsEmpty(symbolStack) == true){
	pushExprStack(symbolStack, literal);
      }
      else{
	char* topChar = peekExprStack(symbolStack);
	if(getPrecedence(*topChar) >= getPrecedence(*literal)){
	  popExprStack(symbolStack);
	  pushExprStack(outPutStack, topChar);
	}
	pushExprStack(symbolStack, literal);
      }
      literal = malloc(sizeof(char) * maxLiteralLength);
    }
    else{
      literal[literalIndex++] = *expressionIterator;
    }
    expressionIterator++;
  }
  if(literalIndex > 0){
    pushExprStack(outPutStack, literal);
    literalIndex = 0;
  }
  while(exprStackIsEmpty(symbolStack) != true){
    pushExprStack(outPutStack, peekExprStack(symbolStack));
    popExprStack(symbolStack);
  }
  printExprStack(outPutStack);
  solveExpression(outPutStack, expressionStringLength, maxLiteralLength);
}

int main(){
  char* evaluationString = (char*)malloc(sizeof(char) * MAX_EXPRESSION_LENGTH);
  printf("Enter expression: ");
  if(scanf("%" MAX_EXPRESSION_LENGTH_S "[^\n]", evaluationString) != 1){
    printf("Error while reading in expression\n");
    return 1;
  }
  evaluateExpression(evaluationString, MAX_EXPRESSION_LENGTH, MAX_LITERAL_LENGTH);
  return 0;
}
