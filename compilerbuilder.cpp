#include "compilerbuilder.h"
#include "ids.h"



/*QList<int> CompilerBuilder::first1(const TMetaSimbolo &simb)
{
    QList<int> resultado;
    TMetaSimbolo simb1 = simb;
    int j,i = 0 ;
    j = tablaSimb.indexOf(simb);

    foreach ( TRegla iReg, reglas) {
        if(simb1 == iReg.noTerm && !marcaRule[i]){
            marcaRule[i] = 1;
            if(iReg.produccion.length() == 0 && (resultado.indexOf(0) == -1)){//Es una exilon-produccion
                resultado.push_back(0);
            }else{//Puede haber terminales y no terminales
                j = 0 ;
                TMetaSimbolo sim = (tablaSimb)[tablaSimb.indexOf(iReg.produccion[j])];
                while( sim.tipo == TMetaSimbolo::NoTerminal){
                    if(!(sim == simb))
                        foreach (int meta, first1(iReg.produccion[j]) ) {
                            if(resultado.indexOf(meta) == -1 && meta != 0)
                                resultado.push_back(meta);
                        }
                    if(tieneEProd(iReg.produccion[j]) && j < ( iReg.produccion.length() - 1 ) )
                        sim = (tablaSimb)[tablaSimb.indexOf(iReg.produccion[++j])];
                    else
                        break;
                }
                if(j<iReg.produccion.length() && sim.tipo == TMetaSimbolo::Terminal &&
                        (resultado.indexOf(tablaSimb.indexOf(iReg.produccion[j])) == -1)){
                    resultado.push_back(tablaSimb.indexOf(iReg.produccion[j]));
                }
            }
        }
        i++;
    }
    return resultado ;
}*/

QList<int> CompilerBuilder::follow1(const TMetaSimbolo &simb)
{
    QList<int> resultado;
    int j,i = 0 ;

    TMetaSimbolo simb1 = simb;

    j = tablaSimb.indexOf(simb);
    if(marcaSimb[j])
        return resultado ;

    marcaSimb[j] = true ;

    if(simb1 == reglas.at(0).noTerm)//el primero
        resultado.push_back(0);

    foreach ( TRegla iReg, reglas) {
        j = 0 ;
        TMetaSimbolo sim ;
        while(j < iReg.produccion.length()){
            sim = (tablaSimb)[tablaSimb.indexOf(iReg.produccion[j])];
            if(sim == simb){
                if( j == iReg.produccion.length() -1 ){//es el ultimo elem
                    if( simb1!=iReg.noTerm)//y no esta en Antecedente
                        foreach (int meta, follow1(iReg.noTerm) ) {
                            if(resultado.indexOf(meta) == -1)
                                resultado.push_back(meta);
                        }
                }else{//no el el ult.
                    sim = (tablaSimb)[tablaSimb.indexOf(iReg.produccion[j+1])];

                    if(sim.tipo == TMetaSimbolo::Terminal){//Le sigue un terminal
                        if(resultado.indexOf(tablaSimb.indexOf(sim)) == -1)//y no esta
                            resultado.push_back(tablaSimb.indexOf(sim));
                    }else{//le sigue un No terminal
                        foreach (int meta, first(sim) ) {
                            if(resultado.indexOf(meta) == -1 && meta != 0)
                                resultado.push_back(meta);
                        }
                        if(tieneEProd(sim))
                            foreach (int meta, follow1(sim) ) {
                                if(resultado.indexOf(meta) == -1)
                                    resultado.push_back(meta);
                            }
                    }

                }
            }
            j++;
        }
        i++;
    }

    return resultado ;
}

void CompilerBuilder::buildFirstList()
{
    QList<TRegla> reglasTerm = reglas;
    TRegla regIter, regI2;
    bool fContinuar = true ;
    while(fContinuar){
        fContinuar = false ;
        foreach (  regIter, reglasTerm)
            if(regIter.produccion.length() == 0 ||
                    regIter.produccion[0].tipo == TMetaSimbolo::Terminal ){//es una epsilon-prod o cominza con terminal
                //se sustrae el no terminal de todos los comienzos de prod donde aparece o
                //hay un term al comienzo y se sustituye en los comienzos donde aparece
                for(int k = 0 ; k < reglasTerm.length(); k++){
                    regI2 = reglasTerm[k];
                    if(regI2.produccion.length() && regI2.produccion[0] == regIter.noTerm){
                        TRegla nuevaR = regI2;
                        if(regIter.produccion.length() == 0)
                            nuevaR.produccion.removeFirst();
                        else
                            nuevaR.produccion[0] = regIter.produccion[0];
                        if(!reglasTerm.contains(nuevaR)){
                            reglasTerm.push_back(nuevaR);
                            fContinuar = true ;
                        }
                    }
                }

            }
    }
    for(int i = 0 ; i < noTermDim ; i++){
        foreach ( regIter, reglasTerm)
            if(regIter.noTerm == tablaSimb[noTerminales[i]] ){
                if(regIter.produccion.length() == 0 ||
                        regIter.produccion[0].tipo == TMetaSimbolo::Terminal){
                    int simIndex =  (regIter.produccion.length() == 0)? 0:
                                                                        tablaSimb.indexOf(regIter.produccion[0]);
                    if(!firstList[noTerminales[i]].contains(simIndex))
                        firstList[noTerminales[i]].push_back(simIndex);
                }
            }
    }
}

