#include "timerWidget.h"

#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QPushButton>
#include <QSignalMapper>
#include <QIntValidator>
#include <QStateMachine>
#include <QState>
#include <QTimer>
#include <QFont>

#include <QDebug>

#include "alarmEvent.h"

TimerWidget::TimerWidget(QWidget *parent) : QWidget(parent)
{
    ////////////////////
    // CONTROLS ////////
    ////////////////////
    m_lblNegative = new QLabel(this);
    QFont font = m_lblNegative->font();
    font.setPointSize(28);
    font.setBold(true);
    m_lblNegative->setFont(font);
    m_lblNegative->setText("-");
    m_lblNegative->setGeometry(10, 10, 15, 50);
    m_lblNegative->setVisible(false);

    m_txtHours = new QLineEdit(this);
    m_txtHours->setFont(font);
    m_txtHours->setGeometry(30, 10, 50, 50);
    m_txtHours->setPlaceholderText("00");
    m_txtHours->setValidator( new QIntValidator(0, 99, this) );

    QLabel* lblColonA = new QLabel(this);
    lblColonA->setFont(font);
    lblColonA->setText(":");
    lblColonA->setGeometry(85, 10, 10, 50);

    m_txtMinutes = new QLineEdit(this);
    m_txtMinutes->setFont(font);
    m_txtMinutes->setGeometry(98, 10, 50, 50);
    m_txtMinutes->setPlaceholderText("00");
    m_txtMinutes->setValidator( new QIntValidator(0, 59, this) );

    QLabel* lblColonB = new QLabel(this);
    lblColonB->setFont(font);
    lblColonB->setText(":");
    lblColonB->setGeometry(153, 10, 10, 50);

    m_txtSeconds = new QLineEdit(this);
    m_txtSeconds->setFont(font);
    m_txtSeconds->setGeometry(166, 10, 50, 50);
    m_txtSeconds->setPlaceholderText("00");
    m_txtSeconds->setValidator( new QIntValidator(0, 59, this) );

    m_pgbTimeLeft = new QProgressBar(this);
    m_pgbTimeLeft->setRange(0.0f, 100.0f);
    m_pgbTimeLeft->setValue(0);
    m_pgbTimeLeft->setInvertedAppearance(true);
    m_pgbTimeLeft->setTextVisible(false);
    m_pgbTimeLeft->setGeometry(10, 65, 225, 20);

    QSignalMapper* signalMapper = new QSignalMapper (this) ;
    m_btnAddTenMinutes = new QPushButton("+10 min", this);
    m_btnAddTenMinutes->setGeometry(10, 100, 70, 30);
    connect(m_btnAddTenMinutes, SIGNAL (released()), signalMapper, SLOT (map()));
    m_btnAddOneMinute = new QPushButton("+1 min", this);
    m_btnAddOneMinute->setGeometry(88, 100, 70, 30);
    connect(m_btnAddOneMinute, SIGNAL (released()), signalMapper, SLOT (map()));
    m_btnAddFifteenSeconds = new QPushButton("+15 sec", this);
    m_btnAddFifteenSeconds->setGeometry(165, 100, 70, 30);
    connect(m_btnAddFifteenSeconds, SIGNAL (released()), signalMapper, SLOT (map()));

    signalMapper -> setMapping (m_btnAddTenMinutes, 600) ;
    signalMapper -> setMapping (m_btnAddOneMinute, 60) ;
    signalMapper -> setMapping (m_btnAddFifteenSeconds, 15) ;

    connect (signalMapper, SIGNAL(mapped(int)), this, SLOT(AddSeconds(int))) ;

    m_btnStart = new QPushButton("Start", this);
    m_btnStart->setGeometry(10, 150, 225, 30);

    m_btnStop = new QPushButton("Stop", this);
    m_btnStop->setGeometry(130, 150, 105, 30);
    m_btnStop->setVisible(false);

    m_btnPause = new QPushButton("Pause", this);
    m_btnPause->setGeometry(10, 150, 105, 30);
    m_btnPause->setVisible(false);

    m_btnResume = new QPushButton("Resume", this);
    m_btnResume->setGeometry(10, 150, 105, 30);
    m_btnResume->setVisible(false);
    ////////////////
    // INIT ////////
    ////////////////
    m_dataRateTimer = new QTimer(this);
    connect(m_dataRateTimer, SIGNAL(timeout()), SLOT(GetTimerData()));

    m_machine = new QStateMachine(this);

    m_stateRunning = new QState();
    m_stateRunning->setObjectName("running");
    QObject::connect(m_stateRunning, SIGNAL(entered()), this, SLOT(Start()));

    m_stateStopped = new QState();
    m_stateStopped->setObjectName("stopped");
    QObject::connect(m_stateStopped, SIGNAL(entered()), this, SLOT(Stop()));

    m_statePaused = new QState();
    m_statePaused->setObjectName("paused");
    QObject::connect(m_statePaused, SIGNAL(entered()), this, SLOT(Pause()));

    m_stateAlarm = new QState();
    m_stateAlarm->setObjectName("alarm");
    QObject::connect(m_stateAlarm, SIGNAL(entered()), this, SLOT(Alarm()));

    m_stateStopped->addTransition(m_btnStart, SIGNAL(clicked()), m_stateRunning);
    m_statePaused->addTransition(m_btnResume, SIGNAL(clicked()), m_stateRunning);
    m_statePaused->addTransition(m_btnStart, SIGNAL(clicked()), m_stateRunning);
    m_stateAlarm->addTransition(m_btnStop, SIGNAL(clicked()), m_stateStopped);
    m_stateRunning->addTransition(m_btnPause, SIGNAL(clicked()), m_statePaused);
    m_stateRunning->addTransition(m_btnStop, SIGNAL(clicked()), m_stateStopped);

    m_machine->addState(m_stateStopped);
    m_machine->addState(m_stateRunning);
    m_machine->addState(m_statePaused);
    m_machine->addState(m_stateAlarm);

    m_machine->setInitialState(m_stateStopped);

    m_machine->start();
}

