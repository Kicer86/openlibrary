
#ifndef QTEXT_CHOOSE_FILE
#define QTEXT_CHOOSE_FILE

#include <QWidget>

#include <functional>

#include "QtExt_export.h"

class QLabel;
class QLineEdit;
class QPushButton;

class QTEXT_EXPORT QtExtChooseFile: public QWidget
{
        Q_OBJECT

    public:
        QtExtChooseFile(QWidget* parent = nullptr);
        QtExtChooseFile(const QString& button_caption, const std::function<QString()>& dialog_callback, QWidget* parent = nullptr);
        QtExtChooseFile(const QString& label, const QString& button_caption, const std::function<QString()>& dialog_callback, QWidget* parent = nullptr);
        QtExtChooseFile(const QtExtChooseFile &) = delete;

        virtual ~QtExtChooseFile();

        QtExtChooseFile& operator=(const QtExtChooseFile &) = delete;

        void setLabel(const QString &);
        void setButton(const QString &);
        void setDialogCallback(const std::function<QString()>&);
        void setValue(const QString &);

        QString text() const;
        QString value() const;    // aliast to text()

    signals:
        void valueChanged() const;

    private:
        QLabel* m_label;
        QPushButton* m_button;
        QLineEdit*   m_lineEdit;
        std::function<QString()> m_dialogCallback;

        void buttonClicked() const;
        void setup(const QString &, const QString &, const std::function<QString()> &);
};

#endif
