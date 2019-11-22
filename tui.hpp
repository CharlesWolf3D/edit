#ifndef TUI_HPP_
#define TUI_HPP_

#include "term.hpp"
#include "window.hpp"
#include "controls.hpp"
#include "menu.hpp"
#include "edit.hpp"

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
extern const char *gchars[];

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
extern int test_scroll;////
#define HK_KEY_NUMMODS 3
extern const char *modkeynames[HK_KEY_NUMMODS]; //nombres de las teclas modificadoras
#define HK_KEY_NUMKEYS 26
extern const char *keynames[HK_KEY_NUMKEYS]; //nombres de las teclas de acceso rápido (((HK_* >> 16) & 0xff) - 1)

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

#endif // TUI_HPP_
