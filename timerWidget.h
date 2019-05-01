#ifndef _TIMER_WIDGET_H_
#define _TIMER_WIDGET_H_

#include <QWidget>

class QFont;
class QLabel;
class QLineEdit;
class QProgressBar;
class QPushButton;
class QStateMachine;
class QState;
class QTimer;

#include "shared/misc/Timer.h"

class TimerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit TimerWidget(QWidget *parent = nullptr);

    QFont m_labelFont;
    QFont m_timerFont;
    QLabel* m_lblNegative;
    QLineEdit* m_txtHours;
    QLineEdit* m_txtMinutes;
    QLineEdit* m_txtSeconds;
	QProgressBar* m_pgbTimeLeft;

    QPushButton* m_btnAddTenMinutes;
    QPushButton* m_btnAddOneMinute;
    QPushButton* m_btnAddFifteenSeconds;

	QPushButton* m_btnStart;
	QPushButton* m_btnStop;
	QPushButton* m_btnPause;
	QPushButton* m_btnResume;

    QStateMachine* m_machine;
    QState *m_stateStopped;
    QState *m_stateRunning;
    QState *m_statePaused;
    QState *m_stateAlarm;

    QTimer* m_dataRateTimer;

private:
    Timer::Timer m_timer;
    int m_secondsForTimer = 0;

private:
    int GetSecondsFromUi();
    void SetUiInSeconds(int seconds);

signals:

public slots:
    void GetTimerData();
    void AddSeconds(int seconds);
    void Start();
    void Stop();
    void Pause();
    void Alarm();
};

#endif
