#include <QtGui>
#include <QTextEdit>
#include<QMessageBox>
#include<QFileDialog>
#include<QMenu>
#include<QMenuBar>
#include<QLayout>

#include "sintactico.h"


#include "mainwindow.h"




//! [0]
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{


    setupFileMenu();
    setupHelpMenu();
    setupEditor();





    setCentralWidget(contenedor);
    setWindowTitle(tr("QT YACC"));
}
//! [0]

void MainWindow::build()
{
    Sintactico sinta;
    QString strResultante, aux;
    sinta.Lex.cargar(editor->toPlainText());
    editor2->clear();
    if(!sinta.Parser()){
        foreach (const QString &iter, sinta.frases) {
            strResultante += iter +" \n";
        }
        editor2->setText(strResultante);
        return;
    }


    //buena compilacion

     builder.cargar(&(sinta.tablaSimb) ,&(sinta.reglas));

    editor2->append(CadSalidas[0]);
    int i = 0 ;
    foreach (const TEstado &estIter, builder.estadosSet) {
        aux = CadSalidas[1]+ QString::number(i++) +CadSalidas[2] +printEstado(estIter) ;
        editor2->append(aux);
    }
    editor2->append("\n\n   //  REGLAS DE PRODUCCION: \n");
    foreach (TRegla  reg, builder.reglas) {
        //strResultante +=reg.print()+QString("\n");
        editor2->append(reg.print());
    }
    strResultante ="\n\n//   NO TERMINALES: \n{ ";
    for(int i = 0 ; i < builder.noTermDim ; i++)
        strResultante +=builder.tablaSimb[ builder.noTerminales[i] ].sim + " , ";
    strResultante +=" } ";
    editor2->append(strResultante);

    strResultante ="\n\n   //TERMINALES: \n{ ";
    foreach (TMetaSimbolo sim, builder.tablaSimb) {
        if(sim.tipo == TMetaSimbolo::Terminal)
            strResultante +=sim.sim + " , ";
    }    
    strResultante +=" }";
    editor2->append(strResultante);

    //Imprimiendo la lista de First
    strResultante ="\n\n   //CONJUNTOS FIRST: \n";
    for(int i = 0 ; i < builder.noTermDim ; i++){
        TMetaSimbolo sim = builder.tablaSimb.operator [](builder.noTerminales[i]);
        strResultante +="first( " + sim.sim + ") = { ";
        foreach (int kk, builder.first(sim)) {
            sim = builder.tablaSimb.operator [](kk);

            strResultante +=sim.sim + " , ";
        }
        strResultante +=" }\n";
    }
    editor2->append(strResultante);
    //Fin de la lista de first

    //Imprimiendo la lista de Follow
    strResultante ="\n    //CONJUNTOS FOLLOW \n \n";
    for(int i = 0 ; i < builder.noTermDim ; i++){
        TMetaSimbolo sim = builder.tablaSimb[builder.noTerminales[i]];
        strResultante +="follow( "+ sim.sim + ") = { ";
        foreach (int kk, builder.follow(sim)) {
            sim = builder.tablaSimb[kk];

            strResultante += sim.sim + " , ";
        }
        strResultante += " }\n";
    }
    editor2->append(strResultante);

    strResultante.clear();
    if(builder.ambiguedadesList.length()){//hay ambigüedades, no se debe continuar
        strResultante = "\n\n//HAY CELDAS MULTIEVALUADAS: \n";
        foreach(const QString &strIter, builder.ambiguedadesList){
            strResultante+= strIter + "\n";
        }
        editor2->append(strResultante);
        builder.reset();

        return;
    }

    editor2->append( "\n\n\n CODIGO PARA AGREGAR AL PROYECTO: \n\n ids.h \n"
            "//////////////////////////////////////////////////// \n"
            "#include <QString> \n  typedef enum {simbError = -1 ");

    foreach (const TMetaSimbolo &simb, builder.tablaSimb) {
        aux = simb.sim;
        aux[0] = aux[0].toUpper();
        strResultante += " , simb" +aux;
    }
    strResultante += " }TSimbolo ; "
            "\n\n static QString NombresSImb[]={";
    foreach (const TMetaSimbolo &simb, builder.tablaSimb) {
        strResultante += "\"" +simb.sim+ "\" ,";
    }

    strResultante.chop(1);//quita los ultimos 1 elems
    strResultante += "};\n\nsintacticoSubclase.cpp\n"
            "//////////////////////////////////////////////////\n"
            "En el consructor de SintacticoSubclase\n"
            "DimRule = new TInfoRule[ " +QString::number( builder.reglas.length()) + " ] ; \n";
    i = 0 ;
    foreach (TRegla  reg, builder.reglas) {
        aux = reg.noTerm.sim;
        aux[0] = aux[0].toUpper();
        strResultante += "DimRule[ "+QString::number(i++) + "] = { simb" +aux+", "+QString::number(reg.produccion.length())+
                " };//"+reg.print()+"\n";
    }
    strResultante +="};\n\n\nstatic int Action[] ={\n";
    for(int i = 0 ; i < builder.filas ; i++){
        for(int j = 0 ; j < builder.columnas ; j++)
            strResultante += QString::number(builder.actionTable[i][j])+ " , ";
        strResultante +="\n";
    }
    //para escribir en fichero

    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),
                                                    "",
                                                    tr("Text File (*.txt)"));
    QFile file(fileName);
    file.open(QFile::WriteOnly | QFile::Text);
    QTextStream out(&file);
    for(int i = 0 ; i < builder.filas ; i++)
        for(int j = 0 ; j < builder.columnas ; j++)
            out<<builder.actionTable[i][j]<<endl;


    strResultante += "};\n\n#define MAXELEM  (sizeof(Action)/sizeof(int) )\n"
            "#define CTE "+QString::number(builder.estadosSet.size())+
            "   //cantidad total de estados\n"+
            "#define CTS  "+QString::number(builder.tablaSimb.length())+
                                            "   //cantidad total de simbolos\n\n\n";

    strResultante += "void Sintactico::ExecSemRule(int regla){\n"
            "if(!fcorrecto)\n  return ; \n\n switch (regla) { \n";
    i = 0 ;
    foreach (TRegla  reg, builder.reglas) {

        strResultante += "   case " +QString::number(i++)+"://"+reg.print()+"\n\n   break;\n";

    }
    strResultante +="   default:\n   break;\n\t}\n}";

    editor2->append(strResultante);
    builder.reset();


}

