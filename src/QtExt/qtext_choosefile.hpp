
#ifndef QTEXT_CHOOSE_FILE
#define QTEXT_CHOOSE_FILE

#include <QWidget>

#include <functional>

#include "QtExt_export.h"

class QPushButton;
class QLineEdit;

class QTEXT_EXPORT QtExtChooseFile: public QWidget
{
        Q_OBJECT

        QPushButton* m_button;
        QLineEdit*   m_lineEdit;
        std::function<QString()> m_dialogCallback;

    private slots:
        void buttonClicked() const;

    public:
        QtExtChooseFile(const QString &, const QString &, const std::function<QString()> &, QWidget * = nullptr);
        QtExtChooseFile(const QtExtChooseFile &) = delete;

        virtual ~QtExtChooseFile();

        QtExtChooseFile& operator=(const QtExtChooseFile &) = delete;

        QString text() const;

    signals:
        void valueChanged() const;
};

#endif
