#ifndef _ALARM_WIDGET_H_
#define _ALARM_WIDGET_H_

#include <QWidget>

class QProgressBar;
class QSlider;
class AlarmWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AlarmWidget(QWidget *parent = nullptr);


    QProgressBar* m_progressBar;
    QSlider* m_slider;

signals:

public slots:
};

#endif
