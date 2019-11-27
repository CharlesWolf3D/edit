#ifndef TERM_HPP_
#define TERM_HPP_

#include <termios.h>

#include "types.hpp"

//atributos de los caracteres para setattr (sólo terminales gráficas)
#define ATTR_NONE      0x00 //normal
#define ATTR_BOLD      0x01 //negrita (puede aclarar el color)
#define ATTR_ITALIC    0x02 //cursiva (soportado en muy pocas terminales)
#define ATTR_UNDERLINE 0x04 //subrayado
#define ATTR_STRIKE    0x08 //tachado

//eventos de teclado y ratón
//bits 0..7:   contenido
//  carácter
//    byte de carácter recibido
//  tecla
//    número de tecla
//  ratón
//    b0 izquierdo
//    b1 derecho
//    b2 central
//    b6 rueda arriba
//    b7 rueda abajo
//bits 8..15:  (rat) coordenada x
//bits 16..23: (rat) coordenada y
//bit 24:      mayúsculas
//bit 25:      control
//bit 26:      alt
//bits 30..31: 0=carácter, 1=tecla de edición, 2=ratón

#define HK_NONE     0x00000000                 //sin evento
#define HK_TYPEMASK 0xc0000000                 //máscara para el tipo
#define HK_TC       0x00000000                 //tipo carácter
#define HK_TK       0x40000000                 //tipo tecla
#define HK_TM       0x80000000                 //tipo ratón
#define HK_C        0x01000000                 //Ctrl
#define HK_A        0x02000000                 //Alt
#define HK_S        0x04000000                 //Mayús
#define HK_CA       (HK_C  | HK_A)             //Ctrl+Alt
#define HK_CS       (HK_C  | HK_S)             //Ctrl+Mayús
#define HK_AS       (HK_A  | HK_S)             //Alt+Mayús
#define HK_CAS      (HK_CA | HK_S)             //Ctrl+Alt+Mayús
#define HK_F1       0x00000001                 //F1
#define HK_F2       0x00000002                 //F2
#define HK_F3       0x00000003                 //F3
#define HK_F4       0x00000004                 //F4
#define HK_F5       0x00000005                 //F5
#define HK_F6       0x00000006                 //F6
#define HK_F7       0x00000007                 //F7
#define HK_F8       0x00000008                 //F8
#define HK_F9       0x00000009                 //F9
#define HK_F10      0x0000000a                 //F10
#define HK_F11      0x0000000b                 //F11
#define HK_F12      0x0000000c                 //F12
#define HK_INS      0x0000000d                 //Insertar
#define HK_DEL      0x0000000e                 //Suprimir
#define HK_HOME     0x0000000f                 //Inicio
#define HK_END      0x00000010                 //Fin
#define HK_PGUP     0x00000011                 //Retroceder página
#define HK_PGDN     0x00000012                 //Avanzar página
#define HK_UP       0x00000013                 //Arriba
#define HK_DOWN     0x00000014                 //Abajo
#define HK_LEFT     0x00000015                 //Izquierda
#define HK_RIGHT    0x00000016                 //Derecha
#define HK_ENTER    0x00000017                 //Intro
#define HK_BKSP     0x00000018                 //Retroceso
#define HK_TAB      0x00000019                 //Tabulador
#define HK_ESC      0x0000001a                 //Escape
#define HK_M1       0x00000001                 //ratón, botón izquierdo
#define HK_M2       0x00000002                 //ratón, botón derecho
#define HK_M3       0x00000004                 //ratón, botón central
#define HK_WHUP     0x00000040                 //Rueda arriba
#define HK_WHDN     0x00000080                 //Rueda abajo
#define HK_GETC(x)  ((x)&0xff)                 //obtener contenido sin modificadores
#define HK_GET(x)   ((x)&(0xff|HK_CAS))        //obtener contenido con modificadores
#define HK_MX(x)    (((x)>>8)&0xff)            //obtener coordenada x del ratón
#define HK_MY(x)    (((x)>>16)&0xff)           //obtener coordenada y del ratón
#define HK_TYPEC(x) (((x)&HK_TYPEMASK)==HK_TC) //si el evento es de tipo carácter
#define HK_TYPEK(x) (((x)&HK_TYPEMASK)==HK_TK) //si el evento es de tipo tecla
#define HK_TYPEM(x) (((x)&HK_TYPEMASK)==HK_TM) //si el evento es de tipo ratón

#define TERMBUFSZ 8192
class TAnsiTerminal
{
public:
	// Inicia el sistema de terminal.
	void Start(void);
	
	// Cierra el sistema de terminal.
	void End(void);
	
	// Escribe una cadena (texto o comandos) en la terminal.
	// La almacena en un búfer y sólo escribe en el dispositivo cuando éste se llena o se llama a Refresh().
	void Print(const char *str);
	
	// Escribe en la terminal la parte del búfer que todavía no ha sido escrita.
	void Refresh(void);
	
	// Borra la pantalla y pone el cursor en 0,0.
	void Clear(void);
	
	// Pone el cursor en x,y.
	void GotoXY(int32 x, int32 y);
	
	// Obtiene el tamaño de la terminal
	void GetSize(int32 *w, int32 *h);
	
	// Muestra u oculta el cursor de texto.
	void ShowCursor(byte show);
	
	// Habilita o deshabilita la nueva línea automática al imprimir más allá del fin de la línea.
	void EnableWrap(byte enable);
	
	// Establece los atributos de los caracteres que se van a escribir.
	void SetAttr(byte attr);
	
	// Establece el color de primer plano.
	// El valor -1 establece el color predeterminado de la terminal.
	void SetFgColor(int8 clr);

	// Establece el color de fondo.
	// El valor -1 establece el color predeterminado de la terminal.
	void SetBgColor(int8 clr);
	
	// Establece los 16 colores de la paleta.
	void SetPal(byte *pal);
	
	// Devuelve si hay datos disponibles en stdin.
	byte KbHit(void);
	
	// Lee un carácter de stdin (0..255).
	// Devuelve -1 si hubo un error al leer.
	int GetChr(void);
	
	// Devuelve una tecla pulsada con sus modificadores.
	dword GetKey(void);
	
private:
	char termBuffer[TERMBUFSZ]; //búfer temporal para enviar datos a stdout en bloques
	int termBufferCount;
	byte textAttr; //últimos atributos de caracteres seleccionados
	int32 textFG; //último color de primer plano seleccionado
	int32 textBG; //último color de fondo seleccionado
	struct termios savedTtyState; //estado de stdin salvado
};

#endif // TERM_HPP_
