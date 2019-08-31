#ifndef SPREADSHEET_H
#define SPREADSHEET_H

#include "Cell/cell.h"

#include <QTableWidget>

class SpreadsheetCompare
{
public:
    bool operator()(const QStringList &row1,
                    const QStringList &row2) const;

    enum {KeyCount = 3};
    int keys[KeyCount];
    bool ascending[KeyCount];
};

class Spreadsheet : public QTableWidget
{
    Q_OBJECT

public:
    Spreadsheet(QWidget *parent = nullptr);

    bool autoRecalculate() const;
    QString currentLocation() const;
    QString currentFormula() const;
    QTableWidgetSelectionRange selectedRange() const;
    void clear();
    bool readFile(const QString &filename);
    bool writeFile(const QString &filename);
    void sort(const SpreadsheetCompare &compare);

public slots:
    void cutEdit();
    void copyEdit();
    void pasteEdit();
    void deleteEdit();
    void selectCurrentRowEdit();
    void selectCurrentColumnEdit();
    void recalculate();
    void setAutoRecalculate(bool recalc);
    void findNext(const QString &str, Qt::CaseSensitivity cs);
    void findPrevious(const QString &str, Qt::CaseSensitivity cs);

signals:
    void modified();

private slots:
    void somethingChanged();

private:
    enum {MagicNumber = 0x7F51C883, RowCount = 999, ColumnCount = 26};

    Cell *cell(int row, int column) const;
    QString text(int row, int column) const;
    QString formula(int row, int column) const;
    void setFormula(int row, int column, const QString &formula);

    bool autoRecalc;

};

#endif // SPREADSHEET_H
