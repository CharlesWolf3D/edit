#include <stdlib.h>
#include "ventana.h"
#include "term.h"

int wndW, wndH;

//mostrar título
char wndShowTitle = SHOWTITLE_NAME;
//mostrar barra de búsqueda
char wndShowSearchBar = SHOWSEARCHBAR_ACTIVE;
//mostrar barra de pestañas
char wndShowTabs = SHOWTABS_ACTIVE;
//mostrar barra de estado
char wndShowStatus = SHOWSTATUS_ACTIVE;
//ubicación de la posición
char wndPosLocation = POSLOCATION_LSTATUS;

char *c1 =         "\u2554"; //arriba izquierda
char *c2 =         "\u2557"; //arriba derecha
char *c3 =         "\u255a"; //abajo izquierda
char *c4 =         "\u255d"; //abajo derecha
char *top =        "\u2550"; //arriba
char *bottom =     "\u2550"; //abajo
char *left =       "\u2551"; //izquierda
char *right =      "\u2551"; //derecha
char *sh =         "\u2591"; //sombra

char *scrl_up =    "\u25b2"; //barra despl., flecha arriba
char *scrl_down =  "\u25bc"; //barra despl., flecha abajo
char *scrl_left =  "\u25c4"; //barra despl., flecha izquierda
char *scrl_right = "\u25ba"; //barra despl., flecha derecha
char *scrl_space = "\u2591"; //barra despl., espacio
char *scrl_thumb = "\u2588"; //barra despl., control

//colores de los elementos
unsigned char colors[] =
{
	0x1a, //título
	0x70, //barra de menús
	0x20, //barra de menús, seleccionado
	0x74, //barra de menús, tecla
	0x24, //barra de menús, tecla, seleccionado
	0x78, //barra de menús, deshabilitado
	0x27, //barra de menús, deshabilitado, seleccionado
	0x70, //menú
	0x2f, //menú, seleccionado
	0x74, //menú, tecla
	0x2c, //menú, tecla, seleccionado
	0x78, //menú, deshabilitado
	0x27, //menú, deshabilitado, seleccionado
	0x70, //barra de búsqueda
	0x80, //barra de pestañas
	0xf1, //pestaña
	0x17, //editor, texto
	0x71, //editor, texto seleccionado
	0x1a, //editor, borde
	0xb3, //editor, barra, flecha
	0x0f, //editor, barra, espacio
	0xb0, //editor, barra, control
	0x3f  //barra de estado
};

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
	CLR_TABBAR,
	CLR_TABBTN,
	CLR_EDIT_TEXT,
	CLR_EDIT_TEXT_SEL,
	CLR_EDIT_BORDER,
	CLR_EDIT_SC_ARR,
	CLR_EDIT_SC_SPACE,
	CLR_EDIT_SC_THUMB,
	CLR_STATUSBAR
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

//nombres de las teclas modificadoras
#define HK_KEY_NUMMODS 3
char *modkeynames[HK_KEY_NUMMODS] = {"Ctrl", "Alt", "Mayús"};
//nombres de las teclas de acceso rápido (((HK_* >> 16) & 0xff) - 1)
#define HK_KEY_NUMKEYS 26
char *keynames[HK_KEY_NUMKEYS] =
{
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"Ins", "Supr", "Inicio", "Fin", "RePág", "AvPág",
	"Arriba", "Abajo", "Izquierda", "Derecha",
	"Intro", "Retroceso", "Tab", "Escape"
};

//identificadores para elementos de menú
enum
{
	MNID_NONE,
	MNID_SAVE,
	MNID_SAVEAS,
	MNID_SAVECOPY,
	MNID_RENAME,
	MNID_CLOSE,
	MNID_UNDO,
	MNID_REDO,
	MNID_CUT,
	MNID_COPY,
	MNID_PASTE,
	MNID_DELETE,
	MNID_SPACES,
	MNID_TABS,
	MNID_NEWLS,
	MNID_ALLCHARS,
	MNID_CTXUNDO,
	MNID_CTXREDO,
	MNID_CTXCUT,
	MNID_CTXCOPY,
	MNID_CTXPASTE,
	MNID_CTXDELETE
};

