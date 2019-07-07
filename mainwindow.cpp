#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "FindDialog/finddialog.h"

#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    spr = new Spreadsheet();
    spr->setColumnCount(26);
    spr->setRowCount(999);
    setCentralWidget(spr);
    spr->setHorizontalHeaderLabels(QStringList() << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H" << "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P" << "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z");

    this->setWindowIcon(QIcon(":/icons/icons/title.png"));

    createActions();
    createMenues();
    createContextMenu();
    createToolBars();
    createStatusBar();

    readSettings();
    findDialog = nullptr;

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
    connect(selectRowAction, SIGNAL(triggered()), this, SLOT(selectRowEdit()));
    selectColumnAction = new QAction(tr("&Column"), this);
    selectColumnAction->setStatusTip(tr("Select column"));
    connect(selectColumnAction, SIGNAL(triggered()), this, SLOT(selectColumnEdit()));
    selectAllAction = new QAction(tr("&All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    selectAllAction->setStatusTip(tr("Select all"));
    connect(selectAllAction, SIGNAL(triggered()), spr, SLOT(selectAll()));

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
    // Grid
    showGridAction = new QAction(tr("&Show Grid"), this);
    showGridAction->setCheckable(true);
    showGridAction->setChecked(spr->showGrid());
    showGridAction->setIcon(QIcon(":/icons/icons/showGridOptions.png"));
    showGridAction->setStatusTip(tr("Show or hide grid"));
    connect(showGridAction, SIGNAL(toggled(bool)), spr, SLOT(setShowGrid(bool)));

    // Auto recalculate


    //======================= HELP MENU =======================//
    aboutAction = new QAction(tr("&About"), this);
    aboutAction->setIcon(QIcon(":/icons/icons/aboutHelp.png"));
    aboutAction->setStatusTip(tr("Show information about program"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(aboutHelp()));

    aboutQtAction = new QAction(tr("&About Qt"), this);
    aboutQtAction->setIcon(QIcon(":/icons/icons/avoutQtHelp.png"));
    aboutQtAction->setStatusTip(tr("Show thi Qt library's About box"));
    connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));


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
    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(showGridAction);

    menuBar()->addSeparator();

    //TIE ACTION TO HELP MENU
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
    helpMenu->addAction(aboutQtAction);
}

void MainWindow::createContextMenu()
{
    // Create context manu when clicking mouse righ button
    spr->addAction(cutAction);
    spr->addAction(copyAction);
    spr->addAction(pasteAction);
    spr->setContextMenuPolicy(Qt::ActionsContextMenu);
}

void MainWindow::createToolBars()
{
    // Create Toolbar
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addSeparator();
    editToolBar->addAction(findAction);
    editToolBar->addAction(goToCellAction);
}

void MainWindow::createStatusBar()
{
    locationLabel = new QLabel(" W999 ");
    locationLabel->setAlignment(Qt::AlignCenter);
    locationLabel->setMinimumSize(locationLabel->sizeHint());

    formulaLabel = new QLabel();
    formulaLabel->setIndent(3);
    statusBar()->addWidget(locationLabel, 0);
    statusBar()->addWidget(formulaLabel, 1);

    connect(spr, SIGNAL(currentCellChanged(int, int, int, int)), this, SLOT(updateStatusBar()));
    connect(spr, SIGNAL(modified()), this, SLOT(spreadSheetModified()));

    updateStatusBar();
}

void MainWindow::writeSettings()
{

}

void MainWindow::readSettings()
{

}

// SET CURRENT WORKING FILE AND UPDATE WINDOW TITLE
void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    setWindowModified(false);

    QString showName = "Untilted";
    if (!curFile.isEmpty()) {
        showName = strippedName(curFile);
        recentFile.removeAll(curFile);
        recentFile.prepend(curFile);
        updateRecentFileActions();
    }

    setWindowTitle(tr("%1[*] - %2").arg(showName).arg(tr("Spreadsheet")));
}

// Private slots
void MainWindow::newFile()
{
    if (okToContinue()) {
        spr->clear();
        setCurrentFile("");
    }
}

void MainWindow::openFile()
{
    if (okToContinue()) {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
        if (!fileName.isEmpty()) {
            loadFile(fileName);
        }
    }
}

bool MainWindow::saveFile()
{
    if (curFile.isEmpty()) {
        return saveAsFile();
    }
    else {
        return saveFile(curFile);
    }
}

bool MainWindow::saveFile(const QString &fileName)
{
    if (!spr->writeFile(fileName)) {
        statusBar()->showMessage(tr("Saving cancelled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}

bool MainWindow::saveAsFile()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Spreadsheet"), ".", tr("Spreadsheet files (*.sp)"));
    if (fileName.isEmpty()) {
        return false;
    }
    return saveFile(fileName);
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

void MainWindow::findEdit()
{
    if (!findDialog) {
        findDialog = new FindDialog(this);
        connect(findDialog, SIGNAL(findNext(const QString &, Qt::CaseSensitivity)), spr, SLOT(findNext(const QString &, Qt::CaseSensitivity)));
        connect(findDialog, SIGNAL(findPrevious(const QString &, Qt::CaseSensitivity)), spr, SLOT(findPrevious(const QString &, Qt::CaseSensitivity)));
    }
    findDialog->setModal(false);
    findDialog->show();

    findDialog->raise();
    findDialog->activateWindow();
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

void MainWindow::showGridOptions()
{

}

void MainWindow::aboutHelp()
{

}

// OPENS RECENTLY USED FILES
void MainWindow::openRecentFile()
{
    if (okToContinue()) {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action) {
            loadFile(action->data().toString());
        }
    }
}

// UPDATES LIST OF RECENTLY USED FILES
void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFile);
    while (i.hasNext()) {
        if (!QFile::exists(i.next())) {
            i.remove();
        }
    }

    for (int j = 0; j < MaxRecentFiles; ++j) {
        if (j < recentFile.count()) {
            QString text = tr("&%1 %2").arg(j+1).arg(strippedName(recentFile[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFile[j]);
            recentFileActions[j]->setVisible(true);
        }
        else {
            recentFileActions[j]->setVisible(false);
        }
    }

    separatorAction->setVisible(!recentFile.isEmpty());
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (okToContinue()) {
        writeSettings();
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MainWindow::updateStatusBar()
{
    locationLabel->setText(spr->currentLocation());
    formulaLabel->setText(spr->currentFormula());
}

void MainWindow::spreadSheetModified()
{
    setWindowModified(true);
    updateStatusBar();
}

// CREATES DIALOG WINDOW WHEN APPLICATION HAS UNSAVED DATA
// returns true when user clicked NO ot YES and save the file
// returns false when user clicked CANCEL
bool MainWindow::okToContinue()
{
    if (isWindowModified()) {
        int r = QMessageBox::warning(this, tr("Spreadsheet"), tr("This document has been modified.\nDo you want to save the changes?"), QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        if (r == QMessageBox::Yes) {
            return saveFile();
        }
        else if (r == QMessageBox::Cancel) {
            return false;
        }
    }
    return true;
}

// CHECK WHETHER LOADING IS SUCCESSED
// return false whe loading is falled
// return true when loading is succesed and set current working file
bool MainWindow::loadFile(const QString &fileName)
{
    if (!spr->readFile(fileName)) {
        statusBar()->showMessage(tr("Loading cancelled"), 2000);
        return false;
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
    return true;
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}
