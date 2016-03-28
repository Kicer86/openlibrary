
#include "qtext_choosefile.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


QtExtChooseFile::QtExtChooseFile(const QString& title,
                                 const QString& button,
                                 const std::function<QString()>& dialogCallback,
                                 QWidget* p):
    QWidget(p)
{
    QHBoxLayout* l = new QHBoxLayout(this);
    QLabel* label = new QLabel(title);
    m_lineEdit = new QLineEdit;
    m_button = new QPushButton(button);

    l->addWidget(label);
    l->addWidget(m_lineEdit);
    l->addWidget(m_button);

    connect(m_button, &QPushButton::clicked, this, &QtExtChooseFile::buttonClicked);
}


QtExtChooseFile::~QtExtChooseFile()
{

}


void QtExtChooseFile::buttonClicked() const
{
    const QString value = m_dialogCallback();
    if (value.isEmpty() == false)
    {
        m_lineEdit->setText(value);

        emit valueChanged();
    }
}


QString QtExtChooseFile::text() const
{
    return m_lineEdit->text();
}
