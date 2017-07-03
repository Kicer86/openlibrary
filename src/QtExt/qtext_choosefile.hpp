
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

    public:
        QtExtChooseFile(const QString& button_caption, const std::function<QString()>& result_callback, QWidget* parent = nullptr);
        QtExtChooseFile(const QString& label, const QString& button_caption, const std::function<QString()>& result_callback, QWidget* parent = nullptr);
        QtExtChooseFile(const QtExtChooseFile &) = delete;

        virtual ~QtExtChooseFile();

        QtExtChooseFile& operator=(const QtExtChooseFile &) = delete;

        QString text() const;

    signals:
        void valueChanged() const;

    private:
        QPushButton* m_button;
        QLineEdit*   m_lineEdit;
        std::function<QString()> m_dialogCallback;

        void buttonClicked() const;
        void setup(const QString &, const QString &, const std::function<QString()> &);
};

#endif
