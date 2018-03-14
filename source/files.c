#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include "includes/files.h"
String_Allocation File_Load(const char* path) {
  struct stat stats;
  (stat(path, &stats) < 0) ?
  App_Fatal("File at %s does not exist, fatal.\n", path) : "Success, file exists.";
  (stats.st_size < 1) ?
  App_Fatal("File at %s is empty, fatal.\n", path) : "Ok.";

  String_Allocation buffer = malloc((stats.st_size + 1) * sizeof(char));
  buffer[stats.st_size] = '\0';
  FILE* file = fopen(path, "r");
  fread(buffer, stats.st_size, 1, file);

  fclose(file);
  return buffer;
}
void* File_Open(const char* path, const char* mode) {
  struct stat stats;
  (stat(path, &stats) < 0) ?
  App_Fatal("File at %s does not exist, fatal.\n", path) : "Success, file exists.";
  (stats.st_size < 1) ?
  App_Fatal("File at %s is empty, fatal.\n", path) : "Ok.";
  return fopen(path, mode);
}
