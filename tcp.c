#include <iostream>
#include <fcntl.h>
#include <unistd.h>

using std::cout;
using std::cin;
using std::endl;

int main(int argc, char* argv[]){

	if(argc != 3){
		cout<<"usage:tcp file1 <file2|dir>"<<endl;
		return 1;
	}

	struct stat buf1;
	struct stat buf2;
	char *filepath_1 = argv[1];
	char *filepath_2 = argv[2];
	int filedes_1,filedes_2;
	if (stat(filepath_1,&buf1) < 0){
		cout<<"File1 not exists!"<<endl;
		return 1;
	}

	if(S_ISDIR(buf1.st_mode)){
		cout<<"Cannot copy a dir"<<endl;
		return 1;
	}

	if(S_ISREG(buf1.st_mode)){
		char temp = filepath_2[strlen(filepath_2)-1];
		// if(temp == '/'){

		// }
		if(stat(filepath_2,&buf2) < 0){
			if(temp == '/'){
				cout<<""<<endl;
			}
		}
	}

	return 0;
}