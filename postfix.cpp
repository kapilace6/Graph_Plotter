#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<ctype.h>
#include<math.h>


class parse{
    char expression[200];
    char post[200][10];
    char stack[200][10];
    int stackTop = -1;
    int postInd = -1;
    float evalstack[200];
    int floattop = -1;
    void pushfloat(float val){
        floattop++;
        evalstack[floattop] = val;
    }
    float popfloat(){
        return evalstack[floattop--];
    }
    void push(char ele[]){
        stackTop++;
        strcpy(stack[stackTop],ele);
    }
    int priority(char op){
        switch(op){
            case '+':
                return 0;
                break;
            case '-':
                return 0;
                break;
            case '*':
                return 1;
                break;
            case '/':
                return 1;
                break;
            case '^':
                return 2;
                break;
            case '(':
                return -1;
                break;
        }
        return 0;
    }
    public: int intopost(char exp[]){
        strcpy(expression,exp);
        //scanf("%s",expression);
        int i,numInd=0;
        int len = strlen(expression);
        int unary = 1;
        char pushE[10];
        for(i=0;i<len;i++){
            if(isdigit(expression[i])||expression[i] == '.'){
                if(numInd==0){
                    pushE[0] = '+';
                    pushE[1] = expression[i];
                    numInd = 2;
                }else{
                    pushE[numInd] = expression[i];
                    numInd++;
                }
                unary = 0;
            }else if(expression[i] == ' '){
                continue;
            }else if(expression[i] == 's'||expression[i] == 'c'||expression[i] == 't'||expression[i] == 'l'||expression[i] == 'x'){
                if(numInd==0){
                    pushE[0] = '+';
                }
                pushE[1] = expression[i];
                pushE[2] = '\0';
                strcpy(post[++postInd],pushE);
                unary = 0;
                numInd = 0;
            }else{
                if(numInd>0){
                    pushE[numInd] = '\0';
                    numInd = 0;
                    strcpy(post[++postInd],pushE);
                }
                if(expression[i] == '('){
                    pushE[0] = expression[i];
                    pushE[1] = '\0';
                    push(pushE);
                    unary = 1;
                }else if(expression[i] == '+'||expression[i] == '-' ||expression[i] == '*' ||expression[i] == '/' ||expression[i] == '^'){
                    if((expression[i] == '+' || expression[i] == '-')&&unary==1){
                        pushE[0] = expression[i];
                        numInd = 1;
                    }else{
                        if(stackTop==-1){
                            pushE[0] = expression[i];
                            pushE[1] = '\0';
                            push(pushE);
                        }else if(stack[stackTop][0] == '('){
                            pushE[0] = expression[i];
                            pushE[1] = '\0';
                            push(pushE);
                        }else{
                            while(stack[stackTop][0]=='+'||stack[stackTop][0]=='-'||stack[stackTop][0]=='*'||stack[stackTop][0]=='/'||stack[stackTop][0]=='^'){
                                if(stackTop==-1){
                                    break;
                                }
                                if(priority(stack[stackTop][0])<priority(expression[i])){
                                    break;
                                }else{
                                    postInd++;
                                    strcpy(post[postInd],stack[stackTop]);
                                    stackTop--; 
                                }
                            }
                            pushE[0] = expression[i];
                            pushE[1] = '\0';
                            push(pushE);
                        }
                    }
                    unary = 1;
                }else if(expression[i] == ')'){
                    unary = 1;
                    printf("%s",stack[stackTop]);
                    while(stack[stackTop][0] != '('){
                        postInd++;
                        if(stackTop>-1){
                            strcpy(post[postInd],stack[stackTop]);
                        }
                        stackTop--;
                    }
                    if(stackTop>-1){
                        //printf("pop");
                        stackTop--;
                    }
                }
                
            }
        }
        if(numInd>0){
            pushE[numInd] = '\0';
            numInd = 0;
            strcpy(post[++postInd],pushE);
        }
        while(stackTop>-1){
            postInd++;
            strcpy(post[postInd],stack[stackTop]);
            stackTop--;
        }
        //printf("%d %d\n",postInd,stackTop);
        for(i=0;i<=postInd+1;i++){
            printf("%s ",post[i]);
        }
        return 0;
    }
    float op(float f1,float f2,char o){
        switch(o){
            case '+':
                return f1+f2;
                break;
            case '-':
                return f1-f2;
                break;
            case '*':
                return f1*f2;
                break;
            case '/':
                return f1/f2;
                break;
            case '^':
                return pow(f1,f2);
                break;
        }
        return 0;
        
    }
    float func(char op,float x){
        switch(op){
            case 's':
                return sin(x);
                break;
            case 'c':
                return cos(x);
                break;
            case 't':
                return tan(x);
                break;
            case 'l':
                return log(x);
                break;
        }
        return 0;
    }
    public: float evalpost(float x){
       float val;
       int i;
       for(i=0;i<=postInd;i++){
           if((post[i][0] == '+'||post[i][0] == '-')&&strlen(post[i])>1){
               if(post[i][1] == 's'||post[i][1] == 'c'||post[i][1] == 't'||post[i][1] == 'l'){
                   if(post[i][0] == '+'){
                       pushfloat(func(post[i][1],x));
                   }else if(post[i][0] == '-'){
                       pushfloat(-1*func(post[i][1],x));
                   }
               }else if(post[i][1] == 'x'){
                   if(post[i][0] == '+'){
                       pushfloat(x);
                   }else if(post[i][0] == '-'){
                       pushfloat(-1*x);
                   }
               }else{
                   pushfloat(strtof(post[i],NULL));
               }
               
           }else if(post[i][0] == '+'||post[i][0] == '-'||post[i][0] == '*'||post[i][0] == '/'||post[i][0] == '^'){
               float f1,f2,f;
               f2 = popfloat();
               f1 = popfloat();
               f = op(f1,f2,post[i][0]);
               pushfloat(f);
           }
       }
       val = popfloat();
       return val;
    }
};
