#ifndef _STOP_WATCH_WIDGET_H_
#define _STOP_WATCH_WIDGET_H_

#include <QWidget>

class QPushButton;
class StopWatchWidget : public QWidget
{
    Q_OBJECT
public:
    explicit StopWatchWidget(QWidget *parent = nullptr);

    QPushButton *myButton;

signals:

public slots:
};

#endif
