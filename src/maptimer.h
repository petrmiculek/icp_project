/* maptimer.h
 * Project: CPP
 * Description: Timer sending periodic signals.
 * Author: Kryštof Lavinger, FIT <xlavin00@stud.fit.vutbr.cz>
 */

#ifndef MAPTIMER_H
#define MAPTIMER_H

#include <QObject>
#include <QTime>
#include <QTimer>

/**
 * @brief The MapTimer class
 * @details Timer class sending periodical signals and with the ability to set time.
 */
class MapTimer : public QObject
{
    Q_OBJECT
public:
    /**
     * @brief Creates new instance of MapTimer.
     * @param h hour
     * @param m minute
     * @param s second
     * @param multiplier Time multiplier for manipulating time speed.
     * @param parent QObject parent.
     * @remarks Uses 24-hour format.
     */
    explicit MapTimer(int h = 0, int m = 0, int s = 0, double multiplier = 1, QObject *parent = nullptr);
    ~MapTimer();

    /**
     * @brief Starts the timer.
     */
    void start();
    /**
     * @brief Stops the timer.
     */
    void stop();
    /**
     * @brief Resets time to its initial value.
     * @details Resets time to its initial value. If the time was running, it starts it again.
     */
    void reset();
    /**
     * @brief Returns true if the timer is running.
     * @return true if the timer is running
     */
    bool isRunning() const;

    /**
     * @brief Sets timeout interval.
     * @details Sets the time in milliseconds of how often the timeout signal will be emitted.
     * @param interval Time in milliseconds
     */
    void setInterval(int interval);
    /**
     * @brief Returns current interval.
     * @details Returns current interval in milliseconds of how often the timeout signal is emitted.
     * @return Interval
     */
    int getInterval() const;

    /**
     * @brief Sets new time multiplier.
     * @param multiplier New time multiplier.
     */
    void setMultiplier(double multiplier);
    /**
     * @brief Returns current time multiplier.
     * @return current multiplier
     */
    double getMultiplier() const;

    /**
     * @brief Returns current time.
     * @return current time
     */
    QTime currentTime() const;
    /**
     * @brief Returns current time.
     * @param format Format in which the time will be returned.
     * @return current time
     */
    QString currentTime(const QString &format) const;

signals:
    /**
     * @brief Timeout signal.
     * @details Periodically emitted timeout signal.
     * @param time New current time of the MapTimer class instance.
     */
    void timeout(QTime time);
    /**
     * @brief Interval changed signal.
     * @details Interval changed signal that is emitted every time the MapTimer's interval changes.
     * @param newInterval New timer interval.
     */
    void intervalChanged(int newInterval);
    /**
     * @brief Multiplier changed signal.
     * @details Multiplier changed signal that is emitted every time the multiplier changes.
     * @param newMultiplier New multiplier
     */
    void multiplierChanged(double newMultiplier);
    /**
     * @brief Signal emitted every time the timer is reset.
     */
    void reset_signal();

private:
    /**
     * @brief Current time.
     */
    QTime *setTime;
    /**
     * @brief Internal timer.
     */
    QTimer *internalTimer; // periodically invokes signals
    /**
     * @brief Time multiplier.
     */
    double timeMultiplier;

    /**
     * @brief Private slot that catches timeout from the internal timer.
     */
    void privateTimeout(); // catches QTimer timeout

    /**
     * @brief Adds to the current time.
     * @param addMilliseconds Milliseconds to be added.
     */
    void updateTime(int addMilliseconds);
};

#endif // MAPTIMER_H
