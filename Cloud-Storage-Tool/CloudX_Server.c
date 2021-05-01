#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/un.h>

#define memalign posix_memalign
#define TIME_SECOND 1000000

long long hexdec(char first, char second){
	char hex[3];
	long long decimal, place;
	int i = 0, val, len;

	decimal = 0;
	place = 1;

	hex[0] = first; hex[1] = second; hex[2] = '\0';

	len = strlen(hex);
	len--;

	for(i=0; hex[i]!='\0'; i++){
		if(hex[i]>='0' && hex[i]<='9'){
			val = hex[i] - 48;
		}
		else if(hex[i]>='a' && hex[i]<='f'){
			val = hex[i] - 97 + 10;
		}
		else if(hex[i]>='A' && hex[i]<='F'){
			val = hex[i] - 65 + 10;
		}
		decimal += val * pow(16, len);
		len--;
	}
	return decimal;
}

size_t ReadFile(char* filePath, char** buffer, char* mode) {
    	FILE *fd = fopen(filePath, mode);

    if (!fd) {
        printf("Couldn't open file\n");
        return 0;
    }
    fseek(fd, 0, SEEK_END);
    size_t numBytes = ftell(fd);
    printf("Read the file %s, and its size in bytes is: %lu\n", filePath, numBytes);
    fseek(fd, 0, SEEK_SET);

    if (numBytes == 0) {
        fclose(fd);
        printf("File is empty:\t%s\n", "multiline_file.txt");
        return 0;
    }

    *buffer = calloc(1, numBytes);
    assert(*buffer);

    int read = fread(*buffer, numBytes, 1, fd);
    assert(read);
    fclose(fd);

    return numBytes;
}

unsigned Extract_HTTP_Variables(char* client_message, char* serverside_variables){
	int counter = 0, found = 1;
        unsigned i = 0, offset = 0;
    	while((i < 2048) && !((client_message[i] == 'a') && (client_message[i+1] == 'c') && (client_message[i+2] == 't')
	 && (client_message[i+3] == 'i') && (client_message[i+4] == 'o') 
	 && (client_message[i+5] == 'n') && (client_message[i+6] == '=')))
    	{
    		++i;
    	}
	if(i < 2048){printf("Found the position at which server side variables begin: i = %d\n", i);}
    	else {printf("No server side variables in this http request.\n\n"); found = 0;}
	if(found){
	    	for(unsigned j = i; client_message[j] != '\0';){
			while(client_message[j] != '='){++j; ++counter; ++offset;}
			++j; ++counter; ++offset;
			while(client_message[j] != '&'){
				if((client_message[j] == '%')){
	    				serverside_variables[j - i - offset] = hexdec(client_message[j+1], client_message[j+2]);
	    				j += 3;
					offset += 2;
					++counter;
	    			}
		    		else{
		    			serverside_variables[j - i - offset] = client_message[j];
		    			++j; 
					++counter;
		    		}
				if(client_message[j] == '\0'){
					serverside_variables[j - i - offset] = '\n';
					printf("Extracted HTTP variables string: %s\n", serverside_variables);
					return i;
				}
			}
			serverside_variables[j - i - offset] = '-';
			++j; ++counter;
	    	}
	}
	return i;
}

void Generate_SQL_Command(char* vars){
	unsigned pos = 2;
	switch(vars[0]){
		case 'A':{
			char *socket_path = "\0hidden";
			char buf[128] = "CloudX_DB-CHECK-Users-name-";
			while(vars[pos] != '\n'){
				buf[pos - 2 + 27] = vars[pos]; 
				++pos;
			}
			buf[pos - 2 + 27] = '\0';
			printf("Constructed the following custom SQL command: %s\n", buf);
			
			int fd;
			struct sockaddr_un addr;
			fd = socket(AF_UNIX, SOCK_STREAM, 0);
			memset(&addr, 0x0, sizeof(addr));
			addr.sun_family = AF_UNIX;
			*addr.sun_path = '\0';
			strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
			connect(fd, (struct sockaddr*)&addr, sizeof(addr));
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, sizeof(buf));
			read(fd, buf, sizeof(buf));
			printf("The database system sent the following answer: %s\n", buf);
			close(fd);
			break;
		}
		default:{
			break;
		}	  
	}
	return;
}

u_int32_t Create_HTTPsend_Filebuf(char* fname, char* ftype, char* mode, char** buf){
    	char* http_header = "HTTP/1.1 200 OK\n\rContent-Type: ";
    	char* cont_len = "Content-Length: ";
    	char* fbuf;
    	size_t fsiz = ReadFile(fname, &fbuf, mode);
   	char* fullHeader;

   	asprintf(&fullHeader, "%s %s\r\n%s %lu\r\n\n", http_header, ftype, cont_len, fsiz);

    	*buf = calloc(1, (strlen(fullHeader) + fsiz));

    	memcpy(*buf, fullHeader, strlen(fullHeader));
    	memcpy(*buf + strlen(fullHeader), fbuf, fsiz);

	u_int32_t bufsiz = strlen(fullHeader) + fsiz;

    	free(fullHeader);
	return bufsiz;
}

