#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QStringList>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    table = new QTableWidget();
    table->setColumnCount(26);
    table->setRowCount(1000);
    table->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z");

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
}

// Private methods
void MainWindow::createActions()
{
    //======================= FILE MENU =======================//
    // Create new table
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/icons/icons/newFile.png"));
    newAction->setShortcut(QKeySequence::New);
    newAction->setStatusTip(tr("Create a new spreadsheet file"));
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    // Open existing table
    openAction = new QAction(tr("&Open"), this);
    openAction->setIcon(QIcon(":/icons/icons/openFile.png"));
    openAction->setShortcut(QKeySequence::Open);
    openAction->setStatusTip(tr("Open spreadsheet file"));
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    // Save existing table
    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/icons/icons/saveFile.png"));
    saveAction->setShortcut(QKeySequence::Save);
    saveAction->setStatusTip(tr("Save current existing spreadsheet"));
    connect(saveAction, SIGNAL(triggered()), this, SLOT(saveFile()));

    // Save current table
    saveAsAction = new QAction(tr("&Save as ..."), this);
    saveAsAction->setIcon(QIcon(":/icons/icons/saveAsFile.png"));
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    saveAsAction->setStatusTip(tr("Save current spreadsheet"));
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAsFile()));

    // Recent files
    for (int i = 0; i < MaxRecentFiles; ++i) {
        recentFileActions[i] = new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    // Quit
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QIcon(":/icons/icons/exitFile.png"));
    quitAction->setShortcut(Qt::CTRL + Qt::Key::Key_Q);
    quitAction->setStatusTip("Exit program");
    connect(quitAction, SIGNAL(triggered()), this, SLOT(closeFile()));

    //======================= EDIT MENU =======================//
    // Cut
    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/icons/icons/cutEdit.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), this, SLOT(cutEdit()));

    // Copy
    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/icons/icons/copyEdit.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), this, SLOT(copyEdit()));

    // Paste
    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/icons/icons/pasteEdit.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), this, SLOT(pasteEdit()));

}

void MainWindow::createMenues()
{
    // TIE ACTIONS TO FILE MENU
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);
    separatorAction = fileMenu->addSeparator();
    for (int i = 0; i < MaxRecentFiles; ++i) {
        fileMenu->addAction(recentFileActions[i]);
    }
    fileMenu->addSeparator();
    fileMenu->addAction(quitAction);

    // TIE ACTION TO EDIT MENU
    editMenu= menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);

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

void MainWindow::openFile()
{

}

bool MainWindow::saveFile()
{
    return true;
}

bool MainWindow::saveAsFile()
{
    return true;
}

void MainWindow::cutEdit()
{

}

void MainWindow::copyEdit()
{

}

void MainWindow::pasteEdit()
{

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
