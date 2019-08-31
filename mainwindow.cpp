#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "GoToCell/gotocelldialog.h"
#include "Sort/sortdialog.h"

#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QCloseEvent>
#include <QSettings>

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

    setAttribute(Qt::WA_DeleteOnClose);

    for (QWidget *win : QApplication::topLevelWidgets()) {
        if (MainWindow *mainWin = qobject_cast<MainWindow *>(win)) {
            mainWin->updateRecentFileActions();
        }
    }

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

    //Close current window
    closeAction = new QAction(tr("Close"), this);
    closeAction->setIcon(QIcon(":/icons/icons/closeFile.png"));
    closeAction->setShortcut(QKeySequence::Close);
    closeAction->setStatusTip(tr("Close this window"));
    connect(closeAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    // Quit
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setIcon(QIcon(":/icons/icons/exitFile.png"));
    quitAction->setShortcut(Qt::CTRL + Qt::Key::Key_Q);
    quitAction->setStatusTip("Exit program");
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(closeAllWindows()));

    //======================= EDIT MENU =======================//
    // Cut
    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setIcon(QIcon(":/icons/icons/cutEdit.png"));
    cutAction->setShortcut(QKeySequence::Cut);
    cutAction->setStatusTip(tr("Cut"));
    connect(cutAction, SIGNAL(triggered()), spr, SLOT(cutEdit()));

    // Copy
    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/icons/icons/copyEdit.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    copyAction->setStatusTip(tr("Copy"));
    connect(copyAction, SIGNAL(triggered()), spr, SLOT(copyEdit()));

    // Paste
    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/icons/icons/pasteEdit.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    pasteAction->setStatusTip(tr("Paste"));
    connect(pasteAction, SIGNAL(triggered()), spr, SLOT(pasteEdit()));

    // Delete
    deleteAction = new QAction(tr("&Delete"), this);
    deleteAction->setIcon(QIcon(":/icons/icons/deleteEdit.png"));
    deleteAction->setShortcut(QKeySequence::Delete);
    deleteAction->setStatusTip(tr("Delete"));
    connect(deleteAction, SIGNAL(triggered()), spr, SLOT(deleteEdit()));

    // Select
    selectRowAction = new QAction(tr("&Row"), this);
    selectRowAction->setStatusTip(tr("Select row"));
    connect(selectRowAction, SIGNAL(triggered()), spr, SLOT(selectCurrentRowEdit()));
    selectColumnAction = new QAction(tr("&Column"), this);
    selectColumnAction->setStatusTip(tr("Select column"));
    connect(selectColumnAction, SIGNAL(triggered()), spr, SLOT(selectCurrentColumnEdit()));
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
    connect(recalculateAction, SIGNAL(triggered()), spr, SLOT(recalculate()));

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
    autoRecalculateAction = new QAction(tr("&auto-Recalculate"), this);
    autoRecalculateAction->setCheckable(true);
    autoRecalculateAction->setChecked(spr->autoRecalculate());
    autoRecalculateAction->setIcon(QIcon(":/icons/icons/autoRecalculateOptions.png"));
    autoRecalculateAction->setStatusTip(tr("Auto recalculate data in the table"));
    connect(autoRecalculateAction, SIGNAL(toggled(bool)), spr, SLOT(setAutoRecalculate(bool)));

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
    fileMenu->addAction(closeAction);
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
    optionsMenu->addAction(autoRecalculateAction);

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
    // Delete default toolbars
    delete ui->mainToolBar;

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
    QSettings settings("Spreadsheet", "Settings");

    settings.setValue("geometry", saveGeometry());
    settings.setValue("recentFiles", recentFile);
    settings.setValue("showGrid", showGridAction->isChecked());
    settings.setValue("autoRecalc", autoRecalculateAction->isChecked());
}

void MainWindow::readSettings()
{
    QSettings settings("Spreadsheet", "Settings");

    restoreGeometry(settings.value("geometry").toByteArray());
    recentFile = settings.value("recentFiles").toStringList();
    updateRecentFileActions();

    bool showGrid = settings.value("showGrid", true).toBool();
    showGridAction->setChecked(showGrid);

    bool autoRecalc = settings.value("123/autoAecalc", true).toBool();
    autoRecalculateAction->setChecked(autoRecalc);
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
    MainWindow *mainWin = new MainWindow();
    mainWin->show();
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
    GoToCellDialog *dialog = new GoToCellDialog(this);
    if (dialog->exec()) {
        QString str = dialog->getText().toUpper();
        spr->setCurrentCell(str.mid(1).toInt() - 1, str[0].unicode() - 'A');
    }
    delete dialog;
}

void MainWindow::sortTools()
{
    SortDialog dialog(this);
    QTableWidgetSelectionRange range = spr->selectedRange();
    dialog.setColumnRange('A' + range.leftColumn(), 'A' + range.rightColumn());
    if (dialog.exec()) {
        SpreadsheetCompare compare;
        compare.keys[0] = dialog.primaryColumnCombo->currentIndex();
        compare.keys[1] = dialog.secondaryColumnCombo->currentIndex()-1;
        compare.keys[2] = dialog.tertiaryColumnCombo->currentIndex()-1;
        compare.ascending[0] = (dialog.primaryOrderCombo->currentIndex() == 0);
        compare.ascending[1] = (dialog.secondaryOrderCombo->currentIndex() == 0);
        compare.ascending[2] = (dialog.tertiaryOrderCombo->currentIndex() == 0);
        spr->sort(compare);
    }
}

void MainWindow::aboutHelp()
{
    QMessageBox::about(this, tr("About Spreadsheet"), tr("<h2>Spreadsheet</h2>"
                                                         "<p>Spreadsheet is my first application "
                                                         "on Qt that demonstrates QAction, QMainWindow, "
                                                         "QMenuBar, QToolBar and many other components "
                                                         "of Qt toolkit"));
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
    writeSettings();
    if (okToContinue()) {
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
