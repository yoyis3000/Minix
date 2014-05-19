#include "sema.h"
#include <unistd.h>
#include <stdio.h>

int main(int argc, char** args) {
  int pointless_value;
  pointless_value = 5;
  sem_init(pointless_value);
}


