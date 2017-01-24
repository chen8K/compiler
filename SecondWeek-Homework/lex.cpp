/*题目： 
http://mooc.study.163.com/learn/USTC-1000002001?tid=1000070000#/learn/hw?id=1000135009
在这部分中，你将使用图转移算法手工实现一个小型的词法分析器。
* 分析器的输入：存储在文本文件中的字符序列，字符取自ASCII字符集。文件中可能包括下面几种记号：关键字if、符合C语言标准的标识符、无符号整型数字、空格符、回车符\n。
* 分析器的输出：打印出所识别的记号的种类、及记号开始行号、开始列号信息。
注意：1. 忽略空格及回车符；2. 对于标识符和数字，要输出符号的具体词法单元（见下面的示例）。
【示例】对于下面的文本文件：
ifx if iif       if  234
iff     if
你的输出应该是（注意，因为文本显示的原因，列号信息可能不一定准确）：
ID(ifx) (1, 1)
IF        (1, 4)
ID(iif)  (1, 8)
IF       (1, 13)
NUM(234) (1, 16)
ID(iff) (2, 1)
IF       (2, 8)
*/

/*
我完全搞错了，一开始给想错了，应该是把他们先按照字符串的形式从文件中取出，我却一个一个字符取，把整个文本文件中所有字符合起来当作了一个字符串，不过写了好久，不舍得删 
*/ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
using namespace std;
enum Exp_Kind {NUM,IF,TAG,ERROR};
typedef struct token{
	enum Exp_Kind kind;
	char ID[100];
	int col,row;
}token;

int table[12][128];//table[i][j]表示状态i通过输入j变成table[i][j] 
int target[4]={3,4,7,9};//accept状态 
int state = 0;//目前状态 
token tokens[100];
int maxTokenIndex=0;
int ROW=1;
int COL=0;
//字符的基本判断函数 
bool isAccept(int state){
	for(int i=0;i<4;i++){
		if(target[i]==state){
			return true;
		}
	}
	return false;
}
bool isCharacter(char ch){
	if((ch>='a'&&ch<='z')||(ch>='a'&&ch<='Z')){
		return true;
	}
	return false;
}
bool isNumber(char ch){
	if(ch>='0'&&ch<='9'){
		return true;
	}
	return false;
}
bool isEmptyOrNextLineOrEOF(char ch){
	if(ch==' '||ch==EOF||ch=='\n'){
		return true;
	}
	return false; 
}
bool isNumberOrChar(char ch){
	if(isNumber(ch)||isCharacter(ch)){
		return true;
	}
	return false;
}

//用于快速写入状态转换表的一些工具函数
void writeTo(bool (*p)(char),int t,int state){//将通过ASCII码表中通过p函数（指针p指向的函数）的字符（返回true的）作为int a，将table[t][a]=state 
	 for(char c=0;c<127;c++){
	 	if((*p)(c)){
	 		table[t][c]=state;	
		 }
	 }
	 if((*p)(127)){
	 	table[t][127]=state;
	 }
} 

//根据终止状态返回kind
Exp_Kind getKind(int acceptedState){
	if(acceptedState==3){
		return ERROR;
	}else if(acceptedState==4){
		return NUM;
	}else if(acceptedState==7){
		return IF;
	}else if(acceptedState==9){
		return TAG;
	}
	return ERROR;
} 

void moveFront(char* s,int len){//s中内容往前移 
	for(int i=0;i<100-len;i++){
		s[i]=s[i+len];
	}
}

void filterTokens(){//把token前面ID的空格 /n拿掉 
	for(int i=0;i<maxTokenIndex;i++){
		int index=-1;
		for(int j=0;j<5;j++){
			if(isEmptyOrNextLineOrEOF(tokens[i].ID[j])){
				index=j;//index是最后一个 空格 /n
			}else{
				break;
			}
		}
		moveFront(tokens[i].ID,index+1);
		tokens[i].col=tokens[i].col+index+1;
	}
}



//临时的函数（无重用性）
bool CharExceptI(char ch){
	if(isCharacter(ch)&&ch!='i'){
		return true;
	} 
	return false;
}
bool NumCharExceptF(char ch){
	if(isNumberOrChar(ch)&&ch!='f'){
		return true;
	} 
	return false;
}
bool isNotEmptyOrNextLineOrEOF(char ch){
	if(!isEmptyOrNextLineOrEOF(ch)){
		return true; 
	} 
	return false;
}

int main(){
	//状态转移表
	//状态0的
	table[0][' ']=0;
	table[0]['i']=5;
	writeTo(isNumber,0,1);
	writeTo(CharExceptI,0,8);
	
	//状态1
	writeTo(isNumber,1,1);
	writeTo(isEmptyOrNextLineOrEOF,1,4);
	writeTo(isCharacter,1,2);
	
	//状态2
	writeTo(isCharacter,2,2);
	writeTo(isEmptyOrNextLineOrEOF,2,3);
	
	//状态3
	//状态4
	
	//状态5
	writeTo(NumCharExceptF,5,8);
	table[5]['f']=6;
	
	//状态6
	writeTo(isEmptyOrNextLineOrEOF,6,7);
	writeTo(isNumberOrChar,6,8);
	
	//状态7

	//状态8
	writeTo(isNumberOrChar,8,8);
	writeTo(isEmptyOrNextLineOrEOF,8,9);
	
	//状态9
	
	
	FILE *file;
	file=fopen("input.txt","r");//文件末位一定要是\n 或" "  
	char ch;
	int index=0;
	
	while(true){
		ch= fgetc(file);
		
		COL++;//现在读到的char的col 
		state = table[state][ch];
		if(isAccept(state)){
			tokens[maxTokenIndex].kind=getKind(state);
			tokens[maxTokenIndex].row=ROW; 
			tokens[maxTokenIndex].col=COL-index;
			index=0;//index是现在正在写的token的ID下一个要写的index 
			maxTokenIndex++;//maxTokenIndex是正在写的token 
			state=0;
		}else{
			tokens[maxTokenIndex].ID[index++]=ch;
		} 
		if(ch=='\n'){
			ROW++;COL=0;
		}
		if(ch==EOF)break;
	}

	filterTokens();
	for(int i=0;i<maxTokenIndex;i++){
		if(tokens[i].kind==IF){
			cout<<"IF"<<"  ("<<tokens[i].row<<","<<tokens[i].col<<")"<<endl;
		}else if(tokens[i].kind==ERROR){
			cout<<"ERROR"<<"  ("<<tokens[i].row<<","<<tokens[i].col<<")"<<endl;
		}else if(tokens[i].kind==NUM){
			cout<<"NUM"<<"  ("<<tokens[i].ID<<")"<<"  ("<<tokens[i].row<<","<<tokens[i].col<<")"<<endl;
		}else if(tokens[i].kind==TAG){
			cout<<"TAG"<<"  ("<<tokens[i].ID<<")"<<"  ("<<tokens[i].row<<","<<tokens[i].col<<")"<<endl;
		}
	}
	
} 