bool CompilerBuilder::tieneEProd( TMetaSimbolo simb)
{
    foreach (const TRegla &iReg, reglas) {
        if(simb == iReg.noTerm && iReg.produccion.length() == 0 )//Es una exilon-produccion
            return true;
    }
    return false ;
}

/*int CompilerBuilder::indexOfEstado(const TEstado &est)
{
    int ind = 0;
    foreach (const TEstado estIter, estadosSet) {
        if(est.size() == estIter.size())
            foreach (TClausuraItem clausIter, estIter) {

            }
    }
}*/

CompilerBuilder::CompilerBuilder()
{

}

void CompilerBuilder::cargar(QList<TMetaSimbolo> *tablaSimb1, QList<TRegla> *reglas1)

{
    tablaSimb = *tablaSimb1;
    reglas = *reglas1;

    int contador = 0, i = 0 , tablaSimbLen = tablaSimb.length();

    TClausuraItem itemCl;
    uint id = 0;


    for(int k = 0 ; k < reglas.length() ; k++){

        itemCl.noTerm = &((tablaSimb)[tablaSimb.indexOf(reglas[k].noTerm)]);
        itemCl.id = id++;
        reglas[k].noTerm.tipo = TMetaSimbolo::NoTerminal ;
        TMetaSimbolo *s;
        for(int kk = 0 ; kk < reglas[k].produccion.length();kk++){
            s = &reglas[k].produccion[kk];
            itemCl.produccion.push_back(&((tablaSimb)[tablaSimb.indexOf(*s)]));
            s->tipo = tablaSimb[tablaSimb.indexOf(*s)].tipo ;
        }

        clausurasList.push_back(itemCl);
        itemCl.produccion.clear();
    }

    static QList<int> auxL;
    auxL.clear();
    i = 0 ;
    foreach (const TMetaSimbolo &meta, tablaSimb) {

        if(meta.tipo == TMetaSimbolo::NoTerminal){
            contador++;
            auxL.push_back(i);
        }
        i++;
    }

    noTerminales = new int[contador];
    noTermDim = contador ;
    marcaSimb = new bool[tablaSimb.length()];
    i = 0 ;
    foreach (int j, auxL) {
        noTerminales[i++] = j ;
    }

    auxL.clear();

    marcaRule = new bool[reglas.length()];

    firstList = new QList<int> [tablaSimbLen];
    followList = new QList<int> [tablaSimbLen];

    buildFirstList();

    for(int i = 0 ; i < tablaSimbLen ; i++){
        TMetaSimbolo sim = tablaSimb[i];
        if(sim.tipo == TMetaSimbolo::NoTerminal)
            followList[i] = followOnce(sim);
    }

    buildTable();

}

void CompilerBuilder::reset()
{
    delete []noTerminales;// = new int[contador];

    delete []marcaSimb;// = new bool[tablaSimb.length()];
    delete []marcaRule;// = new bool[reglas.length()];

    //firstList = new QList<int> [tablaSimbLen];
    //followList = new QList<int> [tablaSimbLen];
    for(int i = 0 ; i < tablaSimb.length() ; i++){
        followList[i].clear();
        firstList[i].clear();
    }

    //filas = estadosSet.length();
    //columnas = tablaSimb.length();

    //actionTable = new int*[filas];
    for(int i = 0 ; i < filas ; i++)
        delete []actionTable[i] ;//= new int[columnas];
    delete [] actionTable;

    TEstado estIter;
    foreach (estIter, estadosSet){
        estIter.clear();
    }
    estadosSet.clear();
    ambiguedadesList.clear();
    clausurasList.clear();
    reglas.clear();
    tablaSimb.clear();
}

CompilerBuilder::~CompilerBuilder()
{
    delete []noTerminales;// = new int[contador];

    delete []marcaSimb;// = new bool[tablaSimb.length()];
    delete []marcaRule;// = new bool[reglas.length()];

    //firstList = new QList<int> [tablaSimbLen];
    //followList = new QList<int> [tablaSimbLen];
    for(int i = 0 ; i < tablaSimb.length() ; i++){
        followList[i].clear();
        firstList[i].clear();
    }

    //filas = estadosSet.length();
    //columnas = tablaSimb.length();

    //actionTable = new int*[filas];
    for(int i = 0 ; i < filas ; i++)
        delete []actionTable[i] ;//= new int[columnas];
    delete [] actionTable;

    TEstado estIter;
    foreach (estIter, estadosSet){
        estIter.clear();
    }
    estadosSet.clear();
    ambiguedadesList.clear();
    clausurasList.clear();
    reglas.clear();
    tablaSimb.clear();
}

