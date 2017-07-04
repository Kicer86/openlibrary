
#include "qtext_choosefile.hpp"

#include <QCompleter>
#include <QFileSystemModel>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>


QtExtChooseFile::QtExtChooseFile(QWidget* p):
    QWidget(p),
    m_button(nullptr),
    m_lineEdit(nullptr),
    m_dialogCallback()
{
    setup("", "", std::function<QString ()>());
}


QtExtChooseFile::QtExtChooseFile(const QString& button,
                                 const std::function<QString ()>& dialogCallback,
                                 QWidget* p):
    QtExtChooseFile("", button, dialogCallback, p)
{

}


QtExtChooseFile::QtExtChooseFile(const QString& title,
                                 const QString& button,
                                 const std::function<QString()>& dialogCallback,
                                 QWidget* p):
    QWidget(p),
    m_button(nullptr),
    m_lineEdit(nullptr),
    m_dialogCallback(dialogCallback)
{
    setup(title, button, dialogCallback);
}


QtExtChooseFile::~QtExtChooseFile()
{

}


void QtExtChooseFile::setLabel(const QString&)
{
}


void QtExtChooseFile::setButton(const QString&)
{
}


void QtExtChooseFile::setCallback(const std::function<QString ()>&)
{
}


QString QtExtChooseFile::text() const
{
    return m_lineEdit->text();
}


void QtExtChooseFile::buttonClicked() const
{
    const QString value = m_dialogCallback();
    if (value.isEmpty() == false)
        m_lineEdit->setText(value);
}


void QtExtChooseFile::setup(const QString& title, const QString& button, const std::function<QString ()>& dialogCallback)
{
    QHBoxLayout* l = new QHBoxLayout(this);
    QLabel* label = new QLabel(title);
    m_lineEdit = new QLineEdit;
    m_button = new QPushButton(button);

    l->addWidget(label);
    l->addWidget(m_lineEdit);
    l->addWidget(m_button);

    QCompleter *completer = new QCompleter(this);
    QFileSystemModel* model = new QFileSystemModel(completer);
    model->setRootPath(QDir::homePath());
    completer->setModel(model);
    m_lineEdit->setCompleter(completer);

    connect(m_button, &QPushButton::clicked, this, &QtExtChooseFile::buttonClicked);
    connect(m_lineEdit, &QLineEdit::textChanged, this, &QtExtChooseFile::valueChanged);
}
