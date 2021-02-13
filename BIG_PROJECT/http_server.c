#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <netinet/in.h>

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

void Extract_HTTP_Variables(char* client_message, char* serverside_variables){
	int counter = 0;
        unsigned i = 0;
        unsigned offset = 0;
    	while(!((client_message[i] == 'a') && (client_message[i+1] == 'c') && (client_message[i+2] == 't')
	 && (client_message[i+3] == 'i') && (client_message[i+4] == 'o') && (client_message[i+5] == 'n') && (client_message[i+6] == '=')) && (i < 1024))
    	{
    		++i;
    	}
    	printf("Found the position at which server side variables begin: i = %d\n", i);
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
    	printf("Looped in server side variables string for %d times before encountering null character\n", counter);
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
    char http_header[16384] = "HTTP/1.1 200 OK\r\n\n";
    char response_data[8192];
    char client_message[1024] = {0};
    struct sockaddr_in server_address, client_address;

    FILE* html_data;
    html_data = fopen("index.php", "r");

    //read the contents of the file into this response string.
    fgets(response_data, 8192, html_data);
    strcat(http_header, response_data);

    //Create a socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    //Defines where our server is going to serve the data.
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

    listen(server_socket, 5);

    FILE* sql_command_file;
    char serverside_variables[1024];

    //We wrap in an infinite while loop so the server can continue serving and responding to requests forever.
    while(1){
    	printf("Listening for HTTP requests on port %d...\n", port);
    	clientLen = sizeof(struct sockaddr_in);
        client_socket = accept(server_socket, (struct sockaddr*) &client_address, (socklen_t*) &clientLen);

    	memset(client_message, '\0', sizeof(client_message));
    	memset(serverside_variables, '\0', sizeof(serverside_variables));
	    
    	if(recv(client_socket, client_message, sizeof(client_message), 0) < 0){
    		printf("Something went wrong while receiving client's HTTP request.\n");
    		break;
    	}   
    	printf("Client reply: %s\n", client_message);
	    
    	sql_command_file = fopen("SERVER_SQL_COMMANDS.txt", "a");
	if(!(sql_command_file)){printf("Unable to open SQL command file.\n");}
	    
	Extract_HTTP_Variables(client_message, serverside_variables);
	printf("After extracting HTTP variables, the string looks like this: %s\n", serverside_variables);
	    
	SQL_Command_Constructor(serverside_variables, sql_command_file);
    	fclose(sql_command_file);
	    
        if(send(client_socket, http_header, sizeof(http_header), 0) < 0){
    		printf("Send failed.\n");
    	}
        close(client_socket);
    }

    return 0;
}
