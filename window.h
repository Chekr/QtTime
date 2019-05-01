#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <QWidget>


class QTabWidget;

class AlarmWidget;
class StopWatchWidget;
class TimerWidget;
class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    QTabWidget* m_tabs;

    AlarmWidget* m_alarmWidget;
    StopWatchWidget* m_stopwatchWidget;
    TimerWidget* m_timerWidget;


signals:

public slots:
};

#endif
