#include "FrameStats.h"

namespace porting_base
{
    void FrameStats::Reset()
    {
        m_AccumulatedFrameTime = 0.0;
        m_FrameCount = 0;
    }

    void FrameStats::Update(double frameSeconds)
    {
        m_AccumulatedFrameTime += frameSeconds;
        ++m_FrameCount;
    }

    double FrameStats::GetAccumulatedFrameTimeInSeconds() const
    {
        return m_AccumulatedFrameTime;
    }

    double FrameStats::GetAverageFPS() const
    {
        if (m_AccumulatedFrameTime > 0.0)
        {
            return m_FrameCount / m_AccumulatedFrameTime;
        }
        return 0.0;
    }

    double FrameStats::GetAverageFrameTimeInMilliseconds() const
    {
        if (m_FrameCount > 0)
        {
            return 1000.0 * m_AccumulatedFrameTime / m_FrameCount;
        }
        return 0.0;
    }
} // namespace porting_base




