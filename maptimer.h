#ifndef MAPTIMER_H
#define MAPTIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

class MapTimer : public QObject
{
    Q_OBJECT
public:
    explicit MapTimer(int h = 0, int m = 0, int s = 0, double multiplier = 1, QObject *parent = nullptr);
    ~MapTimer();

    void start();
    void stop();
    bool isRunning() const;

    void setInterval(int interval);
    int getInterval() const;

    void setMultiplier(double multiplier);
    double getMultiplier() const;

    QTime currentTime() const;
    QString currentTime(const QString &format) const;

signals:
    void timeout(QTime time);
    void intervalChanged(int newInterval);
    void multiplierChanged(double newMultiplier);

private:
    QTime *setTime;
    QTimer *internalTimer; // periodically invokes signals
    double timeMultiplier;

    void privateTimeout(); // catches QTimer timeout

    static void updateTime(int addMilliseconds, QTime **time);
};

#endif // MAPTIMER_H
