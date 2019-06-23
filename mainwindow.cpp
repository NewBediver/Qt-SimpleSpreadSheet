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
    connect(quitAction, SIGNAL(triggered()), this, SLOT(close()));

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

    // Delete
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/icons/icons/deleteEdit.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered()), this, SLOT(deleteEdit()));

    // Select
    selectRowAction = new QAction(tr("&Row"), this);
    selectRowAction->setStatusTip(tr("Select row"));
    connect(selectRowAction, SIGNAL(triggered()), table, SLOT(selectRowEdit()));
    selectColumnAction = new QAction(tr("&Column"), this);
    selectColumnAction->setStatusTip(tr("Select column"));
    connect(selectColumnAction, SIGNAL(triggered()), this, SLOT(selectColumnEdit()));
    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all"));
    connect(selectAllAction, SIGNAL(triggered()), table, SLOT(selectAll()));

    // Find
    findAction = new QAction(tr("&Find"),this);
    findAction->setIcon(QIcon(":/icons/icons/findEdit.png"));
    findAction->setShortcut(QKeySequence::Find);
    findAction->setStatusTip(tr("Find"));
    connect(findAction, SIGNAL(triggered()), this, SLOT(findEdit()));

    // Go to cell
    goToCellAction = new QAction(tr("&Go to cell ..."), this);
    goToCellAction->setIcon(QIcon(":/icons/icons/goToCellEdit.png"));
    goToCellAction->setShortcut(Qt::CTRL + Qt::Key_G);
    goToCellAction->setStatusTip(tr("Go to cell ..."));
    connect(goToCellAction, SIGNAL(triggered()), this, SLOT(goToCellEdit()));

    //======================= TOOLS MENU =======================//
    // Recalculate
    recalculateAction = new QAction(tr("&Recalculate"), this);
    recalculateAction->setIcon(QIcon(":/icons/icons/recalculateTools.png"));
    recalculateAction->setShortcut(Qt::Key_F9);
    recalculateAction->setStatusTip(tr("Recalculate current value"));
    connect(recalculateAction, SIGNAL(triggered()), this, SLOT(recalculateTools()));

    // Sort
    sortAction = new QAction(tr("&Sort"), this);
    sortAction->setIcon(QIcon(":/icons/icons/sortTools.png"));
    sortAction->setStatusTip(tr("Call sort dialog"));
    connect(sortAction, SIGNAL(triggered()), this, SLOT(sortTools()));

    //======================= OPTIONS MENU =======================//

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
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addAction(deleteAction);
        selectSubMenu = editMenu->addMenu(QIcon(":/icons/icons/selectEdit.png"), tr("&Select"));
        selectSubMenu->addAction(selectRowAction);
        selectSubMenu->addAction(selectColumnAction);
        selectSubMenu->addAction(selectAllAction);
    editMenu->addSeparator();
    editMenu->addAction(findAction);
    editMenu->addAction(goToCellAction);

    // TIE ACTION TO TOOLS MENU
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(recalculateAction);
    toolsMenu->addAction(sortAction);

    // TIE ACTION TO OPTIONS MENU

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

void MainWindow::deleteEdit()
{

}

void MainWindow::selectRowEdit()
{

}

void MainWindow::selectColumnEdit()
{

}

void MainWindow::selectAllEdit()
{

}

bool MainWindow::findEdit()
{
    return true;
}

void MainWindow::goToCellEdit()
{

}

void MainWindow::recalculateTools()
{

}

void MainWindow::sortTools()
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
