#ifndef TREGLA_H
#define TREGLA_H
#include <QString>
#include <QList>
#include <QHash>

class TMetaSimbolo{
public:
    QString sim;
    enum Tipo {Terminal , NoTerminal};
    Tipo tipo;

    TMetaSimbolo():tipo(Terminal){}
    TMetaSimbolo(const QString &str,Tipo tip = Terminal):sim(str), tipo(tip){}
    TMetaSimbolo(const TMetaSimbolo &s):sim(s.sim),tipo(s.tipo){}
    ~TMetaSimbolo();

    bool operator ==(const TMetaSimbolo &s){return (sim == s.sim) ;}
    bool operator !=(const TMetaSimbolo &s){return (sim != s.sim) ;}
    TMetaSimbolo operator =(const TMetaSimbolo & s){ sim = s.sim ; tipo = s.tipo ; return s ;}
};


class TRegla
{

public:    
    TMetaSimbolo noTerm;
    QList<TMetaSimbolo> produccion ;

    TRegla(){}
    virtual ~TRegla(){produccion.clear();}
    TRegla(const TRegla & reg):noTerm(reg.noTerm), produccion(reg.produccion){}

    virtual bool operator ==(const TRegla &reg){
        return ( noTerm == reg.noTerm && produccion == reg.produccion ) ;
    }
    TRegla operator =(const TRegla & reg){
        noTerm = reg.noTerm ;
        produccion = reg.produccion ;
        return reg ;
    }

    QString print(){
        QString str = noTerm.sim ;
        str += "->";
        foreach (const TMetaSimbolo &s, produccion) {
            str += s.sim +" ";
        }
        return str + ";";
    }
};

class TClausuraItem
{
public:
    int punto;
    uint id ;
    TMetaSimbolo* noTerm;
    QList<TMetaSimbolo*> produccion ;

    TClausuraItem():punto(0){}
    TClausuraItem(const TClausuraItem & claus);
    ~TClausuraItem();
    TClausuraItem operator =(const TClausuraItem & claus){
        id = claus.id ;
        noTerm = claus.noTerm ;
        produccion = claus.produccion ;
        punto = claus.punto ;
        return claus;
    }
    TMetaSimbolo* actual(){ return (punto< produccion.length())?produccion[punto] : 0 ; }

    QString print(){
        QString str = noTerm->sim ;
        str += "->";
        int i = 0 ;
        foreach ( TMetaSimbolo* s, produccion) {
            if(i++ == punto)
                str+=".";
            str += s->sim +" ";
        }
        if(punto == produccion.length())
            str+=".";
        str + ";";

        return str;
    }
};

inline uint qHash(const TClausuraItem &claus){
    return 100*claus.id + claus.punto;
}

inline bool operator ==(const TClausuraItem & claus, const TClausuraItem & claus2){
    return qHash(claus) == qHash(claus2) ;
}



#endif // TREGLA_H
