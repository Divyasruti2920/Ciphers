#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>

// Driver code
int main()
{
	int clientSocket, portNum, nBytes;
	char file_buffer[3000], path[1024], buffer[3000];

	// This key array stores the hidden key
	char const key[3000] = "HIDDENKEY";
	struct sockaddr_in serverAddr;
	socklen_t addr_size;
	int i;
	clientSocket = socket(PF_INET, SOCK_DGRAM, 0);

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(5004);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	memset(serverAddr.sin_zero, '\0', sizeof serverAddr.sin_zero);

	addr_size = sizeof serverAddr;

	while (1) {
		printf("Specify file name: \n");
		gets(path);

		// printf("%s\n", path);
		FILE* fp;
		fp = fopen(path, "r");
		if (fp == NULL) {
			printf("file does not exist\n");
		}

		fseek(fp, 0, SEEK_END);
		size_t file_size = ftell(fp);
		fseek(fp, 0, SEEK_SET);

		if (fread(file_buffer, file_size, 1, fp) <= 0) {
			printf("unable to copy file into buffer\n");
			exit(1);
		}

		if (sendto(clientSocket, file_buffer, 3000, 0, (struct sockaddr*)&serverAddr,
																	addr_size) < 0) {
			printf("error in sending the file\n");
			exit(1);
		}

		bzero(file_buffer, sizeof(file_buffer));

		nBytes = recvfrom(clientSocket, buffer, 1024, 0, NULL, NULL);

		printf("Received from server: \n");

		// printing some of the character to have a feel of encryption
		for (i = 0; i < 15; ++i)
			printf("%02X ", buffer[i]);
		printf("\n");

		char x[3000];
		for (i = 0; i < nBytes - 1; ++i)
			x[i] = (char)(buffer[i] ^ key[i]);

		// printing some of the character to have a feel of decryption
		printf("Decrypted message: (First 15 characters)\n");
		for (i = 0; i < 11; ++i)
			printf("%c ", x[i]);

		printf("\n");
	}

	return 0;
}
