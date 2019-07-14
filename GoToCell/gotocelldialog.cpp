#include "gotocelldialog.h"

#include <QRegExpValidator>
#include <QHBoxLayout>
#include <QVBoxLayout>

GoToCellDialog::GoToCellDialog(QWidget *parent) : QDialog(parent)
{
    label = new QLabel("&Cell location");
    lineEdit = new QLineEdit();

    QRegExp regExp("[A-Za-z][1-9][0-9]{0,2}");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));

    label->setBuddy(lineEdit);
    connect(lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(lineEditTextChanged()));

    okButton = new QPushButton("&OK");
    okButton->setDefault(true);
    okButton->setEnabled(false);
    connect(okButton, SIGNAL(clicked()), this, SLOT(accept()));

    closeButton = new QPushButton("&Cancel");
    connect(closeButton, SIGNAL(clicked()), this, SLOT(reject()));

    QHBoxLayout *up = new QHBoxLayout();
    up->addWidget(label);
    up->addWidget(lineEdit);

    QHBoxLayout *down = new QHBoxLayout();
    down->addStretch();
    down->addWidget(okButton);
    down->addWidget(closeButton);

    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->addLayout(up);
    mainLayout->addLayout(down);

    setLayout(mainLayout);
    setFixedHeight(sizeHint().height());
}

QString GoToCellDialog::getText()
{
    return lineEdit->text();
}

void GoToCellDialog::lineEditTextChanged()
{
    okButton->setEnabled(lineEdit->hasAcceptableInput());
}
