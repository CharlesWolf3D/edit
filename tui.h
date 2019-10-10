#ifndef TUI_H_
#define TUI_H_

#include "term.h"
#include "window.h"
#include "controls.h"
#include "menu.h"
#include "edit.h"

//colores de los elementos
extern unsigned char colors[];

//constantes para los colores de los elementos
enum
{
	CLR_TITLE,
	CLR_MENUBAR,
	CLR_MENUBAR_SEL,
	CLR_MENUBAR_KEY,
	CLR_MENUBAR_KEY_SEL,
	CLR_MENUBAR_DIS,
	CLR_MENUBAR_DIS_SEL,
	CLR_MENU,
	CLR_MENU_SEL,
	CLR_MENU_KEY,
	CLR_MENU_KEY_SEL,
	CLR_MENU_DIS,
	CLR_MENU_DIS_SEL,
	CLR_SEARCHBAR,
	CLR_SEARCHBAR_TXT,
	CLR_SEARCHBAR_DEL,
	CLR_SEARCHBAR_BTN,
	CLR_TABBAR,
	CLR_TABBAR_ACTIVE,
	CLR_TABBAR_INACTIVE,
	CLR_TABBAR_CLOSE,
	CLR_TABBAR_ARROW,
	CLR_TABBAR_ARROW_DIS,
	CLR_EDIT_TEXT,
	CLR_EDIT_TEXT_SEL,
	CLR_EDIT_BORDER,
	CLR_EDIT_SC_ARR,
	CLR_EDIT_SC_SPACE,
	CLR_EDIT_SC_THUMB,
	CLR_STATUSBAR
};

//caracteres gráficos
extern char *gchars[];

//constantes para los caracrteres gráficos
enum
{
	GCH_BOX1_C1,
	GCH_BOX1_C2,
	GCH_BOX1_C3,
	GCH_BOX1_C4,
	GCH_BOX1_TOP,
	GCH_BOX1_BOTTOM,
	GCH_BOX1_LEFT,
	GCH_BOX1_RIGHT,
	GCH_BOX1_XBOTTOM,
	GCH_BOX1_XTOP,
	GCH_BOX1_XRIGHT,
	GCH_BOX1_XLEFT,
	GCH_BOX2_C1,
	GCH_BOX2_C2,
	GCH_BOX2_C3,
	GCH_BOX2_C4,
	GCH_BOX2_TOP,
	GCH_BOX2_BOTTOM,
	GCH_BOX2_LEFT,
	GCH_BOX2_RIGHT,
	GCH_BOX2_XBOTTOM,
	GCH_BOX2_XTOP,
	GCH_BOX2_XRIGHT,
	GCH_BOX2_XLEFT,
	GCH_SCRL_UP,
	GCH_SCRL_DOWN,
	GCH_SCRL_LEFT,
	GCH_SCRL_RIGHT,
	GCH_SCRL_SPACE,
	GCH_SCRL_THUMB,
	GCH_MENU_CHK,
	GCH_MENU_OPT,
	GCH_PROG_EMPTY,
	GCH_PROG_FULL
};

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

extern int wndW, wndH; //dimensiones de la ventana

//mostrar título
#define SHOWTITLE_NONE       0 //no mostrar
#define SHOWTITLE_NAME       1 //nombre
#define SHOWTITLE_FULL       2 //ruta y nombre
extern char wndShowTitle;
//mostrar barra de búsqueda
#define SHOWSEARCHBAR_NONE   0 //no mostrar
#define SHOWSEARCHBAR_ACTIVE 1 //mostrar
extern char wndShowSearchBar;
//mostrar barra de pestañas
#define SHOWTABS_NONE        0 //no mostrar
#define SHOWTABS_ACTIVE      1 //mostrar
extern char wndShowTabs;
//mostrar barra de estado
#define SHOWSTATUS_NONE      0 //no mostrar
#define SHOWSTATUS_ACTIVE    1 //mostrar
extern char wndShowStatus;
//ubicación de la posición
#define POSLOCATION_NONE     0 //no mostrar
#define POSLOCATION_EDIT     1 //barra de desplazamiento del editor
#define POSLOCATION_LSTATUS  2 //barra de estado, izquierda
#define POSLOCATION_RSTATUS  3 //barra de estado, derecha
#define POSLOCATION_LTITLE   4 //barra de título, izquierda
#define POSLOCATION_RTITLE   5 //barra de título, derecha
#define POSLOCATION_MENU     6 //barra de menú
extern char wndPosLocation;

//inicializa la ventana del editor
void wndInit(void);

//termina la ventana del editor
void wndEnd(void);

//redibuja la ventana del programa
//las dimensiones vienen dadas por wndW y wndH
void wndRedraw(void);

#endif // TUI_H_
