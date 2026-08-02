#pragma once

namespace porting_base
{
    class FrameStats
    {
    public:
        FrameStats() = default;

        void Reset();
        void Update(double frameSeconds);
        double GetAccumulatedFrameTimeInSeconds() const;
        double GetAverageFrameTimeInMilliseconds() const;
        double GetAverageFPS() const;

    private:
        double m_AccumulatedFrameTime { 0.0 };
        int m_FrameCount { 0 };
    };
} // namespace porting_base