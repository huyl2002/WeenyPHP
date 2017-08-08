/*
* parser.h
*
*  Created on: 2017��8��3��
*      Author: hyl

WeenyPHP: 500�е�PHP�������� ��C++���Կ���
���ܽ�����:��1��echo ����
��2�����������������Ӽ��˳��͸�ֵ����
��ӭ����QQȺ��619348997��PHP����̽�֣�
*/

#ifndef PARSER_H_
#define PARSER_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h> // va_list
#include <iostream>
#include <string>
#include <vector>
#include <map>

// TOKEN������ö��
enum TokenType
{
    TokEof = 0,
    //
    // ����Ϊ�ɼ��ַ������� +��-��* �� /, =
    //
    TokEcho = 256,
    TokReserved = 257,
    TokId = 300, // ������

    TokInt = 350,  // ���� ������� $a = 123 �е�123
    TokFloat = 351,
    TokChar = 352,
    TokStrPure = 353, // ������
    TokStrComp = 354  // ˫����
};

// �ʷ�������
class Lex
{
public:
    Lex();
    ~Lex();
    int tok;
    std::string avatar; // ��ǰ��token������
    bool init(char* src);
    bool moveOn(); // ��ȡ��һ��Token
    
	int line; // ��
    int col;  // �� 

private:
    char* buf;
    char* ptr;
};

// �����ࣨ��$a = $b + 4�У�$a,$b��4���Ǳ�����
class Var; 

// ������
class Parser
{
public:
    bool act(char* str);

private:
    Lex l;
    std::map<std::string, Var*> vars; // �����б�
    std::vector<Var*> tmps; // ��ʱ����

    bool sentence();  // �������
    bool echo();      // ����echo���
    bool assign();    // ������ֵ���
    Var* calculate(); // ����������ʽ

private:
    Var* getVar(std::string name);
    Var* impBinary(char op, Var* one, Var* another); 
    void clear();
};

#endif /* PARSER_H_ */
