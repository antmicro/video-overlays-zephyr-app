int bind_socket(int sock, struct sockaddr *sock_addr, socklen_t sock_addrlen);
int send_data(int sock, struct sockaddr *sock_addr);
int send_length(int sock, struct sockaddr *sock_addr);
int init_socket();
void send_image(uint32_t img_buff[], uint32_t img_length);
