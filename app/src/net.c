/*
 * Copyright (c) 2021-2022 Antmicro
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <net/socket.h>
#include <stdio.h>

#include "net.h"

#define HOST_ADDR 0xc0000202 // 192.0.2.2
#define ARTY_ADDR 0xc0000201 // 192.0.2.1
#define SEND_LEN 512

uint8_t buff[SEND_LEN];
uint8_t num_msgs_arr[4];
uint8_t remainder_arr[4];
int rem;

struct sockaddr_in arty_sockaddr;
struct sockaddr_in host_sockaddr;
int sock;

int bind_socket(int sock, struct sockaddr *sock_addr, socklen_t sock_addrlen)
{
	int ret;

	ret = bind(sock, sock_addr, sock_addrlen);

	return ret;
}

int send_data(int sock, struct sockaddr *sock_addr)
{
	int ret;

	ret = sendto(sock, buff, SEND_LEN, 0, sock_addr, sizeof(*sock_addr));

	return ret;
}

int send_num_msgs_arr(int sock, struct sockaddr *sock_addr)
{
	int ret;

	ret = sendto(sock, num_msgs_arr, 4, 0, sock_addr, sizeof(*sock_addr));

	return ret;
}

int send_remainder(int sock, struct sockaddr *sock_addr)
{
	int ret;

	ret = sendto(sock, buff, rem, 0, sock_addr, sizeof(*sock_addr));

	return ret;
}

int send_remainder_arr(int sock, struct sockaddr *sock_addr)
{
	int ret;

	ret = sendto(sock, remainder_arr, 4, 0, sock_addr, sizeof(*sock_addr));

	return ret;
}

int init_socket(void)
{
	int ret;

	arty_sockaddr.sin_family = AF_INET;
	arty_sockaddr.sin_port = htons(4242);
	arty_sockaddr.sin_addr.s_addr = htonl(ARTY_ADDR);

	host_sockaddr.sin_family = AF_INET;
	host_sockaddr.sin_port = htons(4242);
	host_sockaddr.sin_addr.s_addr = htonl(HOST_ADDR);
	/* Create IPv4 UDP socket*/
	sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if (sock < 0) {
		printf("Failed to create socket\n");
		return -1;
	}
	/* Bind socket to pc address */
	ret = bind_socket(sock, (struct sockaddr *)&arty_sockaddr,
			  sizeof(arty_sockaddr));
	if (ret < 0) {
		printf("Failed to bind socket: %d, errno: %d\n", ret, -errno);
		return -1;
	}
	return 0;
}

void send_image(uint32_t img_buff[], uint32_t img_length)
{
	int ret;

	int num_msgs = img_length * 4 / SEND_LEN;

	rem = (img_length * 4) % SEND_LEN;

	if (rem) {
		remainder_arr[0] = rem >> 24;
		remainder_arr[1] = rem >> 16;
		remainder_arr[2] = rem >> 8;
		remainder_arr[3] = rem;
	}

	/* write 32 bit value to 8 bit array */
	num_msgs_arr[0] = num_msgs >> 24;
	num_msgs_arr[1] = num_msgs >> 16;
	num_msgs_arr[2] = num_msgs >> 8;
	num_msgs_arr[3] = num_msgs;

	ret = send_num_msgs_arr(sock, (struct sockaddr *)&host_sockaddr);
	if (ret < 0)
		printf("Failed to send data. Error: %d, errno: %d\n", ret,
			   -errno);

	int k = 0;

	printf("num_msgs: %d\n", num_msgs);
	while (num_msgs) {
		for (int i = 0; i < SEND_LEN; i += 4) {
			buff[i + 3] = img_buff[k] >> 24;
			buff[i + 2] = img_buff[k] >> 16;
			buff[i + 1] = img_buff[k] >> 8;
			buff[i + 0] = img_buff[k];
			k++;
		}
		ret = send_data(sock, (struct sockaddr *)&host_sockaddr);
		if (ret < 0)
			printf("Failed to send data. Error: %d, errno: %d\n",
				   ret, -errno);
		num_msgs--;
	}
	printf("remainder: %d\n", rem);
	send_remainder_arr(sock, (struct sockaddr *)&host_sockaddr);
	if (ret < 0)
		printf("Failed to send data. Error: %d, errno: %d\n",
				ret, -errno);

	int i = 0;

	rem /= 4;

	while (rem) {
		buff[i + 0] = img_buff[k] >> 24;
		buff[i + 1] = img_buff[k] >> 16;
		buff[i + 2] = img_buff[k] >> 8;
		buff[i + 3] = img_buff[k];
		k++;
		i += 4;
		rem--;
	}
	send_remainder(sock, (struct sockaddr *)&host_sockaddr);
	if (ret < 0)
		printf("Failed to send data. Error: %d, errno: %d\n",
				ret, -errno);
}
