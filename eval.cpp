//
//  eval.cpp updated
//  mazestack
//
//  Created by Charles Qin on 2/4/23.
//

#include "Set.h"  // with ItemType being a type alias for char
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cassert>
using namespace std;


int precedence(char c) {
    if (c == '|') return 1;
    if (c == '&') return 2;
    if (c == '!') return 3;
    return 0;
}

bool syntax(string in){
    bool valid = true;
    int count = 0, opCount = 0;
    string infix = "";


    // checks for valid characters in string
    for(int i = 0; i < in.length(); i++){
        if(!islower(in[i]) && in[i] != ' ' && in[i] != '!' && in[i] != '(' && in[i] != ')' && in[i] != '|' && in[i] != '&') valid = false;
        if(in[i] != ' ') infix += in[i];
    }

    if(!valid) {
        return false;
    }

    // checks for if there's only operands or no operands
    for(int i = 0; i < infix.length(); i++){
        if(isalpha(infix[i])) count++;
        if(infix[i] == '|' || infix[i] == '&') opCount++;
    }

    if((count > 1 && opCount < 1) || (count == 0 && opCount > 0)) return false;

    //checks for first character
    if(infix[0] == ')' || infix[0] == '&' || infix[0] == '|') return false;
    
    if(infix[0] == '('){
        bool found3 = false;
        for(int i = 0; i < infix.length(); i ++){
            if(infix[i] == ')') {
                found3 = true;
                infix[i] = '*';
                infix[0] = '*';
                break;
            }
        }
        if(found3 == false) return false;

    }
        
    //checks for middle characters
    for(int i = 1; i < infix.length()-1; i++){

        switch(infix[i]){
            case '!':
            {
                if(!isalpha(infix[i+1]) && infix[i+1] != '(' && infix[i+1] != '!') return false;
                break;
            }

            case ')':
            {
                bool found = false;
                for(int j = 0; j < i; j++){
                    if(infix[j] == '(') {found = true;
                        infix[j] = '*';
                        infix[i] = '*';
                        break;
                    }
                }
                if(!found) return false;
                if(!isalpha(infix[i-1])) return false;
                if(isalpha(infix[i+1]) || infix[i+1] == '(') return false;
                break;
            }

            case '(':
            {
                bool found2 = false;
                for(int j = i; j < infix.length(); j ++){
                    if(infix[j] == ')') {found2 = true;
                        infix[j] = '*';
                        infix[i] = '*';
                        break;
                    }
                }
                if(!found2) return false;
                if(isalpha(infix[i-1]) || infix[i-1] == ')') return false;
                break;
            }

            case '|':
            case '&':
            {
                if(infix[i+1] != '!' && infix[i+1] != '(' && infix[i-1] != ')' && !isalpha(infix[i-1]) && !isalpha(infix[i+1])) return false;
                break;
            }
            case '*': break;
            default:
            {
                if(isalpha(infix[i-1]) || isalpha(infix[i-1])) return false;
                break;
            }

        }

    }

    //checks for last character
    if(infix[infix.length()-1] == '|' || infix[infix.length()-1] == '&' || infix[infix.length()-1] == '!' || infix[infix.length()-1] == '(') return false;

    if(infix[infix.length()-1] == ')' && !isalpha(infix[infix.length()-2])) return false;



    return true;
}

string infixToPostfix(const string &infix) {
    stack<char> operators;
    string postfix;
    
    for (char c : infix) {
            switch (c) {
                case '!':
                case '(':
                    operators.push(c);
                    break;
                case ')':
                    while (operators.top() != '(') {
                        postfix += operators.top();
                        operators.pop();
                    }
                    operators.pop();
                    break;
                case '&':
                case '|':
                    while (!operators.empty() && operators.top() != '(' &&
                           precedence(c) <= precedence(operators.top())) {
                        postfix += operators.top();
                        operators.pop();
                    }
                    operators.push(c);
                    break;
                case ' ': break;
                default:
                    postfix += c;
                    break;
            }
        }
        
        while (!operators.empty()) {
            postfix += operators.top();
            operators.pop();
        }
        
        return postfix;
    }

