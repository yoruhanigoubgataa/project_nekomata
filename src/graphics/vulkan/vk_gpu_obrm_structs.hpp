#pragma once
#include "core/platform/int_def.hpp"

namespace nekomata2 {

struct GpuResourceRetireTimelineValues {
    u64 m_graphicsQueueRetireValue = 0;
    u64 m_asyncComputeQueueRetireValue = 0;

    constexpr auto hasAdvancedAfter(GpuResourceRetireTimelineValues& val) const -> bool {
        return (m_graphicsQueueRetireValue >= val.m_graphicsQueueRetireValue)
            && (m_asyncComputeQueueRetireValue >= val.m_asyncComputeQueueRetireValue);
    }

    static auto latestSubmitValues() -> GpuResourceRetireTimelineValues;
    static auto queueCurrentValues() -> GpuResourceRetireTimelineValues;
};

} // namespace nekomata2