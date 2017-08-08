/*
* parser.cpp
*
*  Created on: 2017年8月3日
*      Author: hyl
*/

#include "parser.h"
using namespace std;

#define Wrong  wrong(__FILE__, __FUNCTION__, __LINE__, &l)
// 打印错误信息
bool wrong(const char* file, const char* func, int line, Lex* l)
{
    printf("Wrong：%s() line%d, lex.line%d, col%d, tk%d \n", 
		func, line, l->line, l->col, l->tok);
    return false;
};

// 过滤不可见字符、空格键(0x20)和注释等
char* filter(char* in)
{
    while(1)
    {
        while(*in && (unsigned char)*in <= 0x20)
            in++;
        if(in[0] == '/' && in[1] == '/') // 过滤 // 开头的注释
        {
            while(*in && *in != '\n')
                in++;
            in++;
            continue; 
        }
        if(in[0] == '/' && in[1] == '*') // 过滤 /* */ 型注释
        {
            in += 2;
            while(in[0] && (in[0] != '*'|| in[1] != '/'))
                in++;
            in += 2;
            continue;
        }
        break;
    }
    return in;
}

bool isNumberic(char ch) 
{ 
    return (ch >= '0' && ch <='9');
}

bool isAlpha(char ch)
{
    return ((ch>='a') && (ch<='z')) || ((ch>='A') && (ch<='Z')) || ch=='_';
}

int small(int x, int y)
{
    if(x <= y)
        return x;
    return  y;
}

Lex::Lex()
{
    buf = 0;
    ptr = buf;
    tok = 0;
    line = 0;
    col = 0;
}

Lex::~Lex()
{
    if(buf)
    {
        free(buf);
        buf = 0;
    }
}

bool Lex::init(char* src)
{
    int len = strlen(src);
    if(buf)
    {
        free(buf);
        buf = 0;
    }
    buf = new char[len + 1];
    memcpy(buf, src, len);
    buf[len] = '\0';
    ptr = buf;
    tok = 0;
    line = 0;
    col = 0;
    return moveOn();
}

// 获取下一个TOKEN, 返回值表示是否结束
bool Lex::moveOn()
{
    avatar.clear();
    ptr = filter(ptr);
    char ch = *ptr;
	    tok = TokEof;
    if(ch == 0)
        return false;
	col++;
    if(isAlpha(ch))
    {
        while(isAlpha(ch) || isNumberic(ch))
        {
            avatar += ch;
            ch = *(++ptr);
        }
        if(avatar == "echo")
            tok = TokEcho;
        else
            tok = TokReserved; // if, else, while and so on.
    }
    else if(ch == '$')
    {
        ch = *(++ptr);
        if(isAlpha(ch))
        {
            while(isAlpha(ch) || isNumberic(ch))
            {
                avatar += ch;
                ch = *(++ptr);
            }
            tok = TokId; // 变量
        }
    }
    else if(isNumberic(ch))
    {
        while(isNumberic(ch))
        {
            avatar += ch;
            ch = *(++ptr);
        }
        tok = TokInt;
    }
    else if(ch == '\'')
    {
        ch = *(++ptr);
        while(ch != '\'')
        {
            avatar += ch;
            ch = *(++ptr);
        }
        ptr++;
        tok = TokStrPure;
    }
    else if(ch == '\"')
    {
        ch = *(++ptr);
        while(ch != '\"')
        {
            avatar += ch;
            ch = *(++ptr);
        }
        ptr++;
        tok = TokStrComp;
    }
    else // 单个字符
    {
        avatar += ch;
        tok = ch;
        ptr++;
        if(ch == ';')
        {
            line++;
            col = 0;
        }
    }
    return true;
}

// 变量类型
enum VarType
{
    TypeUndefined = 0,
    TypeInt = 1,
    TypeFloat = 2,
    TypeString = 3
};

// 变量类型
class Var
{
public:
    Var();
    VarType type;
    int nValue;
    float fValue;
    std::string strValue;
    std::string avatar; // 变量名字
};

Var::Var()
{
    type = TypeUndefined;
    nValue = 0;
    fValue = 0.;
}

// 解析PHP脚本
bool Parser::act(char* src)
{
    if(!l.init(src))
        return Wrong;
    while(l.tok)
    {
        sentence();
        l.moveOn();
    }
    clear();
    return true;
}

// 解析语句
bool Parser::sentence()
{
    if(l.tok == TokEcho)
    {
        l.moveOn();
        return echo();
    }
    else if(l.tok == TokId)
    {
        return assign();
    }
    else
    {
        if(l.tok != ';')
            return Wrong;
    }
    return true;
}

