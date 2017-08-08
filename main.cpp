/*
* main.cpp
*
*  Created on: 2017��7��23��
*      Author: hyl

WeenyPHP: 500�е�PHP�������� ��C++���Կ���
���ܽ�����:��1��echo ����
            ��2�����������������Ӽ��˳��͸�ֵ����
��������VS2008��cygwin2.738�ϲ���ͨ��
��ӭ����QQȺ��619348997��PHP����̽�֣�
WeenyPhp�ܵ���ĿTinyJS��������tinyjs.sourceforge.net/index-en.html��

*/

#include "parser.h"
using namespace std;

int main(int argc, char** argv)
{
    char* src = " // test weenyphp \n echo 'hi world '; $a = 3; $b = $a*3; \
                   echo 'a is '.$a; echo \" b is $b\";";
    if(argc >= 2) // ��ȡ�ļ�
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
    else // �������ʷ� 
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

