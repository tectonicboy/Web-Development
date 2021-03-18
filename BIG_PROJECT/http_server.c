#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

#define MESSAGE_BUFFER_SIZE 1024
#define TIME_SECOND 1000000

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

//Very specific for this system, not generalized at all.
unsigned Extract_HTTP_Variables(char* client_message, char* serverside_variables){
	int counter = 0;
        unsigned i = 0;
        unsigned offset = 0;
	int found = 1;
    	while((i < 1024) && !((client_message[i] == 'a') && (client_message[i+1] == 'c') && (client_message[i+2] == 't')
	 && (client_message[i+3] == 'i') && (client_message[i+4] == 'o') 
	 && (client_message[i+5] == 'n') && (client_message[i+6] == '=')))
    	{
    		++i;
    	}
	if(i < 1024){printf("Found the position at which server side variables begin: i = %d\n", i);}
    	else {printf("No server side variables in this http request.\n"); found = 0;}
	if(found){
	    	for(unsigned j = i; client_message[j] != '\0';){
	    		++counter;
	    		if((client_message[j] == '%')){
	    			serverside_variables[j - i - offset] = hexdec(client_message[j+1], client_message[j+2]);
	    			j += 3;
				offset += 2;
	    		}
			if(client_message[j] == '&'){
				serverside_variables[j - i - offset] = '-';
				++j;
			}
	    		else{
	    			serverside_variables[j - i - offset] = client_message[j];
	    			++j;
	    		}
			if(client_message[j] == '\0'){
				serverside_variables[j - i - offset] = '\n';
			}
			
	    	}
	}
    	if(counter > 0){printf("Looped in server side variables string for %d times before encountering null character\n", counter);}
	return i;
}

