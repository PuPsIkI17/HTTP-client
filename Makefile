CC=gcc
CFLAGS=-I.

client: client.c requests.c helpers.c parson.c
		$(CC) -o client parson.c client.c requests.c helpers.c 
		
run: client
		./client

clean:
		rm -f *.o client