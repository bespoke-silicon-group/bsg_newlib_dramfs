/*
 * fhello.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  int c;

  /* Open hello.txt for reading */
  FILE *hello = fopen("hello.txt", "r");
  if(hello == NULL)
    return -1;


  /* Iterate through the entire file
   * and print the contents to stdout
   */

  putchar('\n');

  while((c = fgetc(hello)) != EOF) {
    putchar(c);
  }

  fclose(hello);
  return 0;
}
