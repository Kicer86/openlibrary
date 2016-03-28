
#include "qtext_choosefile.hpp"

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


QtExtChooseFile::QtExtChooseFile(const QString& title,
                                 const QString& button,
                                 const std::function<QString()>& dialogCallback,
                                 QWidget* p):
    QWidget(p),
    m_button(nullptr),
    m_lineEdit(nullptr),
    m_dialogCallback(dialogCallback)
{
    QHBoxLayout* l = new QHBoxLayout(this);
    QLabel* label = new QLabel(title);
    m_lineEdit = new QLineEdit;
    m_button = new QPushButton(button);

    l->addWidget(label);
    l->addWidget(m_lineEdit);
    l->addWidget(m_button);

    connect(m_button, &QPushButton::clicked, this, &QtExtChooseFile::buttonClicked);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &QtExtChooseFile::valueChanged);
}


QtExtChooseFile::~QtExtChooseFile()
{

}


void QtExtChooseFile::buttonClicked() const
{
    const QString value = m_dialogCallback();
    if (value.isEmpty() == false)
        m_lineEdit->setText(value);
}


QString QtExtChooseFile::text() const
{
    return m_lineEdit->text();
}
