
#ifndef QTEXT_CHOOSE_FILE
#define QTEXT_CHOOSE_FILE

#include <QObject>

class QAbstractButton;
class QLineEdit;
class QFileDialog;

class __attribute__ ((visibility ("default"))) QtExtChooseFile: public QObject
{
        Q_OBJECT

        QAbstractButton *button;
        QLineEdit *lineEdit;
        QFileDialog *dialog;

    private slots:
        void buttonClicked() const;

    public:
        QtExtChooseFile(QAbstractButton *, QLineEdit *, QFileDialog *);     //object takes ownership over dialog (will delete it)

        virtual ~QtExtChooseFile();

    signals:
        void valueChanged() const;
};

#endif
