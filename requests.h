// Pislari Vadim 323CB Tema 3 PC

#ifndef _REQUESTS_
#define _REQUESTS_


char *compute_get_request(char *host, char *url, char *url_params);
char *compute_request(char* method, char *host, char *url, char *type, char *form_data, char cookies[MAX_COOK][STR_DIM], 
	int nmb_of_cookies, char *command, char *jwt_par);

#endif