void SQL_Command_Constructor(char* vars, FILE* cmd_output){
	char email[32], phone[32], pass[32], db_name[256] = "USERS_DB-", tbl_name[32] = "People-",
		insert_into[16] = "INSERT-INTO-", vals[8] = "VALS-";
	char* ptr = NULL;
	char* SQL_command;
	memset(email, '\0', sizeof(email));
	memset(phone, '\0', sizeof(phone));
	memset(pass, '\0', sizeof(pass));
	unsigned i = 0, j = 0, var_counter = 0;
	l1:
	while((vars[i] != '=') && ((vars[i] != '\0'))){++i;}
	++i;
	switch(var_counter){
		case 1:
			ptr = email;
			break;
		case 2:
			ptr = phone;
			break;
		case 3:
			ptr = pass;
			break;
		default:
			break;
	}
	while((vars[i] != '-') && (vars[i] != '\0')){
		if(var_counter == 0){++i;}
		else{
			*(ptr + j) = vars[i];
			if((vars[i+1] == '-') && (var_counter < 3)){
				*(ptr + j + 1) = vars[i+1];
			}
			++i;
			++j;
		}
	}
	++i;
	j = 0;
	if(var_counter < 3){ 
		++var_counter;
		goto l1; 
	}
	printf("Extracted SQL command strings: email=%s, phone=%s, pass=%s\n", email, phone, pass);
	asprintf(&SQL_command, "%s%s%s%s%s%s%s", db_name, insert_into, tbl_name, vals, email, phone, pass);
	printf("Constructed the following custom SQL command: %s\n", SQL_command);
	fprintf(cmd_output, SQL_command);
	printf("Successfully wrote the SQL command to the file!\n");
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
	char* bgimg_name = "pic5.jpg"; char* topicon_name = "favicon.ico";
	char* jpgftype = "image/jpg"; char* icotype = "image/x-icon";

	char* bgimg_response_buf;
	size_t bgimg_resp_buf_siz = Create_HTTPsend_Filebuf(bgimg_name, jpgftype, "rb", &bgimg_response_buf);

	char* ico_response_buf;
	size_t ico_response_buf_siz = Create_HTTPsend_Filebuf(topicon_name, icotype, "rb", &ico_response_buf);

    	//Creating HTTP response buffer for webpage code.
	char* webpage_code;
	if(posix_memalign((void*)&webpage_code, 64, 65536) != 0){printf("HTML buf mem alloc failed.\n");}
	char* http_header = "HTTP/1.1 200 OK\r\n\n";
	char* fname = "index.html";
    	size_t f_siz = ReadFile(fname, &webpage_code, "r");
	char* HTTP_response;
    	if((posix_memalign((void*)&HTTP_response, 64, (strlen(http_header) + strlen(webpage_code) + 1))) != 0){printf("HTTP response buf mem alloc fail\n");}
    	strcpy(HTTP_response, http_header);
    	strcat(HTTP_response, webpage_code);



    	//Creating the server socket, binding it to the machine's IP address and making it listen on port 80.
	int server_socket, port = 80;
    	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address;
    	server_address.sin_family = AF_INET;
    	server_address.sin_port = htons(port);
    	server_address.sin_addr.s_addr = INADDR_ANY;
	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));
	listen(server_socket, 5);



	//Declarations to be used in serving loop.
    	char* serverside_variables;
    	if((posix_memalign((void*)&serverside_variables, 64, 0.25*MESSAGE_BUFFER_SIZE) ) != 0){printf("HTTP vars buf mem alloc fail.\n");}

    	char* requested_fname;
    	if(posix_memalign((void*)&requested_fname, 64, 64) != 0){printf("Req. fname buf mem alloc fail.\n");}

	char* client_message;
    	if((posix_memalign((void*)&client_message, 64, MESSAGE_BUFFER_SIZE) ) != 0){printf("HTTP request buf mem alloc fail\n");}

	FILE* sql_command_file;
	unsigned servervars_start_i;
	struct sockaddr_in client_address;
    	int client_socket, sent; //clientLen = sizeof(struct sockaddr_in);
	socklen_t clientLen = sizeof(struct sockaddr_in);
    	u_int32_t imageResponseLen;
   	size_t k;
    	//We wrap in an infinite while loop so the server can continue serving and responding to requests forever.
    	while(1){
		sent = 0;
		k = 5;
		memset(client_message, 0x0, MESSAGE_BUFFER_SIZE);
    		memset(serverside_variables, 0x0, 0.25*MESSAGE_BUFFER_SIZE);
		memset(requested_fname, 0x0, 64);

    		printf("\n\nListening for HTTP requests on port %d...\n", port);
		
        	client_socket = accept(server_socket, (struct sockaddr*) &client_address, (socklen_t*) &clientLen);
    		if(recv(client_socket, client_message, 1024, 0) < 0){
    			printf("Something went wrong while receiving client's HTTP request.\n");
    			break;
    		}
    		printf("\n****** Received an HTTP request ****** \n%s\n", client_message);
		printf("The size of the HTTP request is: %lu\n", strlen(client_message));
		while(!(client_message[k+1] == 72 && client_message[k+2] == 84 
			&& client_message[k+3] == 84 && client_message[k+4] == 80))
		{
			requested_fname[k-5] = client_message[k];
			++k;
			if(k > 64){break;}
		}
		if(strlen(requested_fname)){
			printf("The client requested a file called: %s\n", requested_fname);
			if(!strcmp(bgimg_name, requested_fname)){
				printf("About to send() contents of a buffer of size: %lu\n", bgimg_resp_buf_siz);
				if(send(client_socket, bgimg_response_buf, bgimg_resp_buf_siz, 0) < 0){
					printf("bgimg sending failed.\n");
				}
				else{
					printf("Sent file %s to the client successfully!\n", bgimg_name);
					sent = 1;
				}
			}
			else if(!strcmp(topicon_name, requested_fname)){
				printf("About to send() contents of a buffer of size: %lu\n", ico_response_buf_siz);
				if(send(client_socket, ico_response_buf, ico_response_buf_siz, 0) < 0){
					printf("favicon sending failed.\n");
				}
				else{
					printf("Sent file %s to the client successfuly!\n", topicon_name); 
					sent = 1;
				}
			}
		}
		printf("After dealin with requested file name, sent = %d\n", sent);

		servervars_start_i = Extract_HTTP_Variables(client_message, serverside_variables);
		if(strlen(serverside_variables)){
			printf("Extracted HTTP variables string: %s\n", serverside_variables);
		}
		if(servervars_start_i < MESSAGE_BUFFER_SIZE){
			l2:
	    	sql_command_file = fopen("SERVER_SQL_COMMANDS.txt", "a");
			if(!(sql_command_file)){
				printf("Unable to open SQL command file for appending. Retrying...\n");
				usleep(0.2 * TIME_SECOND);
				goto l2;
			}
			SQL_Command_Constructor(serverside_variables, sql_command_file);
			fclose(sql_command_file);	
		}
        	if(!sent){
			printf("About to send webpage code.\n");
			if(send(client_socket, HTTP_response, strlen(HTTP_response), 0) < 0){
	    			printf("html/css/js code sending failed.\n");
	    		}
		}
		close(client_socket);
    	}
    printf("About to return 0 and exit.\n");
    return 0;
}
