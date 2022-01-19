#ifndef COMPILERBUILDER_H
#define COMPILERBUILDER_H

#include "tregla.h"
#include<QSet>

typedef QSet<TClausuraItem> TEstado;
typedef QList<TEstado> TEsdatoSet;

inline QString printEstado(const TEstado & estado){
    QString s="{ ";
    foreach (TClausuraItem itemCl, estado) {
        s+= itemCl.print()+", ";
    }
    s+="}";
    return s;
}



class CompilerBuilder
{
    //QList<int> first1(const TMetaSimbolo &simb);
    QList<int> follow1(const TMetaSimbolo &simb);

    void buildFirstList();

   // QList<int> firstOnce(const TMetaSimbolo &simb);
    QList<int> followOnce(const TMetaSimbolo &simb);

    bool * marcaRule;
    bool *marcaSimb;
    bool tieneEProd(TMetaSimbolo simb);

   // int indexOfEstado(const TEstado & est);
public:
    CompilerBuilder();
    void cargar(QList<TMetaSimbolo> *tablaSimb1, QList<TRegla> * reglas1 );
    void reset();
    ~CompilerBuilder();
    QList<TMetaSimbolo> tablaSimb ;
    QList<TRegla>  reglas ;

    QList<TClausuraItem> clausurasList;
    QList<int> * firstList;
    QList<int> * followList;
    int * noTerminales;
    int noTermDim;



    TEsdatoSet estadosSet;
    QList<QString> ambiguedadesList;
    int ** actionTable , filas, columnas;

    QList<int> first(const TMetaSimbolo &simb){
        int index= tablaSimb.indexOf(simb);
        return (index == -1)? QList<int>() : firstList[index] ;

    }
    QList<int> follow(const TMetaSimbolo &simb){
        int index= tablaSimb.indexOf(simb);
        return (index == -1)? QList<int>() : followList[index] ;
    }

    TEstado clausura0();
    TEstado clausura(const TEstado & estado);
    TEstado gotoFunc(const TEstado & estado, TMetaSimbolo *pSimb);

    void buildTable();

};

#endif // COMPILERBUILDER_H
