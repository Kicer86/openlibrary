
#include "qtext_choosefile.hpp"

#include <QFileDialog>
#include <QAbstractButton>
#include <QLineEdit>


QtExtChooseFile::QtExtChooseFile(QAbstractButton *b, QLineEdit *l, QFileDialog *d):
    button(b), lineEdit(l), dialog(d), type(T_QFileDialog)
{
    //wait for button click
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}


QtExtChooseFile::QtExtChooseFile(QAbstractButton *b, QLineEdit *l, QtExtChooseFileDialog *d):
    button(b), lineEdit(l), dialog(d), type(T_QtExtChooseFileDialog)
{
    //wait for button click
    connect(button, SIGNAL(clicked()), this, SLOT(buttonClicked()));
}


QtExtChooseFile::~QtExtChooseFile()
{
    delete dialog;
}


void QtExtChooseFile::buttonClicked() const
{
    //open dialog
    switch(type)
    {
        case T_QFileDialog:
        {
            QFileDialog *dlg = static_cast<QFileDialog *>(dialog);
            if (dlg->exec() == QDialog::Accepted)
            {
                //write new value to edit line
                const QStringList resultList = dlg->selectedFiles();

                const QString &result = resultList.join("; ");

                lineEdit->setText(result);

                emit valueChanged();
            }
            break;
        }

        case T_QtExtChooseFileDialog:
        {
            QtExtChooseFileDialog *dlg = static_cast<QtExtChooseFileDialog *>(dialog);
            if (dlg->exec() == QDialog::Accepted)
            {
                //write new value to edit line
                const QString result = dlg->result();
                lineEdit->setText(result);

                emit valueChanged();
            }
            break;
        }
    }
}
