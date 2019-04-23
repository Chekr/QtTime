#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QProgressBar>
#include <QSlider>

class Window : public QWidget
{
    Q_OBJECT
public:
    explicit Window(QWidget *parent = nullptr);

    QProgressBar *progressBar;
    QSlider *slider;

signals:

public slots:
};

#endif // WINDOW_H
