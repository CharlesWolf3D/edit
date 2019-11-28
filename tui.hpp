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
	CLRF_TITLE,
	CLRB_TITLE,
	CLRF_MENUBAR,
	CLRB_MENUBAR,
	CLRF_MENUBAR_SEL,
	CLRB_MENUBAR_SEL,
	CLRF_MENUBAR_KEY,
	CLRB_MENUBAR_KEY,
	CLRF_MENUBAR_KEY_SEL,
	CLRB_MENUBAR_KEY_SEL,
	CLRF_MENUBAR_DIS,
	CLRB_MENUBAR_DIS,
	CLRF_MENUBAR_DIS_SEL,
	CLRB_MENUBAR_DIS_SEL,
	CLRF_MENU,
	CLRB_MENU,
	CLRF_MENU_SEL,
	CLRB_MENU_SEL,
	CLRF_MENU_KEY,
	CLRB_MENU_KEY,
	CLRF_MENU_KEY_SEL,
	CLRB_MENU_KEY_SEL,
	CLRF_MENU_DIS,
	CLRB_MENU_DIS,
	CLRF_MENU_DIS_SEL,
	CLRB_MENU_DIS_SEL,
	CLRF_SEARCHBAR,
	CLRB_SEARCHBAR,
	CLRF_SEARCHBAR_TXT,
	CLRB_SEARCHBAR_TXT,
	CLRF_SEARCHBAR_DEL,
	CLRB_SEARCHBAR_DEL,
	CLRF_SEARCHBAR_BTN,
	CLRB_SEARCHBAR_BTN,
	CLRF_TABBAR,
	CLRB_TABBAR,
	CLRF_TABBAR_ACTIVE,
	CLRB_TABBAR_ACTIVE,
	CLRF_TABBAR_INACTIVE,
	CLRB_TABBAR_INACTIVE,
	CLRF_TABBAR_CLOSE,
	CLRB_TABBAR_CLOSE,
	CLRF_TABBAR_ARROW,
	CLRB_TABBAR_ARROW,
	CLRF_TABBAR_ARROW_DIS,
	CLRB_TABBAR_ARROW_DIS,
	CLRF_EDIT_TEXT,
	CLRB_EDIT_TEXT,
	CLRF_EDIT_TEXT_SEL,
	CLRB_EDIT_TEXT_SEL,
	CLRF_EDIT_BORDER,
	CLRB_EDIT_BORDER,
	CLRF_EDIT_SC_ARR,
	CLRB_EDIT_SC_ARR,
	CLRF_EDIT_SC_SPACE,
	CLRB_EDIT_SC_SPACE,
	CLRF_EDIT_SC_THUMB,
	CLRB_EDIT_SC_THUMB,
	CLRF_STATUSBAR,
	CLRB_STATUSBAR
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


//mostrar título
#define SHOWTITLE_NONE       0 //no mostrar
#define SHOWTITLE_NAME       1 //nombre
#define SHOWTITLE_FULL       2 //ruta y nombre
extern byte wndShowTitle;
//mostrar barra de búsqueda
#define SHOWSEARCHBAR_NONE   0 //no mostrar
#define SHOWSEARCHBAR_ACTIVE 1 //mostrar
extern byte wndShowSearchBar;
//mostrar barra de pestañas
#define SHOWTABS_NONE        0 //no mostrar
#define SHOWTABS_ACTIVE      1 //mostrar
extern byte wndShowTabs;
//mostrar barra de estado
#define SHOWSTATUS_NONE      0 //no mostrar
#define SHOWSTATUS_ACTIVE    1 //mostrar
extern byte wndShowStatus;
//ubicación de la posición
#define POSLOCATION_NONE     0 //no mostrar
#define POSLOCATION_EDIT     1 //barra de desplazamiento del editor
#define POSLOCATION_LSTATUS  2 //barra de estado, izquierda
#define POSLOCATION_RSTATUS  3 //barra de estado, derecha
#define POSLOCATION_LTITLE   4 //barra de título, izquierda
#define POSLOCATION_RTITLE   5 //barra de título, derecha
#define POSLOCATION_MENU     6 //barra de menú
extern byte wndPosLocation;

// Celda de carácter de la terminal.
// Sólo admite caracteres compuestos por un punto de código.
typedef struct
{
	dword chr; // Punto de código en UTF-32
	byte fg;   // Color de primer plano, valor 255 es predeterminado
	byte bg;   // Color de fondo, valor 255 es predeterminado
} cell_t;

class TTui
{
public:
	int wndW, wndH; // Dimensiones de la terminal
	int maxW, maxH; // Dimensiones máximas permitidas de la terminal
	TAnsiTerminal term;
	
	// Inicializa el entorno de texto.
	// Devuelve 0 si se pudo, 1 si no hay memoria suficiente.
	byte Start(void);
	
	// Cierra el entorno de texto.
	void End(void);
	
	// Redibuja el entorno de texto si es necesario. También comprueba si hay que redimensionar.
	// redraw =
	//     0 = redibujar todo si cambió de tamaño, no redibujar si no cambió el tamaño
	//     1 = redibujar todo si cambió de tamaño, redibujar parcialmente lo que haya cambiado en el búfer
	//     2 = redibujar todo
	void Update(byte redraw);
	
	void TEST_REDRAW(void);
private:
	cell_t *screenBuffer1, *screenBuffer2; // Búferes de pantalla
};

#endif // TUI_HPP_
