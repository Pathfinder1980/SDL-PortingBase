#pragma once

namespace porting_base
{
    class FrameTimer
    {
    public:
        FrameTimer(double fixedTimeStep, double maxFrameTime);

        int Advance(double frameSeconds);
        double GetAlphaTime() const;

    private:
        double m_FixedTimeStep;
        double m_MaxFrameSeconds;

        double m_TimeSimulated { 0.0 };
        double m_AccumulatedTime { 0.0 };

        double m_AlphaTime { 0.0 };
    };
} // namespace porting_base