#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <netinet/in.h>

#define memalign posix_memalign

#define HEAP_ALIGN(TYPE, NAME, COUNT) \
	TYPE* NAME; \
	if(posix_memalign((void*)&NAME, 64, COUNT*sizeof(TYPE))){printf("aligned mem err\n"); return 0;}

#define SITE_FILES 13
#define MAX_FSIZ 4294970000

unsigned long long find_substr(char* str, char* substr, unsigned long long pos, unsigned long long max_index, size_t big_size){
	unsigned long long smallsiz = (unsigned long long)strlen(substr);
	printf("Entered find_substr with substr %s at pos %llu\n", substr, pos);
	char flags = 0b00000000;
	unsigned long long siz1 = (max_index - smallsiz), siz2 = (big_size - smallsiz);
	for(unsigned long long i = pos; i <= siz1 && i < siz2; ++i){
		//printf("INSPECTING: %d\t%c\n", (int)str[i], str[i]);
		flags |= 0b00000001;
		for(unsigned long long j = 0; j < smallsiz; ++j){
			if(str[i+j] != substr[j]){
				flags &= ~0b00000001; 
				break;
			}
		}
		if(flags & 0b00000001){return i;}
	}	
	printf("About to return max_index\n");
	return max_index;
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
		decimal += (val << (len << 2));
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
unsigned new_uploads = 0;
unsigned long long new_storage = 0;
void Generate_SQL_Command(char* vars, char* buf){
	unsigned pos = 2;
	char* socket_path = "\0hidden";
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un addr;
	memset(&addr, 0x0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	*addr.sun_path = '\0';
	strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
	connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	switch(vars[0]){
		case 'A':{
			strcat(buf, "1:CloudX_DB-CHECK-Users-name-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 29] = vars[pos]; 
				++pos;
			}
			strcat(buf, "-pass-unique_num\0");
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			read(fd, buf, 1024);
			printf("The database system sent the following answer: %s\n", buf);
			close(fd);
			break;
		}
		case 'C':{
			strcat(buf, "1:CloudX_DB-INSERT-INTO-Users-VALS-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 35] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 35] = '-';
			++pos;
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 35] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 35] = '-';
			++pos;
			time_t t;
		   	srand((unsigned) time(&t));
			unsigned char num_or_letter = round(rand() % 2), r1, r2;
			if(!num_or_letter){r1 = 48; r2 = 57;}
			else{r1 = 97; r2 = 122;}
			char db_uniqueid[7];
			for(size_t i = 0; i < 6; ++i){db_uniqueid[i] = (r1 + (rand() % (r2 - r1)));}
			db_uniqueid[6] = '\0';
			strcat(buf, db_uniqueid);
			strcat(buf, "-0-0/0/0-4294965097");			
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			for(size_t i = 0; i < 6; ++i){buf[i] = db_uniqueid[i];}
			close(fd);
			break;
		}
		case 'D':{
			strcat(buf, "1:CloudX_DB-ALTER-Users-WHERE-name-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 35] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 35] = '-';
			++pos;
			strcat(buf, "name-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 40] = vars[pos];
				++pos;
			}
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			strcat(buf, "yes");
			close(fd);
			break;
		}
		case 'E':{
			strcat(buf, "1:CloudX_DB-ALTER-Users-WHERE-unique_num-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 41] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 41] = '-';
			++pos;
			strcat(buf, "pass-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 46] = vars[pos];
				++pos;
			}
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			strcat(buf, "yes");
			close(fd);
			break;
		}
		case 'F':{
			size_t z = 0;
			strcat(buf, "3:CloudX_DB-INSERT-INTO-Files-VALS-");
			while(vars[pos] != '\n'){buf[z + 35] = vars[pos]; ++pos; ++z;} //Write HTTP vars into SQL command
			strcat(buf , ";CloudX_DB-ALTER-Users-WHERE-name-");
			pos = 2;
			for(unsigned char x = 0; x != 2; ++x){while(vars[pos] != '-'){++pos;} ++pos;} //Skip 2 hyphens to get to username.
			while(vars[pos] != '-'){buf[z + 69] = vars[pos]; ++pos; ++z;}
			strcat(buf, "-uploads-");	
			buf[z + 78] = (char)(new_uploads + 48);
			strcat(buf, ";CloudX_DB-ALTER-Users-WHERE-name-");
			pos = 2;
			for(unsigned char x = 0; x != 2; ++x){while(vars[pos] != '-'){++pos;} ++pos;} //Skip 2 hyphens to get to username.
			while(vars[pos] != '-'){buf[z + 113] = vars[pos]; ++pos; ++z;}
			strcat(buf, "-bytes_remaining-");
			unsigned char digits[32];
			memset(digits, 0x0, 32);
			//Algorithm: get digits of the number and store them as chars.
			unsigned char c = 48, index = 0;
			while(1){
				while(!(new_storage % 10 == 0)){
					++c;
					--new_storage;
				}
				digits[30 - index] = c;
				if(new_storage == 0){break;}
				new_storage /= 10;
				c = 48;
				++index;
			}
			char* ptr = digits;
			while(*ptr == '\0'){++ptr;}
			strcat(buf, ptr);
			strcat(buf, ";");
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			strcat(buf, "yes");
			close(fd);
			break;
		}
		case 'G':{
			size_t ind = 0;
			int uploads, counter = 0;
			unsigned long long s1 = 0, s2 = 0;
			strcat(buf, "2:CloudX_DB-CHECK-Files-name-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 29] = vars[pos]; 
				++pos;
			}
			++pos;

			strcat(buf, ";CloudX_DB-CHECK-Users-name-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 56] = vars[pos];
				++pos;
			}
			++pos;
			strcat(buf, "-uploads-bytes_remaining;");
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			read(fd, buf, 1024);
			printf("The database system sent the following answer: %s\n", buf);
			if(buf[0] == 'y'){
				memset(buf, 0x0, 1024);
				strcat(buf, "yes1");
				close(fd);
				break;
			}
			else{
				uploads = ((int)buf[7]) - 48;
				if(uploads == 6){
					memset(buf, 0x0, 1024);
					strcat(buf, "yes2");
					close(fd);
					break;
				}
				else{new_uploads = uploads + 1;}
				ind = strlen(buf) - 2;
				printf("ind = %lu\n", ind);
				for(size_t i = ind; buf[i] != '-'; --i){
					s1 += pow(10, counter) * (((int)(buf[i])) - 48); ++counter;
				}
				while(vars[pos] != '\n'){++pos;}
				--pos;
				counter = 0;
				for(size_t i = pos; vars[i] != '-'; --i){
					s2 += pow(10, counter) * (((int)(vars[i])) - 48); ++counter;
				}
				printf("Computed sizes: remaining(%llu) file(%llu)\n", s1, s2);
				memset(buf, 0x0, 1024);
				if (s2 > s1){strcat(buf, "yes3");}
				else{new_storage = s1 - s2; strcat(buf, "no");}
				close(fd);
				break;
			}
		}
		case 'H':{
			strcat(buf, "1:CloudX_DB-ALTER-Files-WHERE-uploader-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 39] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 37] = '-';
			++pos;
			strcat(buf, "uploader-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 48] = vars[pos];
				++pos;
			}
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			strcat(buf, "yes");
			close(fd);
			break;
		}
		case 'I':{
			strcat(buf, "1:CloudX_DB-ALTER-Users-WHERE-name-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 35] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 35] = '-';
			++pos;
			strcat(buf, "last_active-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 47] = vars[pos];
				++pos;
			}
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 1024);
			strcat(buf, "yes");
			close(fd);
			break;
		}
		default:{
			close(fd);
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

struct site_file{
	char* fname;
	char* ftype;
	char* fmode;
	char* fbuf;	
	u_int32_t fsize;
};

int main(){
	char *icotype="image/x-icon", *svgtype="image/svg+xml", *htmltype = "text/html", *pngtype = "image/png", *jpgtype = "image/jpg";

	//Add website files
	struct site_file f1 = {.fname = "index.html", .ftype = htmltype, .fmode = "r"}, f2 = {.fname = "resources/cloud.svg", .ftype = svgtype, .fmode = "rb"},
			 f3 = {.fname = "resources/gear.svg", .ftype = svgtype, .fmode = "rb"}, f4 = {.fname = "resources/favicon.ico", .ftype = icotype, .fmode = "rb"},
			 f5 = {.fname = "resources/user.svg", .ftype = svgtype, .fmode = "rb"}, f6 = {.fname = "resources/tick.svg", .ftype = svgtype, .fmode = "rb"},
			 f7 = {.fname = "resources/gearicon.svg", .ftype = svgtype, .fmode = "rb"}, f8 = {.fname = "resources/file.svg", .ftype = svgtype, .fmode = "rb"},
			 f9 = {.fname = "resources/dbadd.svg", .ftype = svgtype, .fmode = "rb"}, f10= {.fname = "resources/upload.svg", .ftype = svgtype, .fmode = "rb"},
			 f11= {.fname = "resources/download.svg", .ftype = svgtype, .fmode = "rb"}, f12 = {.fname = "resources/dbremove.svg", .ftype = svgtype, .fmode = "rb"},
			 f13= {.fname = "resources/nextarrow.svg", .ftype = svgtype, .fmode = "rb"};
	HEAP_ALIGN(struct site_file*, site_files, 24);
	//struct site_file** site_files;
	//if(memalign((void*)&site_files, 64, 24*sizeof(struct site_file*))){printf("mem err\n"); return 0;}
	site_files[0] = &f1; site_files[1] = &f2; site_files[2] = &f3; site_files[3] = &f4; site_files[4] = &f5; site_files[5] = &f6;
	site_files[6] = &f7; site_files[7] = &f8; site_files[8] = &f9; site_files[9] = &f10; site_files[10] = &f11; site_files[11] = &f12;
	site_files[12] = &f13;
	for(size_t i = 0; i < SITE_FILES; ++i){
		site_files[i]->fsize = Create_HTTPsend_Filebuf(site_files[i]->fname, site_files[i]->ftype, site_files[i]->fmode, &site_files[i]->fbuf);
	}

	//Create the server socket, bind a name to it and make it listen on port 80
	int port = 80, server_socket = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_address = {.sin_family = AF_INET, .sin_port = htons(port), .sin_addr.s_addr = INADDR_ANY};
	bind(server_socket, (struct sockaddr*)&server_address, sizeof(server_address));
	listen(server_socket, 16);

	//Declarations for serving loop
	unsigned servervars_start_i, bytes_from_small;
	unsigned long long pos = 0;
	struct sockaddr_in client_address;
    	int client_socket, sent;
	socklen_t clientLen = sizeof(struct sockaddr_in);
    	u_int32_t imageResponseLen;
   	size_t k, db_response_siz;
	unsigned long long bytes_read, total_read;
	char *serverside_variables, *requested_fname, *client_message, *db_response, *delim, *file_contents, *file_response;
	if(memalign((void*)&serverside_variables, 64, 256) || memalign((void*)&requested_fname, 64, 64) || memalign((void*)&file_contents, 64, MAX_FSIZ)
	   || memalign((void*)&client_message, 64, 2048) || memalign((void*)&db_response, 64, 1024) || memalign((void*)&delim, 64, 128))
	{printf("mem alloc fail\n"); return 0;}
	char command[256], sent_fname[64], boundary_name[128];
	FILE* created_file;
	//Serving loop
	while(1){
		//Reset stuff.
		sent = 0; k = 5;
		memset(serverside_variables, 0x0, 256); memset(requested_fname, 0x0, 64); 
		memset(client_message, 0x0, 2048); memset(db_response, 0x0, 1024);
		memset(delim, 0x0, 128); memset(boundary_name, 0x0, 128);
		printf("Listening for HTTP requests on port %d...\n", port);
		client_socket = accept(server_socket, (struct sockaddr*)&client_address, &clientLen);
		if((bytes_read = recv(client_socket, client_message, 2048, MSG_PEEK)) == -1){printf("Fault while receiving first 2048 bytes of HTTP request.\n"); goto label2;}
		printf("Received an HTTP request. Read first 2048 bytes: %lu\n\n%s", strlen(client_message), client_message);
		//Extract requested file's name if there's one.
		if(client_message[0] == 'G'){
			while(!(client_message[k+1] == 72 && client_message[k+2] == 84 && client_message[k+3] == 84 && client_message[k+4] == 80)){
				requested_fname[k-5] = client_message[k];
				++k;
				if(k > 64){break;}
			}
			if(*requested_fname){
				printf("\nThe client requested a file called: %s\n", requested_fname);
				for(size_t i = 1; i < SITE_FILES; ++i){
					if(!strcmp(site_files[i]->fname, requested_fname)){
						if(send(client_socket, site_files[i]->fbuf, site_files[i]->fsize, 0) < 0){printf("file send fail\n");}
						else{printf("Sent file %s to the client successfully!\n", site_files[i]->fname); sent = 1;}					
					}
				} 
			}
		}
		else{
			servervars_start_i = Extract_HTTP_Variables(client_message, serverside_variables);
			if(servervars_start_i < 2047){
				Generate_SQL_Command(serverside_variables, db_response);
				db_response_siz = strlen(db_response);
				printf("About to send back the following string: %s\n", db_response);
				if(send(client_socket, db_response, db_response_siz, 0) < 0){printf("db response send fail\n");}
				else{printf("Sent db response successfully.\n"); sent = 1;}
			}
			else{
				//sending a file here.
				//sleep();
				bytes_read = 0; total_read = 0;
				sleep(1);
				if((bytes_read = recv(client_socket, file_contents, MAX_FSIZ, 0)) > 0){printf("***************\t FIRST READ: %llu\t **************\n", bytes_read);}
				pos = 0;
				memset(boundary_name, 0x0, 128);
				if((pos = find_substr(file_contents, "boundary=", pos, 1024, MAX_FSIZ)) == 1023){printf("Server error: boundary= string not found.\n"); goto label3;} 
				pos += 9; //Go to where boundary name starts
				for(short i = 0U; file_contents[pos] != '\r'; ++i){ // Get boundary name
					boundary_name[i] = file_contents[pos];
					++pos;
				}
				printf("Extracted boundary string:%s\n", boundary_name);
				if((pos = find_substr(file_contents, "filename=", pos, 2047, MAX_FSIZ)) > 1024){printf("Server error: filename= string not found.\n"); goto label3;} 
				pos += 10; //Go to where sent file's name starts
				memset(sent_fname, 0x0, 64);
				for(short i = 0U; file_contents[pos] != '"'; ++i){ //Get file name
					sent_fname[i] = file_contents[pos];
					++pos;
				}
				if(*sent_fname){
					memset(command, 0x0, 256);
					snprintf(command, 256, "touch %s; chmod +rw %s", sent_fname, sent_fname); //Create the file on the system
					system(command);
				}
				else{printf("Server error: No filename detected."); goto label3;}
				created_file = fopen(sent_fname, "wb"); //Open for writing binary data to it
				printf("Generated the following terminal command:%s\n", command);
				snprintf(delim, 256, "\r\n--%s--", boundary_name);
				printf("Ending delimiter we'll be finding is:%s\n", delim);
				pos = find_substr(file_contents, "\r\n\r\n", pos, 2047, MAX_FSIZ); pos += 4;
				printf("Found the position at which file contents begin: %llu\n", pos);
				unsigned long long end_pos = find_substr(file_contents, delim, pos, bytes_read, MAX_FSIZ);
				printf("FIRST SEARCH RESULTS: bytes_read = %llu\tend_pos = %llu\n", bytes_read, end_pos);
				if(end_pos < bytes_read){
					printf("Skipping additional recv()'s cuz we found ending delim.\n");
					bytes_read = end_pos - pos;
					goto label4;
				}
				else{
					total_read = bytes_read - pos;

					while((bytes_read = recv(client_socket, file_contents + total_read, MAX_FSIZ, 0)) > 0){
						printf("\n***************\t READ: %llu\t **************\n\n", bytes_read);
						total_read += bytes_read;
						if(find_substr(file_contents, delim, pos + (total_read - bytes_read) - 128, total_read, MAX_FSIZ) != total_read){
							bytes_read = total_read - strlen(delim);
							goto label4;	
						}
						sleep(1);
					}
				}
				label4:
				created_file = fopen(sent_fname, "wb");
				printf("Size of sent file: %llu\n", bytes_read);
				const void* mem_pos = (void*)&file_contents[pos];
				fwrite(mem_pos, 1, bytes_read, created_file);
				fclose(created_file);	
				printf("Server: file stored successfully!\n");
				memset(file_contents, 0x0, MAX_FSIZ);
				goto label2;
				label3:
				printf("Server: Could not store file.\n");
				if(send(client_socket, "no", 2, 0) < 0){printf("file_response send error.\n");}
				memset(file_contents, 0x0, MAX_FSIZ);
				sent = 1; goto label2;
			}
		}
		label2:
        	if(!sent){
			printf("About to send webpage code.\n\n");
			memset(site_files[0]->fbuf, 0x0, site_files[0]->fsize);
			site_files[0]->fsize = Create_HTTPsend_Filebuf(site_files[0]->fname, site_files[0]->ftype, site_files[0]->fmode, &(site_files[0]->fbuf)); 
			if(send(client_socket, site_files[0]->fbuf, site_files[0]->fsize, 0) < 0){printf("firstpage code send fail\n");}
		}
		//Make sure data 
		if((recv(client_socket, client_message, 2048, 0)) == -1){printf("Error on last read.\n");}
		close(client_socket);
	}	
	free(serverside_variables); free(requested_fname); free(client_message); free(db_response); free(delim); free(site_files);


	return 0;
}
