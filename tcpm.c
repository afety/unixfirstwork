#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <iostream>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::endl;

//get file name not the path
char *getFilename(const char *filepath){
	int len = strlen(filepath);
	char *temp = new char[len];
	for(size_t i=len-1;; i--){
		if(filepath[i] != '/'){
			strcat(temp,&filepath[i]);
		}else{
			break;
		}
	}
	return temp;
}

void copy(int des1, int des2, int filesize){
	cout<<"size:" << filesize <<endl;

    char *src = (char*)mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, des1, 0);

    
    ftruncate(des2, filesize);

    char *dest = (char*)mmap(NULL, filesize, PROT_WRITE, MAP_SHARED, des2, 0);
	

    memcpy(dest, src, filesize);
    munmap(dest, filesize);
	
	close(des1);
    close(des2);

}

int main(int argc, char* argv[]){

	if(argc != 3){
		cout<<"usage:tcp file1 <file2|dir>"<<endl;
		return 1;
	}

	struct stat buf1;
	struct stat buf2;
	const char *filepath_1 = argv[1];
	const char *filepath_2 = argv[2];
	int filedes_1, filedes_2;

	if (stat(filepath_1,&buf1) < 0){
		cout<<"File1 not exists!"<<endl;
		return 1;
	}

	if(S_ISDIR(buf1.st_mode)){
		cout<<"Cannot copy a dir"<<endl;
		return 1;
	}

	filedes_1 = open(filepath_1,O_RDONLY);

	if(S_ISREG(buf1.st_mode)){
		char temp = filepath_2[strlen(filepath_2)-1];
		if(temp == '/'){
			cout<<"file2 dir not exists"<<endl;

			//get file2 path
			char *name = getFilename(filepath_1);
			char *file2_name = new char[strlen(filepath_2)+strlen(name)+2];
			strcat(file2_name,filepath_2);
			strcat(file2_name,name);
			
			if(stat(file2_name,&buf2)<0){
				if(access(filepath_2,F_OK)<0){
					cout<<"Dir not exiets"<<endl;
					return 1;
				}else{
					if((filedes_2 = open(file2_name,O_RDWR|O_CREAT|O_TRUNC,0644))<0){
						cout<<"Create File error!"<<endl;
						return 1;
					}
				}
			}else{
				cout<<"File path has existed,cover?"<<endl;
				char c;
				cin >> c;
				if(c != 'y' || c != 'Y'){
					return 1;
				}
				//get the file descriptor
				filedes_2 = open(file2_name, O_RDWR | O_TRUNC);
			}
		}else{
			if(access(filepath_2,F_OK)<0){
				if((filedes_2=open(filepath_2,O_RDWR|O_CREAT|O_TRUNC,0644))<0){
					cout<<"Create File Error!"<<endl;
					return 1;
				}
			}else{
				cout<<"File path has existed,cover?"<<endl;
				char c;
				cin >> c;
				if(c != 'y' || c != 'Y'){
					return 1;
				}
				//get the file descriptor
				filedes_2 = open(filepath_2,O_RDWR | O_TRUNC);
			}
		}
	}

	copy(filedes_1,filedes_2,buf1.st_size);

	return 0;
}
