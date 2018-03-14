#ifndef FILES_HEADER_GUARD
#define FILES_HEADER_GUARD
void App_Fatal(const char* fmt, ...);
typedef char* String_Allocation;
#define String_Allocation_Free(x) free(x);
String_Allocation File_Load(const char* path);
void* File_Open(const char* path, const char* mode);
/* Don't use this unless you know how! */
typedef struct {
  unsigned char* data;
  int width, height;
} Files_Texture;
#endif
