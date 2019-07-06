#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Spreadsheet/spreadsheet.h"

#include <QMainWindow>
#include <QTableWidget>
#include <QAction>
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void openFile();
    bool saveFile();
    bool saveAsFile();
    void cutEdit();
    void copyEdit();
    void pasteEdit();
    void deleteEdit();
    void selectRowEdit();
    void selectColumnEdit();
    void selectAllEdit();
    bool findEdit();
    void goToCellEdit();
    void recalculateTools();
    void sortTools();
    void showGridOptions();
    //bool autoRecalculateOptions();
    void aboutHelp();
    void openRecentFile();
    void updateStatusBar();
    void spreadSheetModified();

private:
    Ui::MainWindow *ui;

    // GUI
    Spreadsheet *spr;
    QLabel *locationLabel;
    QLabel *formulaLabel;
    QStringList recentFile;
    QString curFile;

    enum {MaxRecentFiles = 5};
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;

    QMenu *fileMenu;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *quitAction;

    QMenu *editMenu;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *deleteAction;
        QMenu *selectSubMenu;
        QAction *selectRowAction;
        QAction *selectColumnAction;
        QAction *selectAllAction;
    QAction *findAction;
    QAction *goToCellAction;

    QMenu *toolsMenu;
    QAction *recalculateAction;
    QAction *sortAction;

    QMenu *optionsMenu;
    QAction *showGridAction;

    QMenu *helpMenu;
    QAction *aboutAction;
    QAction *aboutQtAction;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;


    void createActions();
    void createMenues();
    void createContextMenu();
    void createToolBars();
    void createStatusBar();
    void readSettings();
    void writeSettings();
    bool okToContinue();
    bool loadFile(const QString &fileName);
    bool saveFile(const QString &fileName);
    void setCurrentFile(const QString &fileName);
    void updateRecentFileActions();
    QString strippedName(const QString &fullFileName);

};

#endif // MAINWINDOW_H
