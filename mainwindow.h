#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
    void closeEvent(QCloseEvent *);

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
    void about();
    void openRecentFile();
    void updateStatusBar();
    void spreadSheetModified();

private:
    Ui::MainWindow *ui;

    // GUI
    QTableWidget *table;
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

    QToolBar *fileToolBar;
    QToolBar *editToolBar;

    QAction *aboutQtAction;

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
    QString strippedName(const QString &fullFileNAme);

};

#endif // MAINWINDOW_H