void fixToOperands(string fix, string& op){
    for(char ch:fix){
        if(islower(ch)) op += ch;
    }
}

bool performEvaluation(const string &postfix, const Set &truthSet, const Set &falseSet, bool &finalResult)
{
    stack<bool> operandStack;
    for (const char &symbol : postfix)
    {
        switch (symbol)
        {
        case '!':
        {
            bool topOfStack = operandStack.top();
            operandStack.pop();
            operandStack.push(!topOfStack);
            break;
        }
        case '&':
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand1 = operandStack.top();
            operandStack.pop();
            operandStack.push(operand1 && operand2);
            break;
        }
        case '|':
        {
            bool operand2 = operandStack.top();
            operandStack.pop();
            bool operand1 = operandStack.top();
            operandStack.pop();
            operandStack.push(operand1 || operand2);
            break;
        }
        default:
        {
            if (truthSet.contains(symbol))
                operandStack.push(true);
            else if (falseSet.contains(symbol))
                operandStack.push(false);
        }
        }
    }
    finalResult = operandStack.top();
    return true;
}

bool isOperand(char ch, string op)
{
    bool is = false;
    for (int i = 0; i < op.size(); i++)
    {
        if (ch == op[i])
            is = true;
    }
    if (!is)
        return false;
    else
        return true;
}


int evaluate(string infix, const Set& trueValues, const Set& falseValues, string& postfix, bool& result){
    // Evaluate a boolean expression
   
    //   If infix is a syntactically valid infix boolean expression whose
    //   only operands are single lower case letters (whether or not they
    //   appear in the values sets), then postfix is set to the postfix
    //   form of the expression.  If not, postfix might or might not be
    //   changed, result is unchanged, and the function returns 1.
    
    if(syntax(infix)) postfix = infixToPostfix(infix);
    else return 1;
    
    
    //creates operands only
    string operands;
    fixToOperands(infix, operands);
    

    //   If infix is a syntactically valid infix boolean expression whose
    //   only operands are single lower case letters:

    bool symdiff = true;
    if(syntax(infix)){


        for(char ch:operands){
            if(!((trueValues.contains(ch) || falseValues.contains(ch)) && !(trueValues.contains(ch) && falseValues.contains(ch)))) //messy
                symdiff = false;
        }

        //      If every operand letter in the expression appears in either
        //      trueValues or falseValues but not both, then result is set to the
        //      result of evaluating the expression (using for each letter in the
        //      expression the value true if that letter appears in trueValues or
        //      false if that letter appears in false values) and the function
        //      returns 0.


        if(symdiff){
            performEvaluation(postfix, trueValues, falseValues, result);

            return 0;
        }
        
        
        //      Otherwise, result is unchanged and the value the function returns
        //      depends on these two conditions:
        //        at least one letter in the expression is in neither the
        //            trueValues nor the falseValues sets; and
        //        at least one letter in the expression is in both the
        //            trueValues and the falseValues set.
        //      If only the first condition holds, the function returns 2; if
        //      only the second holds, the function returns 3.  If both hold
        //      the function returns either 2 or 3 (and the function is not
        //      required to return the same one if called another time with the
        //      same arguments).
        else {
            
            bool first = false;
            bool second = false;
            for(char ch:operands){
                if(!trueValues.contains(ch) && !falseValues.contains(ch)){
                    first = true;
                }
                if(trueValues.contains(ch) && falseValues.contains(ch)){
                    second = true;
                }
            }
            
            if(first && !second) return 2;
            else if(!first && second) return 3;
            else if(first && second) return 2;
            
        }
    }
    
        return -1; //should never be returned
}

