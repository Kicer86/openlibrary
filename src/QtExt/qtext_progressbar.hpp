
#ifndef QEXPROGRESSBAR_HPP
#define QEXPROGRESSBAR_HPP


//class which extends QProgrssBar by posibility of storing and restoring it's content

#include <QProgressBar>

#include "QtExt_exports.hpp"

class QProgressBar;

//model
class QTEXT_EXPORTS QExProgressBarModel: public QObject
{
        Q_OBJECT

        int m_value;
        int m_minimum, m_maximum;
        bool reseted;

    public:
        explicit QExProgressBarModel(QObject *p = 0);
        ~QExProgressBarModel() {}

        int value() const
        {
            return m_value;
        }

        int minimum() const
        {
            return m_minimum;
        }

        int maximum() const
        {
            return m_maximum;
        }

        bool isReseted() const
        {
            return reseted;
        }

    public slots:
        void setMaximum(int);
        void setMinimum(int);
        void setRange (int, int);
        void setValue (int);
        void reset();

    signals:
        void setPBMaximum(int);
        void setPBMimimum(int);
        void setPBRange(int, int);
        void setPBValue(int);
};

//view
class QTEXT_EXPORTS QExProgressBarView: public QProgressBar
{
        const QExProgressBarModel *model;

    public:
        explicit QExProgressBarView(QWidget *p = 0);
        QExProgressBarView(const QExProgressBarView &) = delete;

        void setModel(const QExProgressBarModel *);

        QExProgressBarView& operator=(const QExProgressBarView &) = delete;
};


#endif // QEXPROGRESSBAR_HPP