void MainWindow::newFile()
{
    editor->clear();
}

void MainWindow::openFile(const QString &path)
{
    QString fileName = path;

    if (fileName.isNull())
        fileName = QFileDialog::getOpenFileName(this,
                                                tr("Open File"));

    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly | QFile::Text))
            editor->setPlainText(file.readAll());
    }
}

//! [1]
void MainWindow::setupEditor()
{
    QFont font;
    font.setFamily("Courier");
    font.setFixedPitch(true);
    font.setPointSize(10);

    contenedor = new QWidget(this);
    //contenedor->setWindowTitle(QString(""));

    editor = new QTextEdit(contenedor);
    editor->setFont(font);
    highlighter = new Highlighter(editor->document());

    editor2 = new QTextEdit(contenedor);
    editor2->setFont(font);
    highlighter2 = new Highlighter(editor2->document());

    QVBoxLayout *layout = new QVBoxLayout;

    layout->addWidget(editor);
    layout->addWidget(editor2);

    contenedor->setLayout(layout);





}
//! [1]

void MainWindow::setupFileMenu()
{
    QMenu *fileMenu = new QMenu(tr("&File"), this);
    menuBar()->addMenu(fileMenu);

    fileMenu->addAction(tr("&New"), this, SLOT(newFile()),
                        QKeySequence::New);

    fileMenu->addAction(tr("&Open..."), this, SLOT(openFile()),
                        QKeySequence::Open);

    fileMenu->addAction(tr("E&xit"), qApp, SLOT(quit()),
                        QKeySequence::Quit);
}

void MainWindow::setupHelpMenu()
{
    QMenu *helpMenu = new QMenu(tr("&Action"), this);
    menuBar()->addMenu(helpMenu);

    helpMenu->addAction(tr("&Build"), this, SLOT(build()),
                        QKeySequence::Bold);

}