bool Parser::echo()
{
    while(l.tok && l.tok != ';')
    {
        if(l.tok == TokStrPure)
        {
            printf("%s", l.avatar.c_str());
            l.moveOn();
        }
        else if(l.tok == TokStrComp)
        {
            char str[200], *p;
            string name;
            memset(str, 0, sizeof(str));
            int len = small(sizeof(str) - 1, l.avatar.size());
            memcpy(str, l.avatar.c_str(), len);
            p = str;
            while(*p != '\0')
            {
                if(*p != '$')
                {
                    printf("%c", *p);
                }
                else
                {
                    name.clear();
                    p++;
                    while(*p != ' ' && *p != '\0' && *p != ';')
                    {
                        name += *p;
                        p++;
                    }
                    // 查看该ID是否存在
                    map<string, Var*>::iterator it;
                    it = vars.find(name);
                    if(it != vars.end())
                        printf("%d", vars[name]->nValue);
                    else
                        printf("$%s", name.c_str());
                    if(*p == ' ')
                        printf(" ");
                }
                p++;
            }
            l.moveOn();
        }
        else if(l.tok == TokId)
        {
            map<string, Var*>::iterator it;
            it = vars.find(l.avatar);
            if(it != vars.end())
				printf("%d", vars[l.avatar]->nValue);
            else
                printf("$%s", l.avatar.c_str());
            l.moveOn();
        }
        else if(l.tok == '.')
        {
            l.moveOn();
            continue;  
        }
        else 
        {
            return Wrong;
        }
    }
    return true;
}

// 赋值
bool Parser::assign()
{
    Var *dst, *src;
    dst = getVar(l.avatar);
    l.moveOn();
    if(l.tok != '=')
        return false;
    l.moveOn();
    src = calculate();
    if(src)
    {
        *dst = *src; 
        return true;
    }
    return Wrong;
}

// 解析（计算）表达式 目前（2017.07）支持加减乘除
Var* Parser::calculate() 
{
    Var *ele1 = 0, *ele2;
    int op;
    if(l.tok == TokId)
    {
        ele1 = getVar(l.avatar);
    }
    else if(l.tok == TokInt)
    {
        ele1 = new Var;
        tmps.push_back(ele1);
        ele1->nValue = atoi(l.avatar.c_str());
    }
    else
    {
        Wrong;
    }
    l.moveOn();
    if(l.tok == '+' || l.tok == '-')
    {
        op = l.tok;
        l.moveOn();
        ele2 = calculate();
        return impBinary(op, ele1, ele2);
    }
    if(l.tok == '*' || l.tok == '/') 
    {
        // 循环操作，直到遇到低一级操作符
        while((l.tok == '*' || l.tok == '/'))
        {
            op = l.tok;
            l.moveOn();
            if(l.tok == TokId)
            {
                ele2 = getVar(l.avatar);
            }
            else if(l.tok == TokInt)
            {
                ele2 = new Var;
                tmps.push_back(ele2);
                ele2->nValue = atoi(l.avatar.c_str());
            }
            else
            {
                Wrong;
            }
            ele1 = impBinary(op, ele1, ele2);
            l.moveOn();
            continue;
        }
        if(l.tok == '+' || l.tok == '-')
        {
            op = l.tok;
            l.moveOn();
            ele2 = calculate();
            return impBinary(op, ele1, ele2);
        } 
    }
    else if(l.tok == ';')
    {
        return ele1;
    }
    else
    {
        Wrong;
    }
    return ele1;
}

// 获取或者创建变量
Var* Parser::getVar(std::string name)
{
    Var* r;
    map<string, Var*>::iterator it;
    it = vars.find(l.avatar);
    if(it == vars.end())
    {
        r = new Var;
        vars[l.avatar] = r;
    }
    else
    {
        r = vars[l.avatar];
    }
    return r;
}

// 二元运算的实现
Var* Parser::impBinary(char op, Var* one, Var* another) 
{
    Var* r = new Var;
    tmps.push_back(r);
    int a = one->nValue, b = another->nValue;
    switch(op)
    {
    case '+': r->nValue = a + b;  break;
    case '-': r->nValue = a - b;  break;
    case '*': r->nValue = a * b;  break;
    case '/': r->nValue = a / b;  break;
    default: Wrong;
    }
    return r;
}

void Parser::clear()
{
    map<string, Var*>::iterator it;
    for(it=vars.begin(); it!=vars.end(); it++)
        free(it->second);
    vars.clear();
    vector<Var*>::iterator it2;
    for(it2=tmps.begin(); it2<tmps.end(); it2++)
        free(*it2);
    tmps.clear();
}

