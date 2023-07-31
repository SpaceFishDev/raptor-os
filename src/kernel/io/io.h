#ifndef IO_H
#define IO_H
void _cdecl video_mode(unsigned char mode);
void _cdecl putc(char color, char c);
void puts(char color, char* str);
void putsf(char color, char far* str);
#endif