//opciones para elementos de menú
#define MNFL_NORM 0x00000000 //sin opciones
#define MNFL_DIS  0x00000001 //deshabilitar
#define MNFL_INV  0x00000002 //no visible
#define MNFL_CHK  0x00000004 //con icono de verificación
#define MNFL_OPT  0x00000008 //con icono de opción
#define MNFL_NOHK 0x00000010 //no responder a la tecla de acceso rápido

//elemento de definición de menú
typedef struct
{
	char *caption;                 //el & indica el carácter enfatizado, cada tab al principio es un nivel de anidación
	unsigned int hk;               //tecla de acceso rápido
	unsigned int id;               //identificador para modificar el elemento
	unsigned int flags;            //opciones
	void (*func)(unsigned int id); //callback
} menudef_t;

//menú principal
menudef_t menudefs[] =
{  //Texto                                Tecla rápida       ID             Opciones   Función
	{"&Archivo",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Nuevo",                          HK_C   |      'N', MNID_NONE,     MNFL_NORM, NULL},
	{"	&Abrir...",                       HK_C   |      'O', MNID_NONE,     MNFL_NORM, NULL},
	{"	&Guardar",                        HK_C   |      'S', MNID_SAVE,     MNFL_NORM, NULL},
	{"	Guardar &como...",                HK_CS  |      'S', MNID_SAVEAS,   MNFL_NORM, NULL},
	{"	Guardar copia co&mo...",          HK_CA  |      'S', MNID_SAVECOPY, MNFL_NORM, NULL},
	{"	&Renombrar...",                               HK_F2, MNID_RENAME,   MNFL_NORM, NULL},
	{"	Ce&rrar",                         HK_C   |    HK_F4, MNID_CLOSE,    MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Salir",                          HK_A   |    HK_F4, MNID_NONE,     MNFL_NORM, NULL},
	{"&Edición",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Deshacer",                       HK_C   |      'Z', MNID_UNDO,     MNFL_NORM, NULL},
	{"	&Rehacer",                        HK_C   |      'Y', MNID_REDO,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	C&ortar",                         HK_C   |      'X', MNID_CUT,      MNFL_NORM, NULL},
	{"	&Copiar",                         HK_C   |      'C', MNID_COPY,     MNFL_NORM, NULL},
	{"	&Pegar",                          HK_C   |      'V', MNID_PASTE,    MNFL_NORM, NULL},
	{"	&Eliminar",                                  HK_DEL, MNID_DELETE,   MNFL_NOHK, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	Seleccionar &todo",               HK_C   |      'A', MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	Co&mandos",                                 HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"		&Aumentar sangría",                      HK_TAB, MNID_NONE,     MNFL_NOHK, NULL},
	{"		&Reducir sangría",            HK_S   |   HK_TAB, MNID_NONE,     MNFL_NOHK, NULL},
	{"		-",                                     HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"		&Borrar línea",               HK_C   |      'K', MNID_NONE,     MNFL_NORM, NULL},
	{"		&Duplicar línea o selección", HK_C   |      'D', MNID_NONE,     MNFL_NORM, NULL},
	{"		-",                                     HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"		&Insertar línea debajo",      HK_C   | HK_ENTER, MNID_NONE,     MNFL_NORM, NULL},
	{"		I&nsertar línea encima",      HK_CS  | HK_ENTER, MNID_NONE,     MNFL_NORM, NULL},
	{"		-",                                     HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"		&Mover líneas hacia arriba",  HK_A   |  HK_PGUP, MNID_NONE,     MNFL_NORM, NULL},
	{"		M&over líneas hacia abajo",   HK_A   |  HK_PGDN, MNID_NONE,     MNFL_NORM, NULL},
	{"&Buscar",                                     HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Buscar...",                      HK_C   |      'F', MNID_NONE,     MNFL_NORM, NULL},
	{"	&Siguiente",                                  HK_F3, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Anterior",                       HK_S   |    HK_F3, MNID_NONE,     MNFL_NORM, NULL},
	{"	S&iguiente seleccionado",         HK_C   |    HK_F3, MNID_NONE,     MNFL_NORM, NULL},
	{"	A&nterior seleccionado",          HK_CS  |    HK_F3, MNID_NONE,     MNFL_NORM, NULL},
	{"	Ree&mplazar...",                  HK_C   |      'H', MNID_NONE,     MNFL_NORM, NULL},
	{"&Ver",                                        HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Espacios",                       HK_C   |      ' ', MNID_SPACES,   MNFL_NORM, NULL},
	{"	&Tabuladores",                    HK_CS  |      ' ', MNID_TABS,     MNFL_NORM, NULL},
	{"	&Nuevas líneas",                  HK_C   | HK_ENTER, MNID_NEWLS,    MNFL_NORM, NULL},
	{"	T&odos los caracteres",           HK_C   |      '.', MNID_ALLCHARS, MNFL_NORM, NULL},
	{"&Configuración",                              HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Preferencias...",                          HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Colores...",                               HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"Ve&ntana",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Siguiente",                      HK_C   |  HK_PGUP, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Anterior",                       HK_C   |  HK_PGDN, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Lista de ventanas...",           HK_C   |   HK_TAB, MNID_NONE,     MNFL_NORM, NULL},
	{"A&yuda",                                      HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Temas de ayuda",                             HK_F1, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Acerca de...",                   HK_A   |    HK_F1, MNID_NONE,     MNFL_NORM, NULL},
	{NULL,                                          HK_NONE, MNID_NONE,     MNFL_NORM, NULL}
};

//menú contextual
menudef_t contextmenudefs[] =
{  //Texto                            Tecla rápida       ID              Opciones   Función
	{"&Deshacer",                     HK_C   |      'Z', MNID_CTXUNDO,   MNFL_NOHK, NULL},
	{"&Rehacer",                      HK_C   |      'Y', MNID_CTXREDO,   MNFL_NOHK, NULL},
	{"-",                                       HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"C&ortar",                       HK_C   |      'X', MNID_CTXCUT,    MNFL_NOHK, NULL},
	{"&Copiar",                       HK_C   |      'C', MNID_CTXCOPY,   MNFL_NOHK, NULL},
	{"&Pegar",                        HK_C   |      'V', MNID_CTXPASTE,  MNFL_NOHK, NULL},
	{"&Eliminar",                                HK_DEL, MNID_CTXDELETE, MNFL_NOHK, NULL},
	{"-",                                       HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"Seleccionar &todo",             HK_C   |      'A', MNID_NONE,      MNFL_NOHK, NULL},
	{"-",                                       HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"Co&mandos",                               HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"	&Aumentar sangría",                      HK_TAB, MNID_NONE,      MNFL_NOHK, NULL},
	{"	&Reducir sangría",            HK_S   |   HK_TAB, MNID_NONE,      MNFL_NOHK, NULL},
	{"	-",                                     HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"	&Borrar línea",               HK_C   |      'K', MNID_NONE,      MNFL_NORM, NULL},
	{"	&Duplicar línea o selección", HK_C   |      'D', MNID_NONE,      MNFL_NORM, NULL},
	{"	-",                                     HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"	&Insertar línea debajo",      HK_C   | HK_ENTER, MNID_NONE,      MNFL_NORM, NULL},
	{"	I&nsertar línea encima",      HK_CS  | HK_ENTER, MNID_NONE,      MNFL_NORM, NULL},
	{"	-",                                     HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"	&Mover líneas hacia arriba",  HK_A   |  HK_PGUP, MNID_NONE,      MNFL_NORM, NULL},
	{"	M&over líneas hacia abajo",   HK_A   |  HK_PGDN, MNID_NONE,      MNFL_NORM, NULL},
	{NULL,                                      HK_NONE, MNID_NONE,      MNFL_NORM, NULL}
};

//inicializa la ventana del editor
void wndInit(void)
{
	startText();
	setpal(pal16);
	clear();
	refresh();
	getterminalsize(&wndW, &wndH);
	wndRedraw();
}

//termina la ventana del editor
void wndEnd(void)
{
	setpal(pal16_linux);
	resetcolor();
	clear();
	endText();
}

typedef struct
{
	unsigned int chr;
	unsigned char clr;
} cell_t;

//convierte un punto de código UTF-32 en una cadena UTF-8
void utf32toutf8(char *dest, unsigned int src)
{
	if(src < 0x80)
	{
		*dest++ = src;
		*dest = 0;
		return;
	}
	if(src < 0x800)
	{
		*dest++ = 0xc0 | (src >> 6);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return;
	}
	if(src < 0x10000)
	{
		*dest++ = 0xe0 | (src >> 12);
		*dest++ = 0x80 | ((src >> 6) & 0x3f);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return;
	}
	if(src < 0x110000)
	{
		*dest++ = 0xf0 | (src >> 18);
		*dest++ = 0x80 | ((src >> 12) & 0x3f);
		*dest++ = 0x80 | ((src >> 6) & 0x3f);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return;
	}
	*dest = 0;
}

//imprime texto en un búfer de celdas
//cell=puntero al array de celdas, deberá haber w*h celdas
//x=coordenada x
//y=coordenada y
//w=ancho del búfer
//h=alto del búfer
//str=puntero a la cadena
//clr=color del texto
void cellPrint(cell_t *buffer, int x, int y, int w, int h, char *str, unsigned char clr)
{
	unsigned int code;
	int numread;
	if(y >= h)
		return;
	buffer += x + y * w;
	w -= x;
	while(*str)
	{
		if(w == 0)
			return;
		w--;
		numread = 0;
		if((*str & 0x80) == 0)
		{
			code = *str;
		}
		else
		{
			if((*str & 0xe0) == 0xc0)
			{
				code = *str & 0x1f;
				numread = 1;
			}
			else
			{
				if((*str & 0xf0) == 0xe0)
				{
					code = *str & 0x0f;
					numread = 2;
				}
				else
				{
					if((*str & 0xf8) == 0xf0)
					{
						code = *str & 0x07;
						numread = 3;
					}
					else
					{
						code = '_';
					}
				}
			}
			while(numread)
			{
				str++;
				if((*str & 0xc0) != 0x80)
				{
					code = '_';
					break;
				}
				code <<= 6;
				code |= *str & 0x3f;
				numread--;
			}
		}
		buffer->chr = code;
		buffer->clr = clr;
		buffer++;
		str++;
	}
}

//redibuja la ventana del programa
//las dimensiones vienen dadas por wndW y wndH
void wndRedraw(void)
{
	int titleX = 0;
	int titleY = 0;
	int titleW = wndW;
	int titleH = 0;
	
	int menuX = 0;
	int menuY = 0;
	int menuW = wndW;
	int menuH = 1;
	
	int searchX = 0;
	int searchY = 0;
	int searchW = wndW;
	int searchH = 0;
	
	int tabX = 0;
	int tabY = 0;
	int tabW = wndW;
	int tabH = 0;
	
	int editX = 0;
	int editY = 0;
	int editW = wndW;
	int editH = wndH;
	
	int statusX = 0;
	int statusY = wndH - 1;
	int statusW = wndW;
	int statusH = 0;
	
	char str[8];
	cell_t *buffer;
	buffer = (cell_t *)malloc(wndW * wndH * sizeof(cell_t));
	if(buffer == NULL)
		return;
	
	//barra de título
	if(wndShowTitle != SHOWTITLE_NONE)
	{
		titleH++;
		for(int i = 0; i < titleW; i++)
			cellPrint(buffer, i+titleX, titleY, wndW, wndH, " ", colors[CLR_TITLE]);
		cellPrint(buffer, titleX + 2, titleY, wndW, wndH, "Editor de texto", colors[CLR_TITLE]);
	}
	menuY = titleY + titleH;
	//barra de menús
	for(int i = 0; i < menuW; i++)
		cellPrint(buffer, menuX + i, menuY, wndW, wndH, " ", colors[CLR_MENUBAR]);
	cellPrint(buffer, menuX, menuY, wndW, wndH, " Archivo  Edición  Buscar  Ver  Configuración  Ventana  Ayuda", colors[CLR_MENUBAR]);
	searchY = menuY + menuH;
	//barra de búsqueda
	if(wndShowSearchBar != SHOWSEARCHBAR_NONE)
	{
		searchH++;
		for(int i = 0; i < searchW; i++)
			cellPrint(buffer, searchX + i, searchY, wndW, wndH, " ", colors[CLR_SEARCHBAR]);
		cellPrint(buffer, searchX, searchY, wndW, wndH, "Búsqueda", colors[CLR_SEARCHBAR]);
	}
	tabY = searchY + searchH;
	//barra de pestañas
	if(wndShowTabs != SHOWTABS_NONE)
	{
		tabH++;
		for(int i = 0; i < tabW; i++)
			cellPrint(buffer, tabX + i, tabY, wndW, wndH, " ", colors[CLR_TABBAR]);
		cellPrint(buffer, tabX, tabY, wndW, wndH, " ArchivoUno.txt  FicheroDos.c  DocumentoTres.xml ", colors[CLR_TABBAR]);
	}
	editY = tabY + tabH;
	//barra de estado
	if(wndShowStatus != SHOWSTATUS_NONE)
	{
		statusH++;
		for(int i = 0; i < statusW; i++)
			cellPrint(buffer, statusX + i, statusY, wndW, wndH, " ", colors[CLR_STATUSBAR]);
		cellPrint(buffer, statusX, statusY, wndW, wndH, "Estado", colors[CLR_STATUSBAR]);
	}
	editH = wndH - editY - statusH;
	//editor
	for(int j = 0; j < editH - 1; j++)
	{
		//espacio de texto
		for(int i = 0; i < editW - 1; i++)
			cellPrint(buffer, editX + i, editY + j, wndW, wndH, " ", colors[CLR_EDIT_TEXT]);
		//espacio de barra de desplazamiento vertical
		cellPrint(buffer, editX + editW - 1, editY + j, wndW, wndH, scrl_space, colors[CLR_EDIT_SC_SPACE]);
	}
	cellPrint(buffer, editX + editW - 1, editY, wndW, wndH, scrl_up, colors[CLR_EDIT_SC_ARR]); //flecha arriba
	cellPrint(buffer, editX + editW - 1, editY + editH - 2, wndW, wndH, scrl_down, colors[CLR_EDIT_SC_ARR]); //flecha abajo
	//espacio de barra de desplazamiento horizontal
	for(int i = 0; i < editW - 3; i++)
		cellPrint(buffer, editX + i + 1, editY + editH - 1, wndW, wndH, scrl_space, colors[CLR_EDIT_SC_SPACE]);
	cellPrint(buffer, editX, editY + editH - 1, wndW, wndH, scrl_left, colors[CLR_EDIT_SC_ARR]); //flecha izquierda
	cellPrint(buffer, editX + editW - 2, editY + editH - 1, wndW, wndH, scrl_right, colors[CLR_EDIT_SC_ARR]); //flecha derecha
	cellPrint(buffer, editX + editW - 1, editY + editH - 1, wndW, wndH, c4, colors[CLR_EDIT_BORDER]); //borde abajo derecha
	//borrar pantalla
	setfb(7, 0);
	clear();
	//copiar búfer
	int index = 0;
	for(int j = 0; j < wndH; j++)
	{
		gotoxy(0, j);
		for(int i = 0; i < wndW; i++)
		{
			setcolor(buffer[index].clr);
			utf32toutf8(str, buffer[index].chr);
			if(str[0] == 0)
				tputs(" ");
			else
				tputs(str);
			index++;
		}
	}
	free(buffer);
	refresh();
}
