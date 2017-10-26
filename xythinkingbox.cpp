#include "xythinkingbox.h"
#include <QLabel>
#include <QMovie>
#include <QGridLayout>

XYThinkingBox::XYThinkingBox(const QString &text, QWidget *parent)
    : QWidget(parent), times(0), timeID(0)
{
    QLabel *ico = new QLabel;
    ico->setMinimumWidth(100);
    movie = new QMovie(":/other/thinking.gif");
    movie->setParent(ico);
    ico->setMovie(movie);

    QLabel *word = new QLabel(QString::fromStdWString(L"<font color=#792432 size=4><strong>%1</strong></font>").arg(text));
    word->setMinimumWidth(100);
    timeLabel = new QLabel;

    QGridLayout *layout = new QGridLayout;
    layout->addWidget(ico, 0, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(word, 1, 0, 1, 1, Qt::AlignCenter);
    layout->addWidget(timeLabel, 0, 1, 2, 1, Qt::AlignCenter);

    setLayout(layout);

    resize(150, 70);
}

XYThinkingBox::~XYThinkingBox()
{

}

void XYThinkingBox::start()
{
    setVisible(true);
    timeID = startTimer(1000);
    times = 0;
    movie->start();
    timeLabel->setText(QString("<font color=#158243 size=8><strong>%1</strong></font>").arg(times));
}

void XYThinkingBox::stop()
{
    setVisible(false);
    killTimer(timeID);
    timeID = 0;
    movie->stop();
}

void XYThinkingBox::timerEvent(QTimerEvent *event)
{
    if (timeID == event->timerId())
    {
        times++;
        timeLabel->setText(QString("<font color=#158243 size=10>%1</font>").arg(times));
    }
}

