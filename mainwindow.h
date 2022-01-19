

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "highlighter.h"
#include "compilerbuilder.h"

QT_BEGIN_NAMESPACE
class QTextEdit;
QT_END_NAMESPACE

//! [0]
class MainWindow : public QMainWindow
{
    Q_OBJECT
    CompilerBuilder builder;
public:
    MainWindow(QWidget *parent = 0);

public slots:
    void build();
    void newFile();
    void openFile(const QString &path = QString());

private:
    void setupEditor();
    void setupFileMenu();
    void setupHelpMenu();

    QTextEdit *editor;
    Highlighter *highlighter;

    QTextEdit *editor2;
    Highlighter *highlighter2;

    QWidget * contenedor ;
};
//! [0]

#endif
