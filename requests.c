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


char *compute_request(char* method, char *host, char *url, char *type, char *form_data, char cookies[MAX_COOK][STR_DIM], 
    int nmb_of_cookies, char *command,char *jwt_par){

    if(!method || !host || !url)  return NULL;
    char *message = calloc(BUFLEN, sizeof(char));
    char *line = calloc(LINELEN, sizeof(char));

    // Se scrienumele metodei, calea si tipul protocolului
    if (command != NULL){
        // task 3
        sprintf(line, "%s %s?%s HTTP/1.1", method,url,command);
    }
    else{
        sprintf(line, "%s %s HTTP/1.1",method, url);
    }
    compute_message(message, line);

    // Se adauga host-ul
    sprintf(line, "HOST: %s", host);
    compute_message(message, line);

    // se realizeaza autorizarea cu jwt-ul
    if(jwt_par){
        sprintf(line, "Authorization: Bearer %s", jwt_par);
        compute_message(message, line);
    }

    // se adauga headere
    if(form_data){
        sprintf(line, "Content-Length: %ld", strlen(form_data));
        compute_message(message, line);
    }
    // se adauga tipul
    if(type){
        sprintf(line, "Content-Type: %s", type);
        compute_message(message, line);
    }

    int i;
    // adaugarea cookies 
    for(i = 0; i != nmb_of_cookies; i++){
        sprintf(line, "Cookie: %s", cookies[i]);
        compute_message(message, line);   
    }

    // Se adauga linia de final
    strcpy(line, "\r\n");
    strcat(message, line);
    
    // se adauga datele
    if(form_data){
        strcpy(line, form_data);
        compute_message(message, line);
    }

    free(line);
    return message;
}