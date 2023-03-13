#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char *argv[]) {
	printf("hello world (pid:%d)\n", (int) getpid());
	
	int rc = fork();
	if (rc < 0) {
		// fork failed
		fprintf(stderr, "fork failed\n");
		exit(1);
	} else if (rc == 0) {
		// child (new process)
		char c_box[10240];
		char context[]="child write in file";
		int size = strlen(context);

		int fopen = open("file",O_RDONLY);
		printf("c0. child open file\n");
		printf("c1. child (pid:%d) file = %d\n", (int) getpid(),fopen);
		size_t nread = read(fopen,c_box,10240);
		printf("c2. child read file context:\n");
		close(fopen);
		printf("c3. child close file:\n");

		printf("c4. child read file context:\n");
		printf("c5. %s\n",c_box);
				
		int fwrite = open("file",O_WRONLY|O_TRUNC);
		printf("c6. child open file for write\n");
		size_t nwrite = write(fwrite,context,size);
		printf("c7. child writed in file\n");
		close(fwrite);
		printf("c8. child cloase file for write\n");

		fopen = open("file",O_RDONLY);
		printf("c9. child open file for read\n");
		nread = read(fopen,c_box,10240);
		printf("c10. child readed file context \n");
		close(fopen);
		printf("c11. child close file for read \n");

		printf("c12. child read file context(after write):\n");
		printf("c13. %s\n",c_box);

	} else {
		// parent goes down this path (main)
		//wait(NULL);		
		char box[10240];
		char context[]="parent write in file";
		int size = strlen(context);

		int fopen = open("file",O_RDONLY);		
		printf("p0. parent open file\n");
		printf("p1. parent of %d (pid:%d) file = %d\n",rc, (int) getpid(),fopen);

		size_t nread = read(fopen,box,10240);
		printf("p2. parent readed file context:\n");
		close(fopen);
		printf("p3. parent close file:\n");
		
		printf("p4. parent read file context:\n");
		printf("p5. %s\n",box);

		int fwrite = open("file",O_WRONLY|O_TRUNC);
		printf("p6. parent open for write\n");
		size_t nwrite = write(fwrite,context,size);
		printf("p7. parent writed in file\n");
		close(fwrite);
		printf("p8. parent close file for write\n");		

		fopen = open("file",O_RDONLY);
		printf("p9. parent open for read\n");
		nread = read(fopen,box,10240);
		printf("p10. parent read file context\n");
		close(fopen);
		printf("p11. parent close open for read\n");

		printf("p12. parent read file context(after write):\n");
		printf("p13. %s\n",box);
	}
	return 0;
}