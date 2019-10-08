#ifndef TERM_H_
#define TERM_H_

#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

extern unsigned char pal16[48];
extern unsigned char pal16_linux[48];

//atributos de los caracteres para setattr (sólo terminales gráficas)
#define ATTR_NONE      0x00 //normal
#define ATTR_BOLD      0x01 //negrita (puede aclarar el color)
#define ATTR_ITALIC    0x02 //cursiva (soportado en muy pocas terminales)
#define ATTR_UNDERLINE 0x04 //subrayado
#define ATTR_STRIKE    0x08 //tachado

//escribe una cadena en la terminal
//la almacena en un búfer y sólo escribe en el dispositivo cuando éste se llena
//o se llama a refresh()
void tputs(char *str);

//escribe en la terminal la parte del búfer que todavía no ha sido escrita
void refresh(void);

//establece los atributos de los caracteres que se van a escribir
void setattr(unsigned char attr);

//establece los 16 colores de la paleta
void setpal(unsigned char *pal);

//inicia el sistema de terminal
void startText(void);

//cierra el sistema de terminal
void endText(void);

//devuelve si hay datos disponibles en stdin
int kbhit(void);

//lee un carácter de stdin (0..255)
//devuelve -1 si hubo un error al leer
int getchr(void);

//convierte un número entero en una cadena en base decimal
void int2str(int x, char *str);

//borra la pantalla y pone el cursor en 0,0
void clear(void);

//pone el cursor en x,y
void gotoxy(int x,int y);

//establece el color de primer plano
void setfg(int clr);

//establece el color de fondo
void setbg(int clr);

//establece los colores de primer plano y fondo (dos parámetros)
void setfb(int f, int b);

//establece los colores de primer plano y fondo (un parámetro con los colores combinados)
void setcolor(int clr);

//selecciona el color de primer plano predeterminado
void resetfg(void);

//selecciona el color de fondo predeterminado
void resetbg(void);

//selecciona los colores de primer plano y fondo predeterminados
void resetcolor(void);

//oculta el cursor de texto
void hidecursor(void);

//muestra el cursor de texto
void showcursor(void);

//habilita la nueva línea automática al imprimir más allá del fin de la línea
void enablewrap(void);

//deshabilita la nueva línea automática al imprimir más allá del fin de la línea
void disablewrap(void);

//obtiene el tamaño de la terminal
void getterminalsize(int *w, int *h);

#endif // TERM_H