int main()
        {
    Set tset;
    tset.insert('t');
    Set fset;
    fset.insert('f');
    string pf;
    bool answer;
//            assert(evaluate("w| f", trues, falses, pf, answer) == 0  &&  pf == "wf|" &&  answer);
    // generated at TomBinford.github.io/expressions
    // credit is required per http://web.cs.ucla.edu/classes/winter23/cs32/integrity.html
    assert(evaluate("(t|t|f&t)&((!t))", tset, fset, pf, answer) == 0 && pf == "tt|ft&|t!&" && !answer);
    assert(evaluate("(t&t)&(t&f&!f)&!(t&t|!t)", tset, fset, pf, answer) == 0 && pf == "tt&tf&f!&&tt&t!|!&" && !answer);
    assert(evaluate("(t|t|t&t)&((t&t))", tset, fset, pf, answer) == 0 && pf == "tt|tt&|tt&&" && answer);
    assert(evaluate("!(!t|!f)", tset, fset, pf, answer) == 0 && pf == "t!f!|!" && !answer);
    assert(evaluate("(!(t&t|!f))", tset, fset, pf, answer) == 0 && pf == "tt&f!|!" && !answer);
    assert(evaluate("!((f|f)|!(t&f))", tset, fset, pf, answer) == 0 && pf == "ff|tf&!|!" && !answer);
    assert(evaluate("!!!!t", tset, fset, pf, answer) == 0 && pf == "t!!!!" && answer);
    assert(evaluate("!((t&f)|(f|t|!f))", tset, fset, pf, answer) == 0 && pf == "tf&ft|f!||!" && !answer);
    assert(evaluate("(!!f|(t&t))", tset, fset, pf, answer) == 0 && pf == "f!!tt&|" && answer);
    assert(evaluate("f|f|(t|t)|(!t)|!(f&t)&!(t&f)", tset, fset, pf, answer) == 0 && pf == "ff|tt||t!|ft&!tf&!&|" && answer);
    assert(evaluate("(t&f|!t|(t|t))&(!(f&t)|!(f&t))", tset, fset, pf, answer) == 0 && pf == "tf&t!|tt||ft&!ft&!|&" && answer);
    assert(evaluate("(!t&(f&f)&(!t&!t))", tset, fset, pf, answer) == 0 && pf == "t!ff&&t!t!&&" && !answer);
    assert(evaluate("!(!t|(t|f))|!(!t|(t|f))", tset, fset, pf, answer) == 0 && pf == "t!tf||!t!tf||!|" && !answer);
    assert(evaluate("!(f&f)|(!t)|(f&f&(t|f)|!(f&f))", tset, fset, pf, answer) == 0 && pf == "ff&!t!|ff&tf|&ff&!||" && answer);
    assert(evaluate("(!!f)&(!!t&!(f&t))", tset, fset, pf, answer) == 0 && pf == "f!!t!!ft&!&&" && !answer);
    assert(evaluate("!(t|t)&!(!f&!t)", tset, fset, pf, answer) == 0 && pf == "tt|!f!t!&!&" && !answer);
    assert(evaluate("(!(t|t|!t))", tset, fset, pf, answer) == 0 && pf == "tt|t!|!" && !answer);
    assert(evaluate("(!!t)&(!f|(t|t)|(f&f))", tset, fset, pf, answer) == 0 && pf == "t!!f!tt||ff&|&" && answer);
    assert(evaluate("!(!f|!t)&(t&t&(t|f)&(t&t&(f|f)))", tset, fset, pf, answer) == 0 && pf == "f!t!|!tt&tf|&tt&ff|&&&" && !answer);
    assert(evaluate("(t|f|!f)&(!t)|((!t)|(t&t|!f))", tset, fset, pf, answer) == 0 && pf == "tf|f!|t!&t!tt&f!|||" && answer);
    assert(evaluate("(f&f|t&t)&((t&f)|!(f&t))", tset, fset, pf, answer) == 0 && pf == "ff&tt&|tf&ft&!|&" && answer);
    assert(evaluate("(!!(f&f))", tset, fset, pf, answer) == 0 && pf == "ff&!!" && !answer);
    assert(evaluate("((!t|f&f)&(t&t))", tset, fset, pf, answer) == 0 && pf == "t!ff&|tt&&" && !answer);
    assert(evaluate("(!(f|f)&(!t&!f))", tset, fset, pf, answer) == 0 && pf == "ff|!t!f!&&" && !answer);
    assert(evaluate("(((t&t)))", tset, fset, pf, answer) == 0 && pf == "tt&" && answer);
    assert(evaluate("(!(f|t))&(f|f|!t|f&t&(f|f))", tset, fset, pf, answer) == 0 && pf == "ft|!ff|t!|ft&ff|&|&" && !answer);
    assert(evaluate("!((f|f)&!t)", tset, fset, pf, answer) == 0 && pf == "ff|t!&!" && answer);
    assert(evaluate("(!f|t&t)&(t&t&!f)|(!(f|f)|(f&f|!t))", tset, fset, pf, answer) == 0 && pf == "f!tt&|tt&f!&&ff|!ff&t!|||" && answer);
    assert(evaluate("(!(t|t)|(f&t|(f|t)))", tset, fset, pf, answer) == 0 && pf == "tt|!ft&ft|||" && answer);
    assert(evaluate("!!(!t|!f)", tset, fset, pf, answer) == 0 && pf == "t!f!|!!" && answer);
    assert(evaluate("(f|f|(t|t)|(f&f|t&t))", tset, fset, pf, answer) == 0 && pf == "ff|tt||ff&tt&||" && answer);
    assert(evaluate("!(t|t|t&f)", tset, fset, pf, answer) == 0 && pf == "tt|tf&|!" && !answer);
    assert(evaluate("!((!t|!t)&((f|f)&(t&t)))", tset, fset, pf, answer) == 0 && pf == "t!t!|ff|tt&&&!" && answer);
    assert(evaluate("!(!t)|(f&f|!f|(!t|!f))", tset, fset, pf, answer) == 0 && pf == "t!!ff&f!|t!f!|||" && answer);
    assert(evaluate("(!t|f&f|f&f&(f|t))", tset, fset, pf, answer) == 0 && pf == "t!ff&|ff&ft|&|" && !answer);
    assert(evaluate("(!(f|f)|(f|t)&!t)", tset, fset, pf, answer) == 0 && pf == "ff|!ft|t!&|" && answer);
    assert(evaluate("((t&t))|!(t&t|(f|t))", tset, fset, pf, answer) == 0 && pf == "tt&tt&ft||!|" && answer);
    assert(evaluate("((t|f|t&t))", tset, fset, pf, answer) == 0 && pf == "tf|tt&|" && answer);
    assert(evaluate("!((f|t)&(f&f)&(f|f))", tset, fset, pf, answer) == 0 && pf == "ft|ff&&ff|&!" && answer);
    assert(evaluate("(t|f)&((t|f)&(f|t))|(!f|!f)", tset, fset, pf, answer) == 0 && pf == "tf|tf|ft|&&f!f!||" && answer);
    assert(evaluate("(t|t)&!t&(!t)|(t|t|!f)&!(t|f)", tset, fset, pf, answer) == 0 && pf == "tt|t!&t!&tt|f!|tf|!&|" && !answer);
    assert(evaluate("!(f|f)|t&t&(f|f)&(f|f|(t|f))", tset, fset, pf, answer) == 0 && pf == "ff|!tt&ff|&ff|tf||&|" && answer);
    assert(evaluate("(t&f|(t|f))|!!f&!(t|t)", tset, fset, pf, answer) == 0 && pf == "tf&tf||f!!tt|!&|" && answer);
    assert(evaluate("(!t&!t|!(f&f))&(!t&!t|!(t&t))", tset, fset, pf, answer) == 0 && pf == "t!t!&ff&!|t!t!&tt&!|&" && !answer);
    assert(evaluate("!(t&t|t&t|!(t&t))", tset, fset, pf, answer) == 0 && pf == "tt&tt&|tt&!|!" && !answer);
    assert(evaluate("((t&t)&(!f))", tset, fset, pf, answer) == 0 && pf == "tt&f!&" && answer);
    assert(evaluate("((!t)|!(f|t))", tset, fset, pf, answer) == 0 && pf == "t!ft|!|" && !answer);
    assert(evaluate("!(f|f)&!(!t|(f|f))", tset, fset, pf, answer) == 0 && pf == "ff|!t!ff||!&" && answer);
    assert(evaluate("(t&f)&!!t|t&f&(t&t)&(t&f|!t)", tset, fset, pf, answer) == 0 && pf == "tf&t!!&tf&tt&&tf&t!|&|" && !answer);
    assert(evaluate("(!t)&(!t)|((f|f)|(t|f|f&f))", tset, fset, pf, answer) == 0 && pf == "t!t!&ff|tf|ff&|||" && answer);
    assert(evaluate("f )t", tset, fset, pf, answer) == 1);
    assert(evaluate(")(|f", tset, fset, pf, answer) == 1);
    assert(evaluate("!)!|&", tset, fset, pf, answer) == 1);
    assert(evaluate("|)&(|", tset, fset, pf, answer) == 1);
    assert(evaluate("f(&(", tset, fset, pf, answer) == 1);
    assert(evaluate("&||(", tset, fset, pf, answer) == 1);
    assert(evaluate("ffff", tset, fset, pf, answer) == 1);
    assert(evaluate("|t |))", tset, fset, pf, answer) == 1);
    assert(evaluate("!!)|", tset, fset, pf, answer) == 1);
    assert(evaluate("(&(|", tset, fset, pf, answer) == 1);
    assert(evaluate("|ft(&", tset, fset, pf, answer) == 1);
    assert(evaluate("&))f(", tset, fset, pf, answer) == 1);
    assert(evaluate("t&|((!", tset, fset, pf, answer) == 1);
    assert(evaluate("|!|)", tset, fset, pf, answer) == 1);
    assert(evaluate(")|(&", tset, fset, pf, answer) == 1);
    assert(evaluate("(ff()", tset, fset, pf, answer) == 1);
    assert(evaluate("f!||f", tset, fset, pf, answer) == 1);
    assert(evaluate("&)f)", tset, fset, pf, answer) == 1);
    assert(evaluate("((f(", tset, fset, pf, answer) == 1);
    assert(evaluate(") ()f", tset, fset, pf, answer) == 1);
    assert(evaluate("t)tt", tset, fset, pf, answer) == 1);
    assert(evaluate("(ff|!|", tset, fset, pf, answer) == 1);
    assert(evaluate("!|(|", tset, fset, pf, answer) == 1);
    assert(evaluate("&())", tset, fset, pf, answer) == 1);
    assert(evaluate("&t!)(", tset, fset, pf, answer) == 1);
    assert(evaluate("&)&&!)", tset, fset, pf, answer) == 1);
    assert(evaluate("t t(", tset, fset, pf, answer) == 1);
    assert(evaluate(" &|(", tset, fset, pf, answer) == 1);
    assert(evaluate("&)|t", tset, fset, pf, answer) == 1);
    assert(evaluate(")|t f", tset, fset, pf, answer) == 1);
    assert(evaluate("(f!&", tset, fset, pf, answer) == 1);
    assert(evaluate("&|(t&f", tset, fset, pf, answer) == 1);
    assert(evaluate("&)(|", tset, fset, pf, answer) == 1);
    assert(evaluate("tt!)&", tset, fset, pf, answer) == 1);
    assert(evaluate("|(ttt ", tset, fset, pf, answer) == 1);
    assert(evaluate("f)f!))", tset, fset, pf, answer) == 1);
    assert(evaluate("||t) ", tset, fset, pf, answer) == 1);
    assert(evaluate("t)(t|!", tset, fset, pf, answer) == 1);
    assert(evaluate("  &|", tset, fset, pf, answer) == 1);
    assert(evaluate("!&t(t", tset, fset, pf, answer) == 1);
    assert(evaluate("))f(f&", tset, fset, pf, answer) == 1);
    assert(evaluate("ft|&", tset, fset, pf, answer) == 1);
    assert(evaluate("&||t", tset, fset, pf, answer) == 1);
    assert(evaluate("ft!)|", tset, fset, pf, answer) == 1);
    assert(evaluate("( ||)", tset, fset, pf, answer) == 1);
    assert(evaluate("!)||!f", tset, fset, pf, answer) == 1);
    assert(evaluate("&tf&f", tset, fset, pf, answer) == 1);
    assert(evaluate("t!)&)", tset, fset, pf, answer) == 1);
    assert(evaluate("t!))", tset, fset, pf, answer) == 1);
    assert(evaluate("!t(!)", tset, fset, pf, answer) == 1);
            cout << "Passed all tests" << endl;
        }
