/*��Ŀ�� 
http://mooc.study.163.com/learn/USTC-1000002001?tid=1000070000#/learn/hw?id=1000135009
���ⲿ���У��㽫ʹ��ͼת���㷨�ֹ�ʵ��һ��С�͵Ĵʷ���������
* �����������룺�洢���ı��ļ��е��ַ����У��ַ�ȡ��ASCII�ַ������ļ��п��ܰ������漸�ּǺţ��ؼ���if������C���Ա�׼�ı�ʶ�����޷����������֡��ո�����س���\n��
* ���������������ӡ����ʶ��ļǺŵ����ࡢ���Ǻſ�ʼ�кš���ʼ�к���Ϣ��
ע�⣺1. ���Կո񼰻س�����2. ���ڱ�ʶ�������֣�Ҫ������ŵľ���ʷ���Ԫ���������ʾ������
��ʾ��������������ı��ļ���
ifx if iif       if  234
iff     if
������Ӧ���ǣ�ע�⣬��Ϊ�ı���ʾ��ԭ���к���Ϣ���ܲ�һ��׼ȷ����
ID(ifx) (1, 1)
IF        (1, 4)
ID(iif)  (1, 8)
IF       (1, 13)
NUM(234) (1, 16)
ID(iff) (2, 1)
IF       (2, 8)
*/

/*
����ȫ����ˣ�һ��ʼ������ˣ�Ӧ���ǰ������Ȱ����ַ�������ʽ���ļ���ȡ������ȴһ��һ���ַ�ȡ���������ı��ļ��������ַ�������������һ���ַ���������д�˺þã������ɾ 
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

int table[12][128];//table[i][j]��ʾ״̬iͨ������j���table[i][j] 
int target[4]={3,4,7,9};//accept״̬ 
int state = 0;//Ŀǰ״̬ 
token tokens[100];
int maxTokenIndex=0;
int ROW=1;
int COL=0;
//�ַ��Ļ����жϺ��� 
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

//���ڿ���д��״̬ת�����һЩ���ߺ���
void writeTo(bool (*p)(char),int t,int state){//��ͨ��ASCII�����ͨ��p������ָ��pָ��ĺ��������ַ�������true�ģ���Ϊint a����table[t][a]=state 
	 for(char c=0;c<127;c++){
	 	if((*p)(c)){
	 		table[t][c]=state;	
		 }
	 }
	 if((*p)(127)){
	 	table[t][127]=state;
	 }
} 

//������ֹ״̬����kind
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

void moveFront(char* s,int len){//s��������ǰ�� 
	for(int i=0;i<100-len;i++){
		s[i]=s[i+len];
	}
}

void filterTokens(){//��tokenǰ��ID�Ŀո� /n�õ� 
	for(int i=0;i<maxTokenIndex;i++){
		int index=-1;
		for(int j=0;j<5;j++){
			if(isEmptyOrNextLineOrEOF(tokens[i].ID[j])){
				index=j;//index�����һ�� �ո� /n
			}else{
				break;
			}
		}
		moveFront(tokens[i].ID,index+1);
		tokens[i].col=tokens[i].col+index+1;
	}
}



//��ʱ�ĺ������������ԣ�
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
	//״̬ת�Ʊ�
	//״̬0��
	table[0][' ']=0;
	table[0]['i']=5;
	writeTo(isNumber,0,1);
	writeTo(CharExceptI,0,8);
	
	//״̬1
	writeTo(isNumber,1,1);
	writeTo(isEmptyOrNextLineOrEOF,1,4);
	writeTo(isCharacter,1,2);
	
	//״̬2
	writeTo(isCharacter,2,2);
	writeTo(isEmptyOrNextLineOrEOF,2,3);
	
	//״̬3
	//״̬4
	
	//״̬5
	writeTo(NumCharExceptF,5,8);
	table[5]['f']=6;
	
	//״̬6
	writeTo(isEmptyOrNextLineOrEOF,6,7);
	writeTo(isNumberOrChar,6,8);
	
	//״̬7

	//״̬8
	writeTo(isNumberOrChar,8,8);
	writeTo(isEmptyOrNextLineOrEOF,8,9);
	
	//״̬9
	
	
	FILE *file;
	file=fopen("input.txt","r");//�ļ�ĩλһ��Ҫ��\n ��" "  
	char ch;
	int index=0;
	
	while(true){
		ch= fgetc(file);
		
		COL++;//���ڶ�����char��col 
		state = table[state][ch];
		if(isAccept(state)){
			tokens[maxTokenIndex].kind=getKind(state);
			tokens[maxTokenIndex].row=ROW; 
			tokens[maxTokenIndex].col=COL-index;
			index=0;//index����������д��token��ID��һ��Ҫд��index 
			maxTokenIndex++;//maxTokenIndex������д��token 
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
