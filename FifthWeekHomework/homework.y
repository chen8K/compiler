//序言部分
%{
#include<stdio.h>
#include<stdlib.h>
int yylex();
void yyerror(char *s);
%}
//bison声明
%left '+' '-'
%left '*' '/'
%token NUM

%%
/*swith line*/
lines:line
     |line lines
;
line:exp '\n';
/*题目：
在这个题目中，你将实现一个简单的台式计算器。这个台式计算器的功能像在最后一个讲义中演示的例子一样：即用户可以在控制台上交互输入算术表达式，你的程序判断该表达式是否合法，不合法的话报错并退出运行。

你的程序涉及表达式的部分要支持如下的表达式：
E -> n
     | E + E
     | E - E
     | E * E
     | E / E
     | (E)
其中n是任意的非负整数（注意：在我们演示的例子中，n只是单个字符的整数，所以这个地方你需要做些扩展，这些扩展同时需要涉及修改词法分析yylex函数）。

如果在Linux系统上，那么bison应该是默认安装可用的；如果你需要在Windows上完成，你可以下载Windows平台上的bison：
http://staff.ustc.edu.cn/~bjhua/mooc/bison.exe
注意：1。安装目录不能包含空格汉字等特殊字符；2。安装完成后把安装目录加到环境变量中。
*/


/*exp */
exp:NUM
	|exp '+' exp
	|exp '-' exp
	|exp '*' exp
	|exp '/' exp
	|'('exp')'
;


%%
int yylex(){
	int c;
	//跳过空格和制表符
	/*skip white space */
	while((c=getchar())==' '||c=='\t');
	/*is digit*/
	//将数字转换为非负整数
	if(c>='0'&&c<='9'){
		/*rewrite c*/
		ungetc(c,stdin);
		/*yylval give exp*/
		scanf("%d",&yylval);
		return NUM;
	}
	if(c==EOF)return 0;

	return c;
}

void yyerror(char *s){
	fprintf(stderr,"%s\n",s);
}

int main(int argc,char **argv){
	yyparse();
	return 0;
}
