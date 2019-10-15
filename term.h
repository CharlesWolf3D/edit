#ifndef TERM_H_
#define TERM_H_

extern unsigned char pal16[48];
extern unsigned char pal16_linux[48];

//atributos de los caracteres para setattr (sólo terminales gráficas)
#define ATTR_NONE      0x00 //normal
#define ATTR_BOLD      0x01 //negrita (puede aclarar el color)
#define ATTR_ITALIC    0x02 //cursiva (soportado en muy pocas terminales)
#define ATTR_UNDERLINE 0x04 //subrayado
#define ATTR_STRIKE    0x08 //tachado

//teclas de acceso rápido
//bits 0..7:   byte de carácter recibido
//bit 8:       mayúsculas
//bit 9:       control
//bit 10:      alt
//bits 16..23: número de tecla de edición (byte de carácter es 0)
#define HK_NONE   0x00000000     //sin tecla rápida
#define HK_CHMASK 0x000000ff     //máscara para el byte de carácter
#define HK_C      0x00000100     //Ctrl
#define HK_A      0x00000200     //Alt
#define HK_S      0x00000400     //Mayús
#define HK_CA     (HK_C | HK_A)  //Ctrl+Alt
#define HK_CS     (HK_C | HK_S)  //Ctrl+Mayús
#define HK_AS     (HK_A | HK_S)  //Alt+Mayús
#define HK_CAS    (HK_CA | HK_S) //Ctrl+Alt+Mayús
#define HK_F1     0x00010000     //F1
#define HK_F2     0x00020000     //F2
#define HK_F3     0x00030000     //F3
#define HK_F4     0x00040000     //F4
#define HK_F5     0x00050000     //F5
#define HK_F6     0x00060000     //F6
#define HK_F7     0x00070000     //F7
#define HK_F8     0x00080000     //F8
#define HK_F9     0x00090000     //F9
#define HK_F10    0x000a0000     //F10
#define HK_F11    0x000b0000     //F11
#define HK_F12    0x000c0000     //F12
#define HK_INS    0x000d0000     //Insertar
#define HK_DEL    0x000e0000     //Suprimir
#define HK_HOME   0x000f0000     //Inicio
#define HK_END    0x00100000     //Fin
#define HK_PGUP   0x00110000     //Retroceder página
#define HK_PGDN   0x00120000     //Avanzar página
#define HK_UP     0x00130000     //Arriba
#define HK_DOWN   0x00140000     //Abajo
#define HK_LEFT   0x00150000     //Izquierda
#define HK_RIGHT  0x00160000     //Derecha
#define HK_ENTER  0x00170000     //Intro
#define HK_BKSP   0x00180000     //Retroceso
#define HK_TAB    0x00190000     //Tabulador
#define HK_ESC    0x001a0000     //Escape
#define HK_KMASK  0x00ff0000     //máscara para la tecla de edición

//escribe una cadena en la terminal
//la almacena en un búfer y sólo escribe en el dispositivo cuando éste se llena
//o se llama a refresh()
void tputs(char *str);

//escribe en la terminal la parte del búfer que todavía no ha sido escrita
void refresh(void);

//establece los atributos de los caracteres que se van a escribir
void setattr(unsigned char attr);

//convierte un número entero en una cadena en base hexadecimal, utilizando
//siempre 2 dígitos
void int2hex2(int x, char *str);

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
void gotoxy(int x, int y);

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

//devuelve una tecla pulsada con sus modificadores
unsigned int getKey(void);

#endif // TERM_H
