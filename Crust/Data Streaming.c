#include <curl/curl.h>

void send_metrics_to_cloud(const char *url, const char *data) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK)
            fprintf(stderr, "Failed to send metrics: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
    }
}

int main() {
    const char *url = "https://example-cloud-service.com/api/metrics";
    const char *metrics = "{\"cpu\": 45, \"memory\": 70}";
    send_metrics_to_cloud(url, metrics);
    return 0;
}
