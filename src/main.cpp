#include <iostream>
#include <chrono>

#include <curl/curl.h>
#include <drogon/drogon.h>

using namespace drogon;

struct cbResp {
    std::vector<char> data;
};

size_t
writeCallback(char* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    cbResp* rq = static_cast<cbResp*>(userp);
    std::copy(contents, contents + realsize, std::back_inserter(rq->data));
    return size * nmemb;
}

std::vector<char>
downloadViaCurl(std::string& url) {
    cbResp respFromServer;
    auto t1 = std::chrono::high_resolution_clock::now();
    auto* curl = curl_easy_init();
    if (!curl)
        return respFromServer.data;

    curl_easy_setopt(curl, CURLOPT_URL, url.data());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &respFromServer);

    auto res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    if (res != CURLE_OK) {
        return respFromServer.data;
    }
    auto elapsed1 =
        std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t1);
    std::cout << "receive curl response! size " << respFromServer.data.size() << std::endl;
    std::cout << "Time taken via curl " << elapsed1.count() << std::endl;
    return respFromServer.data;
}

void
downloadViaDrogon(std::string& url) {
    trantor::Logger::setLogLevel(trantor::Logger::kTrace);
    {
        auto t3 = std::chrono::high_resolution_clock::now();
        auto client = HttpClient::newHttpClient(url, nullptr, false, false);
        auto req = HttpRequest::newHttpRequest();
        req->setMethod(drogon::Get);

        client->sendRequest(req, [t3](ReqResult result, const HttpResponsePtr& response) {
            if (result != ReqResult::Ok) {
                std::cout << "error while sending request to server! result: " << result << std::endl;
                return;
            }
            auto body = response->getBody();
            auto elapsed2 =
                std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - t3);
            std::cout << "receive drogon response! size " << body.size() << std::endl;
            std::cout << "Time taken via drogon " << elapsed2.count() << std::endl;

            return;
        });
    }
}

int
main(int argc, char** argv) {
    if (argc < 2) {
        printf("Usage: %s <url>", argv[0]);
        return 0;
    }

    std::string path = argv[1];

    sleep(10);
    downloadViaDrogon(path);
    downloadViaCurl(path);
    app().run();
}
