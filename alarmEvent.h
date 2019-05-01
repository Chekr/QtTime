#ifndef _ALARM_EVENT_H_
#define _ALARM_EVENT_H_

class AlarmEvent : public QEvent
{
public:
    AlarmEvent() : QEvent(QEvent::Type())
        {}
};

#endif
