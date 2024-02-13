#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>

int parseXML(const char *xml_string, char **rresult) {
    xmlDoc *doc = NULL;
    xmlNode *root_element = NULL;

    doc = xmlReadMemory(xml_string, strlen(xml_string), NULL, NULL, 0);
    if (doc == NULL) {
        printf("Ошибка при парсинге XML из строки.\n");
        return 1;
    }

    root_element = xmlDocGetRootElement(doc);

    xmlNode *cur_node = NULL;
    char *host = NULL;
    char *path = NULL;
    char *sign = NULL;
    char *ts = NULL;

    for (cur_node = root_element->children; cur_node; cur_node = cur_node->next) {
        if (cur_node->type == XML_ELEMENT_NODE) {
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"host") == 0) {
                xmlNode *child_node = cur_node->children;
                host = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"path") == 0) {
                xmlNode *child_node = cur_node->children;
                path = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"s") == 0) {
                xmlNode *child_node = cur_node->children;
                sign = (char *)xmlNodeGetContent(child_node);
            }
            if (xmlStrcmp(cur_node->name, (const xmlChar *)"ts") == 0) {
                xmlNode *child_node = cur_node->children;
                ts = (char *)xmlNodeGetContent(child_node);
            }
        }
    }

    if (host && path) {
        sprintf(*rresult, "https://%s/get-mp3/%s/%s%s", host, sign, ts, path);
    }

    xmlFree(host);
    xmlFree(path);
    xmlFree(sign);
    xmlFree(ts);
    xmlFreeDoc(doc);
    xmlCleanupParser();
    return 0;
}

int main() {
    char *input_string = malloc(400);
    input_string[0] = '\0';

    const char *xml_string = "<download-info><host>s286myt.storage.yandex.net</host><path>/rmusic/U2FsdGVkX1-54MNGKKHVgxAnmbQ4iNqmed7CBEbGdEx_YdGa8Rsm2mRvJkNBhl6TOHGtG4wC8CJcuF-7iu66_WxuPvCTNT4VaH9-Men-tNQ/ce62d633ebf608b9298fa36b03ada4774afd30bd7c6ece90a5f20b538cf2cbfd</path><ts>00061130c48de3ab</ts><region>-1</region><s>ce62d633ebf608b9298fa36b03ada4774afd30bd7c6ece90a5f20b538cf2cbfd</s></download-info>";
    parseXML(xml_string, &input_string);

    return 0;
}