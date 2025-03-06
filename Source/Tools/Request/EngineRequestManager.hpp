#pragma once

#include <string>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include "LLMResponse.hpp"

namespace TulparEngine::Tools {
    class EngineRequestManager {
    public:
        static EngineRequestManager& GetInstance() {
            static EngineRequestManager instance;
            return instance;
        }

        void Init();
        void Start();
        void Run();
        void Cleanup();

        // Synchronous call (blocking)
        LLMResponse apiRequest(const std::string& url, const std::string& model, const std::string& prompt);

        // Asynchronous call that returns a future
        std::future<LLMResponse> apiRequestAsync(const std::string& url, const std::string& model, const std::string& prompt);

        // Asynchronous call with a callback invoked when done (on main thread)
        void apiRequestAsyncWithCallback(const std::string& url, const std::string& model, const std::string& prompt,
                                         std::function<void(const LLMResponse&)> onFinish);

        // Streamed requests omitted for brevity, same pattern can apply.

        // Call this each frame (or periodically) on the main thread to run queued callbacks
        void ProcessCallbacks();

        EngineRequestManager(const EngineRequestManager&) = delete;
        void operator=(const EngineRequestManager&) = delete;

    private:
        EngineRequestManager();
        ~EngineRequestManager();

        std::string buildRequestPayload(const std::string& model, const std::string& prompt);
        LLMResponse performApiRequest(const std::string& url, const std::string& payload);

        // Data structure to hold callbacks and responses
        struct CallbackData {
            std::function<void(const LLMResponse&)> callback;
            LLMResponse response;
        };

        std::mutex callbackQueueMutex;
        std::queue<CallbackData> callbackQueue;
    };
}