/*QList<int> CompilerBuilder::firstOnce(const TMetaSimbolo &simb)
{
    for(int i = 0 ; i < reglas.length() ; i++)
        marcaRule[i] = 0 ;

    return first1(simb) ;
}*/

QList<int> CompilerBuilder::followOnce(const TMetaSimbolo &simb)
{
    int len = tablaSimb.length();
    for(int i = 0 ; i < len ; i++)
        marcaSimb[i] = 0 ;

    return follow1(simb) ;
}

TEstado CompilerBuilder::clausura0()
{
    TEstado resultado;

    resultado.insert(clausurasList[0]);

    return clausura(resultado);
}

TEstado CompilerBuilder::clausura(const TEstado &estado)
{
    TEstado resultado = estado;
    TClausuraItem itemCl, itemAux;
    int size1 = 0,size2= estado.size() ;

    while(size1 != size2){
        size1 = size2;
        foreach (itemCl, resultado) {
            TMetaSimbolo * simbAct = itemCl.actual();
            if(simbAct && (simbAct->tipo == TMetaSimbolo::NoTerminal)){
                foreach (itemAux, clausurasList) {
                    if(simbAct == itemAux.noTerm)
                        resultado.insert(itemAux);
                }
            }
        }
        size2 = resultado.size();
    }
    return resultado ;
}

TEstado CompilerBuilder::gotoFunc(const TEstado &estado,  TMetaSimbolo *pSimb)
{
    TEstado resultado;
    TClausuraItem itemAux;

    foreach (TClausuraItem itemCl, estado)
        if(itemCl.actual() && *pSimb == *(itemCl.actual()) ){
            itemAux = itemCl ;
            itemAux.punto++;
            resultado.insert(itemAux);
        }

    return clausura(resultado) ;
}

struct TAction{int estado, simbolo, accion;};
void CompilerBuilder::buildTable()
{
    TEstado estIter,estAux;
    TMetaSimbolo simbIter;

    static QList<TAction> l_action;
    static QList<int> marcasEstados;
    TAction actionItem;

    l_action.clear();
    marcasEstados.clear();

    int size1 = 0,size2= 1 , iSimbolo, iEstado, indexOf;

    estadosSet.push_back(clausura0());

    while(size1 != size2){
        size1 = size2 ;
        iEstado = 0 ;
        foreach (estIter, estadosSet){
            if(marcasEstados.indexOf(iEstado) != -1){
                iEstado++;
                continue;
            }

            marcasEstados.push_back(iEstado);
            iSimbolo = 0 ;
            foreach (simbIter, tablaSimb) {
                estAux = gotoFunc(estIter ,&simbIter);
                if(estAux.size()){
                    indexOf = estadosSet.indexOf(estAux) ;

                    actionItem.estado = iEstado ;
                    actionItem.simbolo = iSimbolo;
                    actionItem.accion = (indexOf == -1)? estadosSet.length():indexOf ;
                    l_action.push_back(actionItem);
                    if(indexOf == -1){//Es un nuevo estado
                        estadosSet.push_back(estAux);
                    }
                }
                iSimbolo++;
            }
            iEstado++;
        }
        size2 = estadosSet.length() ;
    }


    iEstado = 0 ;
    foreach (estIter, estadosSet){
        actionItem.estado = iEstado;
        foreach ( TClausuraItem claus, estIter){
            if(!claus.actual() ){//El punto esta en el final
                actionItem.accion = -1*claus.id ;
                foreach (int i, follow( *(claus.noTerm) ) ) {
                    actionItem.simbolo = i ;
                    l_action.push_back(actionItem);
                }
            }
        }
        iEstado++;
    }

    filas = estadosSet.length();
    columnas = tablaSimb.length();
    actionTable = new int*[filas];
    for(int i = 0 ; i < filas ; i++)
        actionTable[i] = new int[columnas];
    for(int i = 0 ; i < filas ; i++)
        for(int j = 0 ; j < columnas ; j++)
            actionTable[i][j] = 4000;

    foreach (const TAction &accIter, l_action) {
        QString ambiguedad;
        int casilla = actionTable[accIter.estado][accIter.simbolo];
        if( casilla != 4000){
            ambiguedad = "(I"+QString::number(accIter.estado)+", "+
                    tablaSimb[accIter.simbolo].sim+")->{"+
                    QString::number(accIter.accion)+
                    ", "+QString::number(casilla)+"}";
            ambiguedadesList.push_back(ambiguedad);
        }

        actionTable[accIter.estado][accIter.simbolo]= accIter.accion ;
    }

}


