#ifndef TIMER_HPP
#define TIMER_HPP

struct Timer
{
    Timer();
    Timer(float duration);
    ~Timer();

    int mStartTicks = 0;
    int mEndTicks = 0;
    float mDuration = 0;

    void setDuration(float duration);
    /// @brief saves the current reference time
    void reset();
    /// @brief saves the time elapsed since timer was reset
    void mark();

    /// @brief delta time since timer was reset
    /// @return 
    float elapsed() const;
    /// @brief returns true if time elapsed is greater or equal to duration specified
    /// @return 
    bool isDone() const;
    float waitUntilDone() const;
};

#endif