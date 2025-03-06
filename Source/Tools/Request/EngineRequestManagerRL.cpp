#include "EngineRequestManagerRL.hpp"
#include <curl/curl.h>
#include <thread>
#include <nlohmann/json.hpp>

namespace {
    size_t writeCallbackRL(void* ptr, size_t size, size_t nmemb, void* userdata) {
        std::string* response = static_cast<std::string*>(userdata);
        size_t totalSize = size * nmemb;
        response->append((char*)ptr, totalSize);
        return totalSize;
    }
}

using json = nlohmann::json;
using namespace TulparEngine::Tools;

std::string EngineRequestManagerRL::buildRLRequestPayload(const RLRequest& requestData) {
    json j;
    j["player_x"]        = requestData.player_x;
    j["player_z"]        = requestData.player_z;
    j["move_speed"]      = requestData.move_speed;
    j["rot_x"]           = requestData.rot_x;
    j["rot_speed"]       = requestData.rot_speed;
    j["player_health"]   = requestData.player_health;
    j["attack_cooldown"] = requestData.attack_cooldown;
    j["opp_x"]           = requestData.opp_x;
    j["opp_z"]           = requestData.opp_z;
    j["opp_rot"]         = requestData.opp_rot;
    j["opp_health"]      = requestData.opp_health;
    j["reward"]          = requestData.reward;
    j["done"]            = requestData.done;
    return j.dump();
}

RLResponse EngineRequestManagerRL::performRLApiRequest(const std::string& url, const std::string& payload) {
    RLResponse rlResp;
    CURL* curl = curl_easy_init();
    if (!curl) {
        // Could log an error
        return rlResp;
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
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallbackRL);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseStr);

    CURLcode res = curl_easy_perform(curl);
    if (res == CURLE_OK) {
        try {
            auto j = json::parse(responseStr);
            rlResp.action = j.value("action", 0);
            rlResp.msg    = j.value("msg", "");
        } catch (...) {
            // parse error
        }
    } else {
        // Could log the curl error
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    return rlResp;
}

void EngineRequestManagerRL::rlRequestAsyncWithCallback(
    const std::string& url,
    const RLRequest& requestData,
    std::function<void(const RLResponse&)> onFinish
) {
    std::string payload = buildRLRequestPayload(requestData);

    // Launch a background thread
    std::thread([this, url, payload, onFinish]() {
        RLResponse response = performRLApiRequest(url, payload);
        {
            std::lock_guard<std::mutex> lock(callbackQueueMutex);
            callbackQueue.push({onFinish, response});
        }
    }).detach();
}

void EngineRequestManagerRL::ProcessCallbacks() {
    std::lock_guard<std::mutex> lock(callbackQueueMutex);
    while (!callbackQueue.empty()) {
        RLCallbackData cbData = callbackQueue.front();
        callbackQueue.pop();
        cbData.callback(cbData.response);
    }
}