#include "sintactico.h"

#define ERRORSINTACTICO 4000
//REGLAS SINTÁCTICAS NUMERADAS
// 0) Start -> S
// 1) S -> Reglas
// 2) Reglas -> R
// 3) Reglas -> Reglas R
// 4) R -> id flecha Der pcoma
// 5) Der -> Der id
// 6) Der ->

struct{TSimbolo noTerminal; int dim;}
DimRule[]=  {
    {simbStart, 1},
    {simbS, 1},
    {simbReglas, 1},
    {simbReglas, 2},
    {simbR, 4},
    {simbDer , 2},
    {simbDer, 0},
};

static int Action[] =
{5,	    4000,	4000,	4000,	1,	    2,	    3,	    4,	    4000,
 4000,	4000,	4000,	0,	    4000,	4000,	4000,	4000,	4000,
 4000,	4000,	4000,	0,	    4000,	4000,	4000,	4000,	4000,
 5,	    4000,	4000,	-1,	    4000,	4000,	4000,	6,	    4000,
 -2,	4000,	4000,	-2,	    4000,	4000,	4000,	4000,	4000,
 4000,	7,	    4000,	4000,	4000,	4000,	4000,	4000,	4000,
 -3,	4000,	4000,	-3,	    4000,	4000,	4000,	4000,	4000,
 -6,	4000,	-6,	    4000,	4000,	4000,	4000,	4000,	8,
 9,	    4000,	10,	    4000,	4000,	4000,	4000,	4000,	4000,
 -5,	4000,	-5,	    4000,	4000,	4000,	4000,	4000,	4000,
 -4,	4000,	4000,	-4,	    4000,	4000,	4000,	4000,	4000,

} ;
#define MAXELEM  (sizeof(Action)/sizeof(int) )
#define CTE 11   //cantidad total de estados
#define CTS  9     //cantidad total de simbolos

void Sintactico::ErrorSintactico(int est, Token t_param)
{
    fcorrecto = false ;
    Token t1=Lex.NextToken1();
    QString frase ;
    frase.setNum(Lex.cline);
    if(t_param.token!=simbError){
        frase += QString(" No se esperaba token ")+ NombresSImb[t_param.token] +
               ((t_param.lexeme != "")? (QString(" (") + t_param.lexeme + ")"):QString(""));
        frases.push_back(frase) ;

    }


    if(index(est,t1.token)!=4000 ||t_param.token==simbError)
    {
        t=Lex.NextToken();
        return;
    }
    if(t_param.token== simbEos){
        fcontinuar = false ;
        return;
    }
    while(index(Pila[top], t_param.token)==4000 && top>=0) top--;
    if(top>=0) return;
    firstNewState(t_param.token,top,Pila);
}

int Sintactico::index(int estado, TSimbolo simbolo)
{
    if(estado>-1 && estado<CTE && simbolo != simbError)
      return Action[estado*CTS+ simbolo];
    return  ERRORSINTACTICO;
}

void Sintactico::origen(int q, int &tope, int *Stack_param)
{
    int *res=new int[CTE];
    tope=0;
    res[tope]=q;
    //----------------------------------
    int est=q,i;
    while(est!=0)
    {
       for(i=0;i<q*CTS && Action[i] != est ;i++);
       est=i/CTS;
       res[++tope]=est;
    }
    for(int k=0;k<=tope;k++)
        Stack_param[k]=res[tope-k];
}

void Sintactico::firstNewState(TSimbolo simb, int &tope, int *Stack_param)

{
     int i;
     for(i=0;index(i,simb)==4000 && i<CTE;i++);
     origen(i,tope,Stack_param);
}



void Sintactico::ExecSemRule(int regla)
{
    if(!fcorrecto)
        return ;


    QString str;
    static TRegla reglaP;
    int indice = -1 ;



    switch (regla) {
    case 0:// 0) Start -> S

        break;
    case 1:// 1) S -> Reglas

        break;
    case 2:// 2) Reglas -> R
        reglas.clear();
        reglas.push_back(reglaP);
        break;
    case 3:// 3) Reglas -> Reglas R
        if(reglas.indexOf(reglaP) == -1){//no esta la regla
            reglas.push_back(reglaP);
        }else{//Ya esta la regla
          ErrorSem(QString("se repite la regla ")+reglaP.print());
        }
        break;
    case 4:// 4) R -> id flecha Der pcoma
        str = *((QString*)Sem[tope - 1]);
        ((QString*)Sem[tope- 1])->~QString();

        reglaP.noTerm = TMetaSimbolo(str);
        reglaP.produccion = *((QList<TMetaSimbolo>*)Sem[tope]);
        ((QList<TMetaSimbolo>*)Sem[tope])->~QList();
        tope -= 2;

        indice = tablaSimb.indexOf(reglaP.noTerm);

        if(indice >=0)
            tablaSimb[indice].tipo = TMetaSimbolo::NoTerminal ;

        break;
    case 5:// 5) Der -> Der id
        str = *((QString*)Sem[tope]);
        ((QString*)Sem[tope--])->~QString();
        ((QList<TMetaSimbolo>*)Sem[tope])->push_back(TMetaSimbolo(str));
        break;
    case 6:// 6) Der ->
        Sem[++tope] = new QList<TMetaSimbolo> ;
        break;
    default:
        break;
    }
}

int Sintactico::Parser()
{
    tope=top=0;
    fcorrecto=fcontinuar=true;
    Lex.reset();
    tablaSimb.clear();
    tablaSimb.push_back(TMetaSimbolo(QString("eos")));
    Sem[0]=0;
    int iterador=0;
    t=Lex.NextToken();
    int est_actual,action,temporal;
    Pila[top]=0;
    while(fcontinuar)
    {   iterador++;
        est_actual=Pila[top];
        action=index(est_actual,t.token);

        if(action>0 && action!=4000)//shift
        {
            if(fcorrecto && (t.token==simbId)){
                Sem[++tope]= new QString(t.lexeme);
                if(tablaSimb.indexOf(TMetaSimbolo(t.lexeme)) == -1){//no esta el simbolo
                    tablaSimb.push_back(TMetaSimbolo(t.lexeme));
                }
            }

            Pila[++top]=action;
            t=Lex.NextToken();
        }
        else if(action <= 0)//reduce
        {
            top-=DimRule[abs(action)].dim;
            temporal=index(Pila[top],DimRule[abs(action)].noTerminal);
            ExecSemRule( -1 * action );
            Pila[++top]=temporal;
            if(action == 0){
                if(fcorrecto)
                    frases.push_back(QString("compilación exitosa!!"));
                frases +=Lex.frases;
                return fcorrecto;
            }
        }
        else //hay un error
        {
            ErrorSintactico(est_actual,t);

        }
    }
    return fcorrecto;
}

Sintactico::~Sintactico()
{
    reglas.clear();
    tablaSimb.clear();
    frases.clear();
}


