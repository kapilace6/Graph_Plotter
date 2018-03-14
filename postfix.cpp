#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>
#include<stdbool.h>
#include<regex>

using namespace std;

#define STACK_SIZE 200

struct values{
	bool op;
	bool constant;
	double value;
	int operation;
};

class parse{
    char expression[STACK_SIZE];
    
    char token[20];
    int tokenInd = -1;
    int numTokens = 0;
    struct values postfix[STACK_SIZE];
    int postInd = -1;
    
    int stack[STACK_SIZE];
    int stack_top = -1;
    
    double evalStack[STACK_SIZE];
    int evalInd = -1;
    
    bool unary = 1;
    
    void push_op(int op){
		if(stack_top < STACK_SIZE - 1){
			stack_top++;
			stack[stack_top] = op;
		}
    }
    
    int pop_op(){
		int ret;
		if(stack_top >= 0){
			ret = stack[stack_top];
			stack_top--;
			return ret;
		}else{
			return -1;
		}
	}
	
	void evalPush(double d){
		
		if(evalInd < STACK_SIZE - 1){
			evalInd++;
			evalStack[evalInd] = d;
			printf("pushed %lf.\n", d);
		}
	}
	
	double evalPop(){
		double ret;
		if(evalInd >= 0){
			ret = evalStack[evalInd];
			evalInd--;
			return ret;
		}else{
			printf("Popping from empty stack.\n");
			return -1;
		}
	}
	
	
    int priority(int op){
        switch(op){
            case 0:
				return 1;
				break;
			case 1:
				return 1;
				break;
			case 2:
				return 2;
				break;
			case 3:
				return 2;
				break;
			case 4:
				return 3;
				break;
			case 5:
				return 4;
				break;
			case 6:
				return 4;
				break;
			case 7:
				return 0;
				break;
			case 9:
				return 0;
				break;
			case 10:
				return 0;
				break;
			case 11:
				return 0;
				break;
			case 12:
				return 0;
				break;
        }
        return 0;
    }
    int process_character(char c, bool unary){
		switch(c){
			case '+':
				if(unary){
					return 5;
				}else{
					return 0;
				}
				break;
			case '-':
				if(unary){
					return 6;
				}else{
					return 1;
				}
				break;
			case '*':
				return 2;
				break;
			case '/':
				return 3;
				break;
			case '^':
				return 4;
				break;
			case '(':
				return 5;
				break;
			case ')':
				return 6;
				break;
		}
	}
	
	int checkFunction(){
		if(!strcmp(token,"sin")){
			return 9;
		}else if(!strcmp(token,"cos")){
			return 10;
		}else if(!strcmp(token,"tan")){
			return 11;
		}else if(!strcmp(token,"log")){
			return 12;
		}
		
		return -1;
	}
	
	bool checkNumber(){
		regex e ("[-+]?([0-9]*.[0-9]+|[0-9]+)");

		if (regex_match (token,e)){
			return true;
		}
		return false;
	}
	
