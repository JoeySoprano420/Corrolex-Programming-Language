void send_script_to_cloud(const char *script, const char *server_url) {
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, server_url);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, script);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Error uploading script: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
    }
}
