void upload_to_cloud(const char *script_path, const char *cloud_url) {
    // Upload script for remote execution.
    CURL *curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, cloud_url);
        FILE *script = fopen(script_path, "rb");
        curl_easy_setopt(curl, CURLOPT_READDATA, script);
        curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);
        CURLcode res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            fprintf(stderr, "Error uploading script: %s\n", curl_easy_strerror(res));
        }
        curl_easy_cleanup(curl);
        fclose(script);
    }
}
