#include <json-c/json_object.h>
#include <json-c/json_tokener.h>
#include <json-c/json_types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <curl/curl.h>
#include <curl/easy.h>
#include <json-c/json.h>

struct String {
    char *data;
    size_t size;
};

static size_t write_callback(char *contents, size_t size, size_t nmemb, void *userp){
    size_t realsize = size * nmemb;
    struct String *s = (struct String *)userp;
    char *ptr = realloc(s->data, s->size + realsize + 1);
    if (!ptr) return 0;
    
    s->data = ptr;
    memcpy(&(s->data[s->size]), contents, realsize);
    s->size += realsize;
    s->data[s->size]='\0';
    return realsize;
}

const char* getoneprayer(const char *prayername, json_object *timingsObj){
    json_object *prayerObj = NULL;
    json_object_object_get_ex(timingsObj, prayername, &prayerObj);
    // printf("%s", json_object_get_string(prayerObj));
    return json_object_get_string(prayerObj);
}
int get_prayertimes(struct String chunk, char prayers[5][6], char prayernames[5][10]){
    json_object *dataObj = NULL;
    json_object *timingsObj = NULL;
    json_object *prayerObj = NULL;
    json_object *root = json_tokener_parse(chunk.data);
    if(!root){
        fprintf(stderr, "Failed to parse JSON:\n %s\n", chunk.data);
        free(chunk.data);
        return 1;
    }
    if (json_object_object_get_ex(root, "data", &dataObj)) {
        const char *name = json_object_get_string(root);
    }else{
        printf("Field 'name not found.\n");
    }
    
    if (json_object_object_get_ex(dataObj, "timings", &timingsObj)) {
        const char *name = json_object_get_string(timingsObj);
    }else{
        printf("Field 'name not found.\n");
    }
    for(int i=0;i<5;i++){
        snprintf(prayers[i], sizeof prayers[0], "%s", getoneprayer(prayernames[i], timingsObj));
    }
    json_object_put(root);
    return 0;
}
void create_url(char *url, int urlsize,int argc, char *argv[]){
    char url_date_arg[11];
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    snprintf(url_date_arg, sizeof(url_date_arg), "%02d-%02d-%04d", t->tm_mday, t->tm_mon + 1, t->tm_year + 1900);
    snprintf(url, urlsize, "%s%s%s%s%s%s", "https://api.aladhan.com/v1/timingsByAddress/", url_date_arg, "?address=", argv[1], ",%20", argv[2]);
}

struct String get_json(CURL *curl, char *url){
    if (!curl){
        fprintf(stderr, "curl_easy_init failed\n");
        //return 1;
    }
    struct String chunk = {0};
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, ( void * )&chunk);
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK){
        fprintf(stderr, "curl_easy_perform failed: %s\n", curl_easy_strerror(res));
        curl_easy_cleanup(curl);
        free(chunk.data);
        // return 1;
    }
    return chunk;
}
int main(int argc, char *argv[]){
    char url[258];
    create_url(url, sizeof(url), argc, argv);
    printf("%s\n", url);

    CURL *curl = curl_easy_init();
    struct String chunk = get_json(curl, url);
    char prayernames[5][10] = { "Fajr", "Dhuhr", "Asr", "Maghrib", "Isha"};
    char prayers[5][6];
    get_prayertimes(chunk, prayers, prayernames);
    for(int i=0;i<5;i++){
        printf("%s:\t%s\n",prayernames[i], prayers[i]);
    }
    curl_easy_cleanup(curl);
    free(chunk.data);
    return 0;
}
