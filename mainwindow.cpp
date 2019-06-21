#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new QTableWidget();
    table->setColumnCount(30);
    table->setRowCount(1000);
    setCentralWidget(table);

    createActions();
    createMenues();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();

    setCurrentFile("");
}

MainWindow::~MainWindow()
{
    delete ui;
    delete table;
}

// Private methods
void MainWindow::createActions()
{

}

void MainWindow::createMenues()
{

}

void MainWindow::createContextMenu()
{

}

void MainWindow::createToolBars()
{

}

void MainWindow::createStatusBar()
{

}

void MainWindow::readSettings()
{

}

void MainWindow::setCurrentFile(const QString &fileName)
{

}

// Private slots
void MainWindow::newFile()
{

}

void MainWindow::open()
{

}

bool MainWindow::save()
{
    return true;
}

bool MainWindow::saveAs()
{
    return true;
}

bool MainWindow::find()
{
    return true;
}

void MainWindow::goToCell()
{

}

void MainWindow::sort()
{

}

void MainWindow::about()
{

}

void MainWindow::openRecentFile()
{

}

void MainWindow::updateStatusBar()
{

}

void MainWindow::spreadSheetModified()
{

}

void MainWindow::closeEvent(QCloseEvent *)
{

}
