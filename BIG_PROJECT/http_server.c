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
        printf("Could'nt open file\n");
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

    *buffer = calloc(1, numBytes+1);
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
	    		if((client_message[j] == '%') && (client_message[j+1] == '4') && (client_message[j+2] == '0')){
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
			printf("About to check the character: %c, following to write in serverside_variables[%d-%d]\n", client_message[j], j, i);
			
	    	}
	}
    	if(counter > 0){printf("Looped in server side variables string for %d times before encountering null character\n", counter);}
	return i;
}

void SQL_Command_Constructor(char* vars, FILE* cmd_output){
	char email[32], phone[32], pass[32], db_name[256] = "USERS_DB-", tbl_name[32] = "People-",
		insert[8] = "INSERT-", into[8] = "INTO-", vals[8] = "VALS-";
	char* ptr = NULL;
	memset(email, '\0', sizeof(email));
	memset(phone, '\0', sizeof(phone));
	memset(pass, '\0', sizeof(pass));
	unsigned i = 0, j = 0, var_counter = 0;
	l1:
	while((vars[i] != '=') && ((vars[i] != '\0'))){++i;}
	printf("Found a = at i = %d\n", i);
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
			printf("Setting *(ptr + %d) to character:%c\n", j, vars[i]);
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
	strcat(db_name, insert); strcat(db_name, into); strcat(db_name, tbl_name);
	strcat(db_name, vals); strcat(db_name, email); strcat(db_name, phone); strcat(db_name, pass);
	printf("Constructed the following custom SQL command: %s\n", db_name);
	fprintf(cmd_output, db_name);
	printf("Successfully wrote the SQL command to the file!\n");
	return;
}

int main(){
    int server_socket, client_socket, clientLen, read_size, port = 80;
    char* r_mode = "r"; char* rb_mode = "rb";

    char* http_header_bgimg = "HTTP/1.1 200 OK\n\rContent-Type: image/jpg\n\rContent-Length: 31623\r\n\n";
    char* bgimg_buf = calloc(1, 31623);
    char* bgimg_name = "pic5.jpg";
    size_t bgimg_siz = ReadFile(bgimg_name, &bgimg_buf, rb_mode);
    char* bgimg_response_buf = calloc(1, (strlen(http_header_bgimg) + bgimg_siz));  
    strcpy(bgimg_response_buf, http_header_bgimg);
    memcpy((bgimg_response_buf + strlen(http_header_bgimg)), bgimg_buf, bgimg_siz);

    char* http_header = "HTTP/1.1 200 OK\r\n\n";
    char* response_data;

    if(posix_memalign((void*)&response_data, 64, 65536) != 0){
	printf("Unable to allocate 64-byte aligned memory for html buffer.\n");
    }
    char* client_message;
    if((posix_memalign((void*)&client_message, 64, MESSAGE_BUFFER_SIZE) ) != 0){
        printf("Unable to allocate 64-byte aligned memory for client HTTP request buffer.\n");
    }	

    struct sockaddr_in server_address, client_address;
    char* fname = "index.html";
    size_t f_siz = ReadFile(fname, &response_data, r_mode);

    printf("We have read file contents: %s\n", response_data);

    //Combine the header and the actual body by concatenating into one string.
    char* HTTP_response;
    if((posix_memalign((void*)&HTTP_response, 64, (strlen(http_header) + strlen(response_data) + 1))) != 0){
	printf("Unable to allocate 64-byte aligned memory for HTTP response buffer.\n");
    }
    strcpy(HTTP_response, http_header);
    strcat(HTTP_response, response_data);
    //create a socket

    //same params as TCP one.
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //we define the address again. Defines where our server is going to serve the data.
    server_address.sin_family = AF_INET;
    //convert the port to the right format for this structure with htons.
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 5);

    FILE* sql_command_file;
    char* serverside_variables;
    if((posix_memalign((void*)&serverside_variables, 64, MESSAGE_BUFFER_SIZE) ) != 0){
	printf("Unable to allocate 64-byte aligned memory for serverside variables buffer.\n");
    }
    unsigned servervars_start_i;
    char* requested_fname;
    if(posix_memalign((void*)&requested_fname, 64, 64) != 0){
	printf("Unable to allocate 64-byte aligned memory for http requested file name buffer.\n");
    }
    int sent;
    u_int32_t imageResponseLen;
    size_t k;
    //We wrap in an infinite while loop so the server can continue serving and responding to requests forever.
    while(1){
	sent = 0;
    	printf("\n\nListening for HTTP requests on port %d...\n", port);
    	clientLen = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr*) &client_address, (socklen_t*) &clientLen);
	memset(client_message, 0x0, MESSAGE_BUFFER_SIZE);
    	memset(serverside_variables, 0x0, MESSAGE_BUFFER_SIZE);
	memset(requested_fname, 0x0, 64);
    	if(recv(client_socket, client_message, 1024, 0) < 0){
    		printf("Something went wrong while receiving client's HTTP request.\n");
    		break;
    	}
    	printf("\n****** Received an HTTP request ****** \n%s\n", client_message);
        k = 5;
	while(!(client_message[k+1] == 'H' && client_message[k+2] == 'T' 
		&& client_message[k+3] == 'T' && client_message[k+4] == 'P'))
	{
		requested_fname[k-5] = client_message[k];
		++k;
		if(k > 64){break;}
	}
	if(strlen(requested_fname)){
		printf("The client requested a file called: %s\n", requested_fname);
		if(!strcmp(bgimg_name, requested_fname)){
			imageResponseLen = (strlen(http_header_bgimg) + bgimg_siz);
			if(send(client_socket, bgimg_response_buf, imageResponseLen, 0) < 0){
				printf("bgimg sending failed.\n");
			}
			else{sent = 1;}
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
