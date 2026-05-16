#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSplitter>
#include <QVBoxLayout>

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr) : QMainWindow(parent)
    {
        setStyleSheet("QMainWindow { background: #1e1e1e; }");
        setAttribute(Qt::WA_DeleteOnClose);

        QWidget *central = new QWidget(this);
        QVBoxLayout *layout = new QVBoxLayout(central);
        layout->setContentsMargins(0, 0, 0, 0);

        mainSplitter = new QSplitter(Qt::Horizontal, central);
        mainSplitter->setHandleWidth(1);
        mainSplitter->setStyleSheet("QSplitter { border: none; } QSplitter::handle { background: #252526; }");

        layout->addWidget(mainSplitter);
        setCentralWidget(central);
    }

    QSplitter *splitter() const { return mainSplitter; }

private:
    QSplitter *mainSplitter;
};

#endif
