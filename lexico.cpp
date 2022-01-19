#include "lexico.h"

//------------------------------------------------------------------------------------------------

bool digito(char ch){return (ch>='0'&&ch<='9');}
bool letra(char x){return ((x>='a'&&x<='z')||(x>='A'&&x<='Z'));}
//-----------------------------------------------------------------------------------------------------

char Lexico::NextSimbol()
{
    thisptr++;
    if((thisptr-1)<Cant)
        return A[thisptr-1];
    return 0;
}

QString Lexico::GetLexeme()
{
    //char*caden=new char[thisptr-antptr+2];
    ///caden[0]=0;
    QString caden ;
    for(int i=antptr;i<thisptr;i++)
        if((A[i]!=' ')&&(A[i]!='\t')&&(A[i]!='\r')&&(A[i]!='\n'))
            caden.append(QChar(A[i]));
    //strncat(caden,&A[i],1);
    antptr=thisptr;
    return caden;
}



Token Lexico::NextToken()
{
    int state=0;
    char x;
    QString str;

    while(1){
        x=NextSimbol();
        //if(x==0)return Token(simbEos);
        switch (state) {
        case 0:
            switch (x) {
            case 0: antptr=thisptr;return Token(simbEos);
            case '\n':cline++;
            case ' ':
            case '\t':
            case '\r':
                break;
            case ';':antptr=thisptr;return Token(simbPComa);
            case '-':state = 1;break;
            default:                
                if(letra(x)){state=2;break;}
                else{ErrorLex();return Token(simbError);}
                break;
            }
            break;
        case 1://El simbolo anterior es el menos (-)
            if(x=='>'){
                antptr=thisptr;
                return Token(simbFlecha);
            }else{
                str = GetLexeme() ;
                frases.push_back(QString("No se esperaba token ")+str);
                return Token(simbError);
            }
            break;
        case 2:
            if((!letra(x))&&!digito(x)&&x!='_'){
                Retract();
                str = GetLexeme();
                if(str != "begin" && str != "eos")
                    return Token(simbId, str);
                frases.push_back(QString("Se prohibe utilizar el token ")+str);
                return Token(simbError);
            }
            break;
        default:
            break;
        }
    }
}

Token Lexico::NextToken1()
{
    int ant_viejo=antptr,this_viejo=thisptr,cline_viejo=cline;
    Token t_aux=NextToken();
    antptr=ant_viejo;
    thisptr=this_viejo;
    cline=cline_viejo;
    return t_aux;
}

void Lexico::cargar(const QString &fuente)
{
    int i=0;

    if(A)
        delete [] A ;

    Cant= fuente.length() + 1;
    A=new char[Cant];

    foreach (const QChar &ch, fuente) {
        A[i++] = (char)ch.cell();
    }
    A[i] = 0 ;
    antptr = thisptr = 0 ;
    cline = 1 ;
}

Token::~Token()
{
    lexeme.clear();
}
