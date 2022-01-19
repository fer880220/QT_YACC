#ifndef SINTACTICO_H
#define SINTACTICO_H

#include <QObject>
#include "lexico.h"
#include "tregla.h"


class Sintactico: public QObject
{
    Q_OBJECT

    void ErrorSintactico( int est,Token t_param );
    void ErrorSem(const QString &frase){frases.push_back(frase);fcorrecto=false;}
    Token t;
    void *Sem[140];
    int Pila[50];
    int tope, top;

    int index(int estado,TSimbolo simbolo);
    void origen(int q,int &tope, int* Stack_param);
    void firstNewState(TSimbolo simb,int &tope,int * Stack_param);
public:
     bool fcorrecto, fcontinuar;

     Lexico Lex;

     QList<TRegla> reglas ;
     QList<TMetaSimbolo> tablaSimb;
     QList<QString> frases;
     void ExecSemRule(int regla);
     int Parser();
     Sintactico(){ fcorrecto = fcontinuar = true ;  }
     ~Sintactico();

};

#endif // SINTACTICO_H
