#include <stdio.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>

#define HOST_ADDR 0xc0000202 // 192.0.2.2
#define ARTY_ADDR 0xc0000201 // 192.0.2.1
#define RECV_LEN 512

uint8_t buff[RECV_LEN];
uint8_t num_msgs_arr[4];
uint8_t remainder_arr[2];
int rem;

int bind_socket(int sock, struct sockaddr *sock_addr, socklen_t sock_addrlen)
{
	int ret;

	ret = bind(sock, sock_addr, sock_addrlen);

	return ret;
}

int receive_data(int sock, struct sockaddr *sock_addr, socklen_t *sock_addrlen)
{
	int ret;

	ret = recvfrom(sock, buff, RECV_LEN, 0, sock_addr, sock_addrlen);

	return ret;
}

int receive_num_msgs_arr(int sock, struct sockaddr *sock_addr, socklen_t *sock_addrlen)
{
	int ret;

	ret = recvfrom(sock, num_msgs_arr, 4, 0, sock_addr, sock_addrlen);

	return ret;
}

int receive_remainder(int sock, struct sockaddr *sock_addr, socklen_t *sock_addrlen)
{
	int ret;

	ret = recvfrom(sock, buff, rem, 0, sock_addr, sock_addrlen);

	return ret;
}

int receive_remainder_arr(int sock, struct sockaddr *sock_addr, socklen_t *sock_addrlen)
{
	int ret;

	ret = recvfrom(sock, remainder_arr, 2, 0, sock_addr, sock_addrlen);

	return ret;
}

void main()
{
	FILE *file;
	struct sockaddr_in arty_sockaddr;
	arty_sockaddr.sin_family = AF_INET;
	arty_sockaddr.sin_port = htons(4242);
	arty_sockaddr.sin_addr.s_addr = htonl(ARTY_ADDR);

	struct sockaddr_in host_sockaddr;
	host_sockaddr.sin_family = AF_INET;
	host_sockaddr.sin_port = htons(4242);
	host_sockaddr.sin_addr.s_addr = htonl(HOST_ADDR);

	int ret;

	/* Create IPv4 UDP socket*/
	int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	printf("Socket created: %d\n", sock);
	file = fopen("img.rgb565", "w");
	/* Bind socket to pc address */
	ret = bind_socket(sock, (struct sockaddr *)&host_sockaddr,
			  sizeof(host_sockaddr));

	int addrlen = sizeof(host_sockaddr);

	ret = receive_num_msgs_arr(sock, (struct sockaddr *)&host_sockaddr,
			   (socklen_t *)&addrlen);
			   
	int num_msgs = num_msgs_arr[0] << 24 | num_msgs_arr[1] << 16 | num_msgs_arr[2] << 8 | num_msgs_arr[3];
	printf("num_msgs: %d\n", num_msgs);

	while (num_msgs) {
		ret = receive_data(sock, (struct sockaddr *)&host_sockaddr,
				   (socklen_t *)&addrlen);

		if (ret < 0)
			printf("Failed to receive data. Error: %d, errno: %d\n",
				   ret, -errno);

		uint16_t rgb565_pixel = 0;
		for(int i = 0; i < RECV_LEN; i++){
			if(i % 2 == 0){
				rgb565_pixel |= buff[i] << 8;
			}
			else{
				rgb565_pixel |= buff[i];
				fwrite(&rgb565_pixel, 1, sizeof(rgb565_pixel), file);
				rgb565_pixel = 0;
			}
		}
		num_msgs--;
	}

	ret = receive_remainder_arr(sock, (struct sockaddr *)&host_sockaddr,
			   (socklen_t *)&addrlen);

	rem = remainder_arr[0] << 8 | remainder_arr[1];
	printf("remainder: %d\n", rem);

	if (rem) {
		ret = receive_remainder(sock, (struct sockaddr *)&host_sockaddr,
					(socklen_t *)&addrlen);
		if (ret < 0)
			printf("Failed to receive data. Error: %d, errno: %d\n",
					ret, -errno);
		uint16_t rgb565_pixel = 0;

		for(int i = 0; i < rem; i++){
			if(i % 2 == 0){
				rgb565_pixel |= buff[i] << 8;
			}
			else{
				rgb565_pixel |= buff[i];
				fwrite(&rgb565_pixel, 1, sizeof(rgb565_pixel), file);
				rgb565_pixel = 0;
			}
		}
	}

	fclose(file);
}
