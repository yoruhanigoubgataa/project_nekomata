#include "vk_gpu_obrm_structs.hpp"

#include "context.hpp"
#include "vk_queue.hpp"

namespace nekomata2 {

auto GpuResourceRetireTimelineValues::latestSubmitValues() -> GpuResourceRetireTimelineValues {
    auto graphicsValue = VulkanContext::get().vkQueueGraphics().lastTimelineSubmissionValue();
    auto asyncComputeValue = VulkanContext::get().vkQueueAsyncCompute().lastTimelineSubmissionValue();
    return {graphicsValue, asyncComputeValue};
}
auto GpuResourceRetireTimelineValues::queueCurrentValues() -> GpuResourceRetireTimelineValues {
    auto graphicsValue = VulkanContext::get().vkQueueGraphics().currentTimelineValue();
    auto asyncComputeValue = VulkanContext::get().vkQueueAsyncCompute().currentTimelineValue();
    return {graphicsValue, asyncComputeValue};
}

} // namespace nekomata2