	bool checkVar(){
		if(!strcmp(token,"x")){
			return true;
		}
		return false;
	}
    public: int intopost(char exp[]){
        strcpy(expression,exp);
        //scanf("%s",expression);
        int i;
        int len = strlen(expression);
        bool unary = 1;
        bool constant = 1;
        
        
        for(i=0;i<len;i++){
			char c = expression[i];
			if(c == '+'||c == '-'||c == '*'||c=='/'||c=='^'){
				tokenInd++;
				token[tokenInd] = '\0';
				printf("%s ", token);
				bool isnum = checkNumber();
				if(!isnum){
					bool isvar = checkVar();
					if(isvar){
						postInd++;
						postfix[postInd].op = false;
						postfix[postInd].constant = false;
					}
				}else{
					postInd++;
					postfix[postInd].constant = true;
					postfix[postInd].value = atof(token);
				}
				while(stack_top >= 0){
					if(priority(process_character(c, unary)) <= priority(stack[stack_top])){
						int to_exp = pop_op();
						postInd++;
						postfix[postInd].constant = false;
						postfix[postInd].op = true;
						postfix[postInd].operation = to_exp;
					}else{
						break;
					}
					
					if(stack_top == -1){
						//error
					}
				}
				tokenInd = -1;
				push_op(process_character(c,unary));
				unary = 1;
			}else if(isalnum(c) || c == '.'){
				unary = 0;
				tokenInd++;
				token[tokenInd] = c;
			}else if(c == '('){
				if(tokenInd >= 0){
					tokenInd++;
					token[tokenInd] = '\0';
					printf("%s ", token);
					int isfunc = checkFunction();
					if(isfunc == -1){
						//error
					}else{
						push_op(isfunc);
					}
				}else{
					push_op(process_character(c,unary));
				}
				tokenInd = -1;
				unary = 1;
				
			}else if(c == ')'){
				
				if(tokenInd>=0){
					tokenInd++;
					token[tokenInd] = '\0';
					printf("%s ", token);
					bool num = checkNumber();
					if(num){
						double f = atof(token);
						postInd++;
						postfix[postInd].constant=true;
						postfix[postInd].op = false;
						postfix[postInd].value = f;
					}else{
						bool isvar = checkVar();
						if(isvar){
							postInd++;
							postfix[postInd].constant=false;
							postfix[postInd].op = false;
						}
					}
				}
				unary = 0;
				while(stack_top >= 0){
					int to_exp = pop_op();
					if(to_exp == 9 || to_exp == 10 || to_exp == 11 || to_exp == 12){
						postInd++;
						postfix[postInd].constant = false;
						postfix[postInd].op = true;
						postfix[postInd].operation = to_exp;
						break;
					}else if(to_exp == 7){
						break;
					}else{
						postInd++;
						postfix[postInd].op = true;
						postfix[postInd].operation = to_exp;
						postfix[postInd].constant = false;
					}
					 
					if(stack_top < 0){
						//error
					}
					
					
				}
				tokenInd = -1;
				unary = 0;
			}
		}
		
		if(tokenInd>-1){
			tokenInd++;
			token[tokenInd] = '\0';
			printf("%s ", token);
			bool isnum = checkNumber();
			if(!isnum){
				bool isvar = checkVar();
				if(isvar){
					postInd++;
					postfix[postInd].op = false;
					postfix[postInd].constant = false;
				}
			}else{
				postInd++;
				postfix[postInd].op = false;
				postfix[postInd].constant = true;
				postfix[postInd].value = atof(token);
			}
		}
		while(stack_top >= 0){
			postInd++;
			postfix[postInd].op = true;
			postfix[postInd].operation = pop_op();
		}
		numTokens = postInd + 1;
        
    }
    float op(int o){
        switch(o){
			double f1,f2;
            case 0:
				f2 = evalPop();
				f1 = evalPop();
				printf("f1 and f2 %f %f ", f1,f2);
				printf("Added %f ", f1+f2);
				return f1+f2;
				break;
			case 1:
				f2 = evalPop();
				f1 = evalPop();
				return f1-f2;
				break;
			case 2:
				f2 = evalPop();
				f1 = evalPop();
				return f1*f2;
				break;
			case 3:
				f2 = evalPop();
				f1 = evalPop();
				return f1/f2;
				break;
			case 4:
				f2 = evalPop();
				f1 = evalPop();
				return pow(f1,f2);
				break;
			case 5:
				f1 = evalPop();
				return f1;
				break;
			case 6:
				f1 = evalPop();
				return -f1;
				break;
			case 9:
				f1 = evalPop();
				return sin(f1);
				break;
			case 10:
				f1 = evalPop();
				return cos(f1);
				break;
			case 11:
				f1 = evalPop();
				return tan(f1);
				break;
			case 12:
				f1 = evalPop();
				return log(f1);
				break;
        }
        return 0;
        
    }
    
    public: double evalpost(double x){
		int i;
		for(i=0;i<numTokens;i++){
			if(postfix[i].op==false){
				if(postfix[i].constant==true){
					printf("pushed %lf evalInd = %d ", postfix[i].value, evalInd);
					evalPush(postfix[i].value);
				}else{
					evalPush(x);
				}
			}else{
				printf("Operation %d ", postfix[i].operation);
				evalPush(op(postfix[i].operation));
			}
		}
		//printf("evalInd %d ", evalInd);
		//for(i=0;i<=evalInd;i++){
			//printf("%d %lf ", i, evalStack[i]);
		//}
		if(evalInd == 0){
			return evalPop();
		}else{
			//error
		}
    }
};

