
#ifndef QTEXT_CHOOSE_FILE
#define QTEXT_CHOOSE_FILE

#include <QObject>

#include "../OpenLibraryExports.hpp"

class QAbstractButton;
class QLineEdit;
class QFileDialog;

class QtExtChooseFileDialog: public QObject
{
    public:
        virtual int exec() = 0;
        virtual QString result() const = 0;
};


class OL_EXPORTS QtExtChooseFile: public QObject
{
        Q_OBJECT

        QAbstractButton *button;
        QLineEdit *lineEdit;
        QObject *dialog;

        const enum Type
        {
            T_QFileDialog,
            T_QtExtChooseFileDialog
        } type;

    private slots:
        void buttonClicked() const;

    public:
        QtExtChooseFile(QAbstractButton *, QLineEdit *, QFileDialog *);     //object takes ownership over dialog (will delete it)
        QtExtChooseFile(QAbstractButton *, QLineEdit *, QtExtChooseFileDialog *);
        QtExtChooseFile(const QtExtChooseFile &) = delete;

        virtual ~QtExtChooseFile();

        QtExtChooseFile& operator=(const QtExtChooseFile &) = delete;

    signals:
        void valueChanged() const;
};

#endif
