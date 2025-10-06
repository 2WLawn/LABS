#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUF_SIZE 256

struct header {
  uint64_t size;
  struct header *next;
};

void print_out(char *format, void *data, size_t data_size) {
  char buf[BUF_SIZE];
  ssize_t len = snprintf(buf, BUF_SIZE, format,
                         data_size == sizeof(uint64_t) ? *(uint64_t *)data
                                                       : *(void **)data);
  if (len < 0) {
    perror("snprintf");
    exit(1);
  }
  write(STDOUT_FILENO, buf, len);
}

int main() {
  // INCREASE HEAP SIZE
  void *start = sbrk(0);
  void *space = sbrk(256);

  // BLOCKS
  // assign pointers to each block
  struct header *firstBlock = (struct header *)start;
  struct header *secondBlock = (struct header *)((char *)start + 128);

  // initilaize headers
  firstBlock->size = 128;
  secondBlock->size = 128;

  firstBlock->next = NULL;
  secondBlock->next = firstBlock;

  // fill in data
  size_t datasize = 128 - (sizeof(struct header));

  unsigned char *first_data = (unsigned char *)(firstBlock + 1);
  unsigned char *second_data = (unsigned char *)(secondBlock + 1);

  memset(first_data, 0, datasize);
  memset(second_data, 1, datasize);
  // print

  print_out("first block:            %p\n", &firstBlock, sizeof(void *));
  print_out("second block:           %p\n", &secondBlock, sizeof(void *));
  print_out("first block size:       %lu\n", &firstBlock->size,
            sizeof(uint64_t));
  print_out("first block next:       %p\n", &firstBlock->next, sizeof(void *));
  print_out("second block size:      %lu\n", &secondBlock->size,
            sizeof(uint64_t));
  print_out("second block next:      %p\n", &secondBlock->next, sizeof(void *));

  for (size_t i = 0; i < datasize; i++) {
    uint64_t val = first_data[i];
    print_out("%d\n", &val, sizeof(uint64_t));
  }

  for (size_t i = 0; i < datasize; i++) {
    uint64_t val = second_data[i];
    print_out("%d\n", &val, sizeof(uint64_t));
  }
}
