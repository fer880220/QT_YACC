#ifndef LEXICO_H
#define LEXICO_H

#include "ids.h"
#include <QString>
#include <QList>

class Token
{
public:
    TSimbolo token ;
    QString lexeme ;

    Token(TSimbolo simb = simbEos):token(simb){ }
    Token(TSimbolo simb , const QString &lex): token(simb), lexeme(lex){}
    void operator = (Token tok){token = tok.token; lexeme = tok.lexeme ;}
    ~Token();
};


class Lexico
{
    int Cant;int thisptr,antptr;
    char NextSimbol();
    void Retract(){thisptr--;}
    QString GetLexeme();
    void ErrorLex(){ 	antptr=thisptr;  }
public:
    int cline;
    char *A;
    QList<QString> frases;
    Lexico(){thisptr = antptr = 0;cline=1; A = 0 ;}
    ~Lexico(){delete [] A ;frases.clear();}
    Token NextToken();
    Token NextToken1();
    void cargar(const QString &fuente);
    void reset(){thisptr = antptr = 0;cline=1; frases.clear();}
    char operator[](int indice){return A[indice];}
};

#endif // LEXICO_H
