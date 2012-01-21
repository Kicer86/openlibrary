
#include "qtext_choosefile.hpp"

#include <QFileDialog>
#include <QAbstractButton>
#include <QLineEdit>


QtExtChooseFile::QtExtChooseFile(QAbstractButton *b, QLineEdit *l, QFileDialog *d):
    button(b), lineEdit(l), dialog(d)
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

    if (dialog->exec() == QDialog::Accepted)
    {
        //write new value to edit line
        const QStringList resultList = dialog->selectedFiles();

        const QString &result = resultList.join("; ");

        lineEdit->setText(result);

        emit valueChanged();
    }
}
