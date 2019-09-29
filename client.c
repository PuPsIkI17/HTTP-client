// Pislari Vadim 323CB Tema 3 PC

#include <stdlib.h>     /* exit, atoi, malloc, free */
#include <stdio.h>
#include <unistd.h>     /* read, write, close */
#include <string.h>     /* memcpy, memset */
#include <sys/socket.h> /* socket, connect */
#include <netinet/in.h> /* struct sockaddr_in, struct sockaddr */
#include <netdb.h>      /* struct hostent, gethostbyname */
#include <arpa/inet.h>
#include "helpers.h"
#include "requests.h"
#include "parson.h"


int main(int argc, char *argv[]){
    int i,j;
    char *message;
    char *response;
    int sockfd;
    char cookies[MAX_COOK][STR_DIM]; // pastreaza cookies
    int nmb_of_cookies = 0; // numarul de cookies

    // task 1
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_request("GET",IP_SERVER, "/task1/start", NULL,NULL,cookies,nmb_of_cookies,NULL,NULL);
    if (!message){
        printf("eroare\n");
        return 1;
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);


/////////////////////////////////////////////////////////////////////////////////////////////////
    // task 2
    char find_cookie[STR_DIM];
    int l = strlen(response);
    i = 0;

    // identificarea cookies si a mesajului JSON
    while(response[i] != '{'){
        if(i >= l){
            printf("eroare\n");
            return 1;
        }
        memset(find_cookie,0,STR_DIM);
        strncpy(find_cookie,response+i, 11);
        if(strcmp(find_cookie,"Set-Cookie:") == 0){
            j = 0;
            while(response[i+12+j] != ';')j++;
            strncpy(cookies[nmb_of_cookies],response + i + 12,j);
            nmb_of_cookies++;
        }
        i++;
    }

    // parsarea mesajului JSON si crearea obiectului JSON_VALUE
    JSON_Value *val = json_parse_string(response + i);  
    if (val == NULL) {  
        return 0;
    }
    JSON_Object *object = json_value_get_object(val);
    if(object == NULL){
        printf("eroare");
        return 1;
    }

    //identificarea obiectului data
    JSON_Object *sec = json_object_get_object(object, "data");
    if(sec == NULL){
        printf("eroare\n");
        return 1;
    }

    // formarea username=xxx&password=yyy
    char *post_string = malloc(sizeof(char) * STR_DIM);
    JSON_Value *value;  
    if(json_object_get_count(sec) != 2){
        printf("eroare\n");
        return 1;
    }

    // identifiarea cuvantului "login" sau altceva ce substituie login
    strcat(post_string, json_object_get_name(sec, 0));
    strcat(post_string,"=");

    // identificarea loginului
    value = json_object_get_value_at(sec, 0);
    strcat(post_string,json_value_get_string (value));
    strcat(post_string,"&&&");

    // identifiarea cuvantului "password" sau altceva ce substituie password
    strcat(post_string, json_object_get_name(sec, 1));
    strcat(post_string,"=");

    //identificarea passwordului
    value = json_object_get_value_at(sec, 1);
    strcat(post_string,json_value_get_string (value));

    // identificarea celorlali parametri din JSON
    char *method = malloc(sizeof(char) * STR_DIM);
    strcpy(method, json_object_get_string(object,"method"));
    char *url = malloc(sizeof(char) * STR_DIM);
    strcpy(url, json_object_get_string(object,"url"));
    char *type = malloc(sizeof(char) * STR_DIM);
    strcpy(type, json_object_get_string(object,"type"));

    // transmiterea mesajului
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_request(method, IP_SERVER, url, type, post_string, cookies, nmb_of_cookies,NULL,NULL);
    if(message == NULL){
        printf("eroare\n");
        return 1;
    }

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // task 3
    l = strlen(response);
    i = 0;
    nmb_of_cookies = 0;

    // identificarea cookies si a mesajului JSON
    while(response[i] != '{'){
        if(i >= l){
            printf("eroare\n");
            return 1;
        }
        memset(find_cookie,0,STR_DIM);
        strncpy(find_cookie,response+i, 11);
        if(strcmp(find_cookie,"Set-Cookie:") == 0){
            j = 0;
            while(response[i+12+j] != ';')j++;
            memset(cookies[nmb_of_cookies],0,STR_DIM);
            strncpy(cookies[nmb_of_cookies],response + i + 12,j);
            nmb_of_cookies++;
        }
        i++;
    }

    // parsarea
    val = json_parse_string(response + i);  
    if (val == NULL) {  
        return 0;
    }

    object = json_value_get_object(val);
    if(object == NULL){
        printf("eroare");
        return 1;
    }

    //identificarea obiectului data
    sec = json_object_get_object(object, "data");    
    if(sec == NULL){
        printf("eroare\n");
        return 1;
    }

    //identificarea obiectului data
    JSON_Value *jwt = json_object_get_value_at(sec, 0);
    char *jwt_par = malloc(sizeof(char)*STR_DIM);
    strcpy(jwt_par,json_value_get_string(jwt));

    sec = json_object_get_object(sec, "queryParams");
    if(sec == NULL){
        printf("eroare\n");
        return 1;
    }

    //raspuns1=xxx&raspuns2=yyy&id=zzz
    char command[STR_DIM] = "raspuns1=omul&raspuns2=numele&";
    strcat(command, json_object_get_name(sec, 0));
    strcat(command,"=");

    // identificarea loginului
    value = json_object_get_value_at(sec, 0);
    strcat(command,json_value_get_string (value));

    // identificarea celorlali parametri din JSON
    memset(method,0,STR_DIM);
    strcpy(method, json_object_get_string(object,"method"));
    memset(url,0,STR_DIM);
    strcpy(url, json_object_get_string(object,"url"));

    // transmiterea mesajului
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_request(method, IP_SERVER, url, NULL, NULL, cookies, nmb_of_cookies,command,jwt_par);
    if(message == NULL){
        printf("eroare\n");
        return 1;
    }

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // task 4
    l = strlen(response);
    i = 0;
    nmb_of_cookies = 0;
    
    // identificarea cookies si a mesajului JSON
    while(response[i] != '{'){
        if(i >= l){
            printf("eroare\n");
            return 1;
        }
        memset(find_cookie,0,STR_DIM);
        strncpy(find_cookie,response+i, 11);
        if(strcmp(find_cookie,"Set-Cookie:") == 0){
            j = 0;
            while(response[i+12+j] != ';')j++;
            memset(cookies[nmb_of_cookies],0,STR_DIM);
            strncpy(cookies[nmb_of_cookies],response + i + 12,j);
            nmb_of_cookies++;
        }
        i++;
    }

    // parsarea
    val = json_parse_string(response + i);  
    if (val == NULL) {  
        return 0;
    }

    object = json_value_get_object(val);
    if(object == NULL){
        printf("eroare");
        return 1;
    }

    // identificarea celorlali parametri din JSON
    memset(method,0,STR_DIM);
    strcpy(method, json_object_get_string(object,"method"));
    memset(url,0,STR_DIM);
    strcpy(url, json_object_get_string(object,"url"));

    // transmiterea mesajului
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_request(method, IP_SERVER, url, NULL, NULL, cookies, nmb_of_cookies,NULL,jwt_par);
    if(message == NULL){
        printf("eroare\n");
        return 1;
    }

    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    close_connection(sockfd);

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
    // task 5
    l = strlen(response);
    i = 0;
    nmb_of_cookies = 0;

    // identificarea cookies si a mesajului JSON
    while(response[i] != '{'){
        if(i >= l){
            printf("eroare\n");
            return 1;
        }
        memset(find_cookie,0,STR_DIM);
        strncpy(find_cookie,response+i, 11);
        if(strcmp(find_cookie,"Set-Cookie:") == 0){
            j = 0;
            while(response[i+12+j] != ';')j++;
            memset(cookies[nmb_of_cookies],0,STR_DIM);
            strncpy(cookies[nmb_of_cookies],response + i + 12,j);
            nmb_of_cookies++;
        }
        i++;
    }   

    char vector[LINELEN];
    strcpy(vector,response+i);
    val = json_parse_string(vector);
    if (val == NULL) {  
        printf("eroare\n");
        return 0;
    }

    object = json_value_get_object(val);
    if(object == NULL){
        printf("eroare");
        return 1;
    }

    object = json_value_get_object(val);
    if(object == NULL){
        printf("eroare");
        return 1;
    }

    //identificarea obiectului data
    sec = json_object_get_object(object, "data");
    if(sec == NULL){
        printf("eroare\n");
        return 1;
    }

    char *new_url = malloc(sizeof(char) * STR_DIM);
    char *new_method = malloc(sizeof(char) *STR_DIM);
    char new_params[STR_DIM];
    value = json_object_get_value_at(sec, 0);
    strcpy(new_url,json_value_get_string (value));
    value = json_object_get_value_at(sec, 2);
    strcpy(new_method,json_value_get_string (value));
    value = json_object_get_value_at(sec, 1);
    JSON_Object *new_obj = json_value_get_object(value);

    // formarea parametrelor
    strcat(new_params, json_object_get_name(new_obj, 0));
    strcat(new_params,"=");
    value = json_object_get_value_at(new_obj, 0);
    strcat(new_params,json_value_get_string (value));
    strcat(new_params,"&");
    strcat(new_params, json_object_get_name(new_obj, 1));
    strcat(new_params,"=");
    value = json_object_get_value_at(new_obj, 1);
    strcat(new_params,json_value_get_string (value));

    // determinrea url-uluii
    char ip[100],hostname[100],url_fin[100];
    i = 0;
    while(new_url[i] != '/') i++;
    strncpy(hostname, new_url, i);
    strcpy (url_fin,new_url + i);
    
    // identificarea ip-ului 
    struct hostent *host = gethostbyname(hostname);     
    struct in_addr **addr;      
    if (!host){
        return 1;
    }     

    addr = (struct in_addr**) host->h_addr_list;
    for(i = 0; addr[i] != NULL; i++){   
        strcpy(ip , inet_ntoa(*addr[i]));
        break;
    }

    // transmiterea mesajului
    sockfd = open_connection(ip, 80, AF_INET, SOCK_STREAM, 0);
    message = compute_request(new_method, ip, url_fin, NULL, NULL, NULL, 0, new_params, NULL);
    if(message == NULL){
        printf("eroare\n");
        return 1;
    }

    // determinarea json-ului
    send_to_server(sockfd, message);
    char *json = receive_from_server(sockfd);
    puts(json);
    close_connection(sockfd);
    while(*json != '{'){
        json++;
    }

    memset(method,0,STR_DIM);
    strcpy(method, json_object_get_string(object,"method"));
    memset(url,0,STR_DIM);
    strcpy(url, json_object_get_string(object,"url"));
    memset(type,0,STR_DIM);
    strcpy(type, json_object_get_string(object,"type"));
    
    // transmiterea mesajului
    sockfd = open_connection(IP_SERVER, PORT_SERVER, AF_INET, SOCK_STREAM, 0);
    message = compute_request(method, IP_SERVER, url, type, json, cookies, nmb_of_cookies,NULL,jwt_par);
    if(message == NULL){
        printf("eroare\n");
        return 1;
    }
    send_to_server(sockfd, message);
    response = receive_from_server(sockfd);
    puts(response);
    free(response);
    free(method);
    free(url);
    free(type);
    close_connection(sockfd);
    return 0;
}