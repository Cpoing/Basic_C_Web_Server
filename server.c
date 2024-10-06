#include <sys/socket.h>
#include <string.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/uio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main() {
  int s = socket(AF_INET, SOCK_STREAM, 0);
  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(0x901f), // Use htons to convert port to network byte order
    .sin_addr.s_addr = INADDR_ANY // Listen on all interfaces
  };
  bind(s, (struct sockaddr *)&addr, sizeof(addr));

  listen(s, 10);

  int client_fd = accept(s, 0, 0);
  
  char buffer[256] = {0};
  recv(client_fd, buffer, 256, 0);

  char* f = buffer + 5;
  
  *strchr(f, ' ') = 0;
  int opened_fd = open(f, O_RDONLY);
  sendfile(opened_fd, client_fd, 0, NULL, 0, 0); // Use correct number of arguments

  close(opened_fd);
  close(client_fd);
  close(s);

  return 0; // Return an int from main
}

