#include <stdio.h>
#include <stdlib.h>
#include "customStack.h"

#define MAX_EXPRESSION_LENGTH 50
#define MAX_EXPRESSION_LENGTH_S "50"
#define MAX_LITERAL_LENGTH 10

int getPrecedence(char* c){
  switch(*c){
  case '+':
  case '-':
    return 1;
  case '*':
  case '/':
    return 2;
  case '^':
    return 3;
  case '(':
    return 0;
  default:
    printf("Unknown operator detected in getPrecedence() : %c\n", *c);
    exit(1);
  }
}

bool isOperator(char* expressionIterator){
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
  char* peek = NULL; //FOR MEMORY DEALLOCATION OF POPPED VALUES
  int operand1 = 0, operand2 = 0, result = 0;
  printExprStack(postFixStack);
  while(exprStackIsEmpty(postFixStack) == false){
    top = peekExprStack(postFixStack);
    popExprStack(postFixStack);
    if(isOperator(top) == false){
      pushExprStack(solutionStack, top);
    }
    else{
      //operand2
      peek = peekExprStack(solutionStack);
      operand2 = strtol(peek, &end, 10);
      popExprStack(solutionStack);
      free(peek);
      //operand1
      peek = peekExprStack(solutionStack);
      operand1 = strtol(peek, &end, 10);
      popExprStack(solutionStack);
      free(peek);

      result = binaryOperateToInt(top, operand1, operand2);
      free(top);

      intString = malloc(sizeof(char) * 10);
      sprintf(intString, "%d", result);
      pushExprStack(solutionStack, intString);
    }
  }
  printf("Solution: %s\n", peekExprStack(solutionStack));

  //MEMORY DEALLOCATING
  freeStackMemory(postFixStack);
  freeStackMemory(solutionStack);
}

void pushCurrentLiteral(char** literal, int* literalIndex, ExprStack* outPutStack){
  if(*literalIndex > 0){
    pushExprStack(outPutStack, *literal);
    *literalIndex = 0;
    *literal = malloc(sizeof(char));
  }
}

void evaluateExpression(char* expressionString, int expressionStringLength, int maxLiteralLength){
  ExprStack* symbolStack = newExprStack(expressionStringLength, 1);
  ExprStack* outPutStack = newExprStack(expressionStringLength, maxLiteralLength);
  char* expressionIterator = expressionString;
  char* literal = malloc(sizeof(char) * MAX_LITERAL_LENGTH);
  int literalIndex = 0;
  char* topChar = NULL;

  while(*expressionIterator != '\0'){
    //IGNORE SPACES
    if(*expressionIterator == ' '){
    }
    //IF ( DETECTED THEN PUSH IT ONTO THE SYMBOL STACK
    else if(*expressionIterator == '('){
      *literal = *expressionIterator;
      pushExprStack(symbolStack, literal);
      literal = malloc(sizeof(char) * maxLiteralLength);
    }
    //IF ) THEN POP SYMBOL STACK ONTO OUTPUTSTACK UNTIL A MATCHING ) IS DETECTED
    else if(*expressionIterator == ')'){
      pushCurrentLiteral(&literal, &literalIndex, outPutStack);
      topChar = peekExprStack(symbolStack);
      while(exprStackIsEmpty(symbolStack) == false && *topChar != '('){
	pushExprStack(outPutStack, topChar);
	popExprStack(symbolStack);
	topChar = peekExprStack(symbolStack);
      }
      popExprStack(symbolStack);
    }
    //IF AN OPERATOR IS DETECTED THEN PUSH IT ONTO THE OUTPUT STACK
    else if(isOperator(expressionIterator) == true){
      pushCurrentLiteral(&literal, &literalIndex, outPutStack);
      *literal = *expressionIterator;
      if(exprStackIsEmpty(symbolStack) == true){
	pushExprStack(symbolStack, literal);
      }
      else{
	topChar = peekExprStack(symbolStack);
	if(getPrecedence(topChar) >= getPrecedence(literal)){
	  popExprStack(symbolStack);
	  pushExprStack(outPutStack, topChar);
	}
	pushExprStack(symbolStack, literal);
      }
      literal = malloc(sizeof(char) * maxLiteralLength);
    }
    //ELSE THE ITEM MUST BE A NUMERICAL VALUE SO APPEND IT TO THE GROWING LITERAL 
    else{
      literal[literalIndex++] = *expressionIterator;
    }
    expressionIterator++;
  }
  pushCurrentLiteral(&literal, &literalIndex, outPutStack);

  while(exprStackIsEmpty(symbolStack) == false){
    pushExprStack(outPutStack, peekExprStack(symbolStack));
    popExprStack(symbolStack);
  }

  printExprStack(outPutStack);
  solveExpression(outPutStack, expressionStringLength, maxLiteralLength);

  //MEMORY DEALLOCTING
  freeStackMemory(symbolStack);
  freeStackMemory(outPutStack);
  free(literal);
}

int main(){
  char* evaluationString = (char*)malloc(sizeof(char) * MAX_EXPRESSION_LENGTH);
  printf("Enter expression: ");
  if(scanf("%" MAX_EXPRESSION_LENGTH_S "[^\n]", evaluationString) != 1){
    printf("Error while reading in expression\n");
    return 1;
  }
  evaluateExpression(evaluationString, MAX_EXPRESSION_LENGTH, MAX_LITERAL_LENGTH);
  free(evaluationString);
  return 0;
}
