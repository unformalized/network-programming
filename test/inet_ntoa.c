#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

int main(int argc, char const *argv[])
{
  struct sockaddr_in add1, add2;
  char *str_ptr;
  char str_arr[20];

  add1.sin_addr.s_addr = htonl(0x1020304);
  add2.sin_addr.s_addr = htonl(0x1010101);

  str_ptr = inet_ntoa(add1.sin_addr);
  strcpy(str_arr, str_ptr);
  printf("Dotted-Decimal notation1: %s \n", str_ptr);

  str_ptr = inet_ntoa(add2.sin_addr);
  printf("Dotted-Decimal notation2: %s \n", str_ptr);
  printf("Dotted-Decimal notation3: %s \n", str_arr);
  return 0;
}
