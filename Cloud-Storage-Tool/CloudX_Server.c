void Generate_SQL_Command(char* vars, char* buf){
	unsigned pos = 2;
	char *socket_path = "\0hidden";
	int fd = socket(AF_UNIX, SOCK_STREAM, 0);
	struct sockaddr_un addr;
	memset(&addr, 0x0, sizeof(addr));
	addr.sun_family = AF_UNIX;
	*addr.sun_path = '\0';
	strncpy(addr.sun_path+1, socket_path+1, sizeof(addr.sun_path)-2);
	connect(fd, (struct sockaddr*)&addr, sizeof(addr));
	switch(vars[0]){
		case 'A':
		case 'B':{
			strcat(buf, "CloudX_DB-CHECK-Users-name-");
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 27] = vars[pos]; 
				++pos;
			}
			strcat(buf, "-pass-unique_num\0");
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 128);
			read(fd, buf, 128);
			printf("The database system sent the following answer: %s\n", buf);
			close(fd);
			break;
		}
		case 'C':{
			strcat(buf, "CloudX_DB-INSERT-INTO-Users-VALS-");
			while(vars[pos] != '-'){
				buf[(pos - 2) + 33] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 33] = '-';
			++pos;
			while(vars[pos] != '\n'){
				buf[(pos - 2) + 33] = vars[pos];
				++pos;
			}
			buf[(pos - 2) + 33] = '-';
			++pos;
			time_t t;
		   	srand((unsigned) time(&t));
			char* db_uniqueid;
			memalign((void*)&db_uniqueid, 64, 6);
			for(size_t i = 0; i < 6; ++i){db_uniqueid[i] = ((rand() % 56) + 40);}
			strcat(buf, db_uniqueid);
			strcat(buf, "-0-0.0.0");			
			printf("Constructed the following custom SQL command: %s\n", buf);
			write(fd, buf, strlen(buf));
			memset(buf, 0x0, 128);
			for(size_t i = 0; i < 6; ++i){buf[i] = db_uniqueid[i];}
			close(fd);
			break;
		}
		default:{
			break;
		}	  
	}
	return;
}
