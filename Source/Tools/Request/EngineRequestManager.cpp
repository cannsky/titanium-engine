#include "EngineRequestManager.hpp"
#include <curl/curl.h>
#include <sstream>
#include "EngineDebugManager.hpp"
#include <nlohmann/json.hpp>
#include <thread>

using json = nlohmann::json;
using namespace TulparEngine::Tools;

namespace {
    size_t writeCallback(void* ptr, size_t size, size_t nmemb, void* userdata) {
        std::string* response = static_cast<std::string*>(userdata);
        size_t totalSize = size * nmemb;
        response->append((char*)ptr, totalSize);
        return totalSize;
    }
}

EngineRequestManager::EngineRequestManager() {
    // constructor
}

EngineRequestManager::~EngineRequestManager() {
    // destructor
}

void EngineRequestManager::Init() {
    curl_global_init(CURL_GLOBAL_ALL);
}

void EngineRequestManager::Start() {

}

void EngineRequestManager::Run() {
    // process callbacks
    ProcessCallbacks();
}

void EngineRequestManager::Cleanup() {
    curl_global_cleanup();
}

std::string EngineRequestManager::buildRequestPayload(const std::string& model, const std::string& prompt) {
    json j;
    j["model"] = model;
    j["prompt"] = prompt;
    j["stream"] = false;
    return j.dump();
}

LLMResponse EngineRequestManager::performApiRequest(const std::string& url, const std::string& payload) {
    LLMResponse result;
    CURL* curl = curl_easy_init();
    if (!curl) {
        EngineDebugManager::logEngineError("Failed to initialize CURL");
        return result;
    }

    std::string responseStr;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_POST, 1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, payload.c_str());
    curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)payload.size());

    // Set headers
    struct curl_slist* headers = nullptr;
    headers = curl_slist_append(headers, "Content-Type: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set write callback
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        EngineDebugManager::logEngineError("CURL request failed: " + std::string(curl_easy_strerror(res)));
    } else {
        try {
            auto j = json::parse(responseStr);
            result.model = j.value("model", "");
            result.response = j.value("response", "");
            result.created_at = j.value("created_at", "");
            result.done = j.value("done", false);
        } catch (...) {
            EngineDebugManager::logEngineError("Failed to parse JSON response");
        }
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return result;
}

LLMResponse EngineRequestManager::apiRequest(const std::string& url, const std::string& model, const std::string& prompt) {
    std::string payload = buildRequestPayload(model, prompt);
    return performApiRequest(url, payload);
}

std::future<LLMResponse> EngineRequestManager::apiRequestAsync(const std::string& url, const std::string& model, const std::string& prompt) {
    std::string payload = buildRequestPayload(model, prompt);
    return std::async(std::launch::async, [this, url, payload]() {
        return performApiRequest(url, payload);
    });
}

void EngineRequestManager::apiRequestAsyncWithCallback(const std::string& url, const std::string& model, const std::string& prompt,
                                                       std::function<void(const LLMResponse&)> onFinish) {
    std::string payload = buildRequestPayload(model, prompt);
    // Launch a background thread
    std::thread([this, url, payload, onFinish]() {
        LLMResponse response = performApiRequest(url, payload);
        {
            // Lock and push the callback + response into the queue
            std::lock_guard<std::mutex> lock(callbackQueueMutex);
            callbackQueue.push({onFinish, response});
        }
    }).detach();
}

void EngineRequestManager::ProcessCallbacks() {
    // Called from main thread each frame
    std::lock_guard<std::mutex> lock(callbackQueueMutex);
    while (!callbackQueue.empty()) {
        auto cbData = callbackQueue.front();
        callbackQueue.pop();
        // Execute the callback
        cbData.callback(cbData.response);
    }
}