#pragma once

#include <functional>
#include <mutex>
#include <queue>
#include <string>
#include "RLRequest.hpp"
#include "RLResponse.hpp"

namespace TulparEngine {
namespace Tools {

    class EngineRequestManagerRL {
    public:
        static EngineRequestManagerRL& GetInstance() {
            static EngineRequestManagerRL instance;
            return instance;
        }

        // Async request with callback
        void rlRequestAsyncWithCallback(
            const std::string& url,
            const RLRequest& requestData,
            std::function<void(const RLResponse&)> onFinish
        );

        // Must be called periodically (e.g. once per frame in main thread)
        void ProcessCallbacks();

    private:
        EngineRequestManagerRL() = default;
        ~EngineRequestManagerRL() = default;

        // Build JSON payload
        std::string buildRLRequestPayload(const RLRequest& requestData);

        // Perform the synchronous HTTP request
        RLResponse performRLApiRequest(const std::string& url, const std::string& payload);

        // Data structure to hold a callback plus the RLResponse
        struct RLCallbackData {
            std::function<void(const RLResponse&)> callback;
            RLResponse response;
        };

        std::mutex callbackQueueMutex;
        std::queue<RLCallbackData> callbackQueue;
    };

} // namespace Tools
} // namespace TulparEngine