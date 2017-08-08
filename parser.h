/*
* parser.h
*
*  Created on: 2017年8月3日
*      Author: hyl

WeenyPHP: 500行的PHP解析器， 用C++语言开发
功能仅限于:（1）echo 功能
（2）整数变量创建，加减乘除和赋值运算
欢迎加入QQ群：619348997（PHP深入探讨）
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

// TOKEN的类型枚举
enum TokenType
{
    TokEof = 0,
    //
    // 这里为可见字符，比如 +，-，* ， /, =
    //
    TokEcho = 256,
    TokReserved = 257,
    TokId = 300, // 变量名

    TokInt = 350,  // 整数 比如语句 $a = 123 中的123
    TokFloat = 351,
    TokChar = 352,
    TokStrPure = 353, // 单引号
    TokStrComp = 354  // 双引号
};

// 词法分析器
class Lex
{
public:
    Lex();
    ~Lex();
    int tok;
    std::string avatar; // 当前的token的名字
    bool init(char* src);
    bool moveOn(); // 获取下一个Token
    
	int line; // 行
    int col;  // 列 

private:
    char* buf;
    char* ptr;
};

// 变量类（例$a = $b + 4中，$a,$b和4都是变量）
class Var; 

// 解释器
class Parser
{
public:
    bool act(char* str);

private:
    Lex l;
    std::map<std::string, Var*> vars; // 变量列表
    std::vector<Var*> tmps; // 临时变量

    bool sentence();  // 解析语句
    bool echo();      // 解析echo语句
    bool assign();    // 解析赋值语句
    Var* calculate(); // 解析计算表达式

private:
    Var* getVar(std::string name);
    Var* impBinary(char op, Var* one, Var* another); 
    void clear();
};

#endif /* PARSER_H_ */