int main(){
	char *cloudimg_name="resources/cloud.svg", *gearimg_name="resources/gear.svg", *favico_name="resources/favicon.ico", *firstpage_name = "index.html";
	char *jpgtype="image/jpg", *icotype="image/x-icon", *svgtype="image/svg+xml", *htmltype = "text/html";

	char *cloudimg_response_buf, *gearimg_response_buf, *favico_response_buf, *firstpage_response_buf;
	size_t cloudimg_resp_buf_siz = Create_HTTPsend_Filebuf(cloudimg_name, svgtype, "rb", &cloudimg_response_buf),
	       gearimg_response_buf_siz = Create_HTTPsend_Filebuf(gearimg_name, svgtype, "rb", &gearimg_response_buf),
	       favico_response_buf_siz = Create_HTTPsend_Filebuf(favico_name, icotype, "rb", &favico_response_buf),
	       firstpage_buf_siz = Create_HTTPsend_Filebuf(firstpage_name, htmltype, "r", &firstpage_response_buf); 

	//Create the server socket, bind a name to it and make it listen on port 80
	int port = 80, server_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = INADDR_ANY};
	bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
	listen(server_socket, 16);

	//Declarations for serving loop
	FILE *response;
	unsigned servervars_start_i;
	struct sockaddr_in client_address;
    	int client_socket, sent;
	socklen_t clientLen = sizeof(struct sockaddr_in);
    	u_int32_t imageResponseLen;
   	size_t k, db_response_siz;
	char *serverside_variables, *requested_fname, *client_message, *db_response;
	if(memalign((void*)&serverside_variables, 64, 256) || memalign((void*)&requested_fname, 64, 64) || memalign((void*)&client_message, 64, 2048)){return 0;}

	//Serving loop
	while(1){
		//Reset stuff.
		sent = 0; k = 5;
		memset(serverside_variables, 0x0, 256); memset(requested_fname, 0x0, 64); memset(client_message, 0x0, 2048);
		
		printf("Listening for HTTP requests on port %d...\n", port);
		client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientLen);
		if(recv(client_socket, client_message, 2048, 0) == -1){printf("Fault while receiving an HTTP request.\n"); break;}
		printf("Received an HTTP request of size %lu\n\n%s", strlen(client_message), client_message);

		while(!(client_message[k+1] == 72 && client_message[k+2] == 84 && client_message[k+3] == 84 && client_message[k+4] == 80)){
			requested_fname[k-5] = client_message[k];
			++k;
			if(k > 64){break;}
		}
		if(*requested_fname){
			printf("\nThe client requested a file called: %s\n", requested_fname);
			if(!strcmp(cloudimg_name, requested_fname)){
				if(send(client_socket, cloudimg_response_buf, cloudimg_resp_buf_siz, 0) < 0){printf("cloudimg send fail\n");}
				else{printf("Sent file %s to the client successfully!\n", cloudimg_name); sent = 1;}
			}
			else if(!strcmp(gearimg_name, requested_fname)){
				if(send(client_socket, gearimg_response_buf, gearimg_response_buf_siz, 0) < 0){printf("gearimg send fail\n");}
				else{printf("Sent file %s to the client successfully!\n", gearimg_name); sent = 1;}
			}
			else if(!strcmp(favico_name, requested_fname)){
				if(send(client_socket, favico_response_buf, favico_response_buf_siz, 0) < 0){printf("favico send fail");}
				else{printf("Sent file %s to the client successfully\n", favico_name); sent = 1;}
			}
		}
		printf("After dealing with requested file name, sent = %d\n", sent);

		servervars_start_i = Extract_HTTP_Variables(client_message, serverside_variables);
		if(servervars_start_i < 2048){
			Generate_SQL_Command(serverside_variables);
			usleep(0.3 * TIME_SECOND);
			db_response_siz = ReadFile("response.txt", &db_response,"r");
			printf("About to send back the following string: %s\n", db_response);
			if(send(client_socket, db_response, db_response_siz, 0) < 0){printf("firstpage code send fail\n");}
			else{printf("Sent db response successfully.\n"); sent = 1;}
			free(db_response);
		}

        	if(!sent){
			printf("About to send webpage code.\n\n");
			if(send(client_socket, firstpage_response_buf, firstpage_buf_siz, 0) < 0){printf("firstpage code send fail\n");}
		}
		close(client_socket);
	}	



	return 0;
}
