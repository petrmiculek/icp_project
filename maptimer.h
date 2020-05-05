#ifndef MAPTIMER_H
#define MAPTIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class MapTimer : public QObject
{
    Q_OBJECT
public:
    explicit MapTimer(int h, int m, int s, double multiplier = 1, QObject *parent = nullptr);
    ~MapTimer();

    void start();
    void stop();

    void setInterval(int interval);
    int getInterval() const;

private slots:
    void privateTimeout();

signals:
    void timeout();
    void intervalChanged();

private:
    QTime *setTime;
    QTimer *internalTimer; // periodically invokes signals
    double timeMultiplier;

};

#endif // MAPTIMER_H
