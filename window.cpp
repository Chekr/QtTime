#include "window.h"

#include <QTabWidget>

#include "alarmWidget.h"
#include "stopWatchWidget.h"
#include "timerWidget.h"

Window::Window(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(QString::fromUtf8("Time App"));
    setFixedSize(268, 500);

    m_alarmWidget = new AlarmWidget();
    m_stopwatchWidget = new StopWatchWidget();
    m_timerWidget = new TimerWidget();

    m_tabs = new QTabWidget(this);
    m_tabs->setGeometry(10, 10, 248, 480);
    m_tabs->addTab(m_timerWidget,"Timer");
    //m_tabs->addTab(m_alarmWidget,"Alarm");
    //m_tabs->addTab(m_stopwatchWidget,"Stopwatch");



}
