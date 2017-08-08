/*
* main.cpp
*
*  Created on: 2017年7月23日
*      Author: hyl

WeenyPHP: 500行的PHP解析器， 用C++语言开发
功能仅限于:（1）echo 功能
            （2）整数变量创建，加减乘除和赋值运算
程序已在VS2008和cygwin2.738上测试通过
欢迎加入QQ群：619348997（PHP深入探讨）
WeenyPhp受到项目TinyJS的启发（tinyjs.sourceforge.net/index-en.html）

*/

#include "parser.h"
using namespace std;

int main(int argc, char** argv)
{
    char* src = " // test weenyphp \n echo 'hi world '; $a = 3; $b = $a*3; \
                   echo 'a is '.$a; echo \" b is $b\";";
    if(argc >= 2) // 读取文件
    {
        FILE* f = fopen(argv[1], "rb");
        if(!f)
        {
            printf("file%s open fail \n", argv[1]);
            return 0;
        }
        fseek(f, 0, SEEK_END);
        int len = ftell(f);
        fseek(f, 0, SEEK_SET);
        src = (char*)malloc(len+1);
        memset(src, 0, len+1);
        fread(src, 1, len, f);
        fclose(f);
    }
    if(1) 
    {
        Parser parser;
        parser.act(src);
    }
    else // 仅解析词法 
    { 
        Lex lex;
        int cntr = 0;
        lex.init(src);
        printf(" cntr    token   avatar \n");
        while(lex.tok > 0)
        {
            printf("%3d,     %3d,    %s \n", cntr++, lex.tok, lex.avatar.c_str());
            lex.moveOn();
        }
        printf("line:%d \n", lex.line);
    }
    if(argc >= 2)
       free(src);
    return 0;
}