int TimerWidget::GetSecondsFromUi()
{
    qInfo() << "get ui";
    int hours = m_txtHours->text().toInt();
    int minutes = m_txtMinutes->text().toInt();
    int seconds = m_txtSeconds->text().toInt();
    return (hours*60*60) + (minutes*60) + seconds;
}

void TimerWidget::SetUiInSeconds(int seconds)
{
    int displaySeconds = seconds;
    if(seconds < 0)
    {
        m_lblNegative->setVisible(true);
        displaySeconds = -displaySeconds;
    }
    else
    {
        m_lblNegative->setVisible(false);
    }

    int hours = displaySeconds / 60 / 60;
    int minutes = (displaySeconds / 60) % 60;
    int secondsLeftOver = displaySeconds % 60;
    m_txtHours->setText(QString::number(hours).rightJustified(2, '0'));
    m_txtMinutes->setText(QString::number(minutes).rightJustified(2, '0'));
    m_txtSeconds->setText(QString::number(secondsLeftOver).rightJustified(2, '0'));
}

void TimerWidget::GetTimerData()
{
    float elapsedTime = m_timer.GetElapsedTimeInSeconds();

    if(m_timer.IsRunning())
    {
        float secondsLeft = static_cast<float>(m_secondsForTimer) - (elapsedTime);
        SetUiInSeconds(static_cast<int>(secondsLeft));

        int barFill = secondsLeft > 0.0f ? (secondsLeft*100.0f)/static_cast<float>(m_secondsForTimer) : 0.0f;
        m_pgbTimeLeft->setValue(barFill);

        if(elapsedTime < static_cast<float>(m_secondsForTimer))
        {
            m_machine->postEvent(new AlarmEvent());
        }
    }
}

void TimerWidget::AddSeconds(int secondsToAdd)
{
    if(m_timer.IsRunning())
    {
        GetSecondsFromUi();
    }
    m_secondsForTimer += secondsToAdd;

    int hours = m_secondsForTimer / 60 / 60;
    int minutes = (m_secondsForTimer / 60) % 60;
    int seconds = m_secondsForTimer % 60;

    m_txtHours->setText(QString::number(hours));
    m_txtMinutes->setText(QString::number(minutes));
    m_txtSeconds->setText(QString::number(seconds));
}

void TimerWidget::Start()
{
    m_txtHours->setDisabled(true);
    m_txtMinutes->setDisabled(true);
    m_txtSeconds->setDisabled(true);

    // Don't only grab the time from the UI if the main timer doesn't have time.
    if(m_timer.GetElapsedTimeInSeconds() == 0.0f)
    {
        m_secondsForTimer = GetSecondsFromUi();
    }

    m_timer.Start();
    m_dataRateTimer->start(250);

    m_btnStart->setVisible(false);
    m_btnStop->setVisible(true);
    m_btnPause->setVisible(true);
    m_btnResume->setVisible(false);
}

void TimerWidget::Stop()
{
    qInfo() << "stop" << m_secondsForTimer;

    m_txtHours->setDisabled(false);
    m_txtMinutes->setDisabled(false);
    m_txtSeconds->setDisabled(false);

    m_pgbTimeLeft->setValue(0.0f);

    SetUiInSeconds(m_secondsForTimer);

    m_timer.ResetAndStop();
    m_dataRateTimer->stop();

    m_btnStart->setVisible(true);
    m_btnStop->setVisible(false);
    m_btnPause->setVisible(false);
    m_btnResume->setVisible(false);
}

void TimerWidget::Pause()
{
    m_timer.Stop();
    m_dataRateTimer->stop();

    m_btnStart->setVisible(false);
    m_btnStop->setVisible(true);
    m_btnPause->setVisible(false);
    m_btnResume->setVisible(true);
}

void TimerWidget::Alarm()
{
    qInfo() << "ALARM!!!" ;
    //m_timer.Stop();
    //m_dataRateTimer->stop();
}
