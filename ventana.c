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

int clr_title =       0x1a;
int clr_menu =        0x70;
int clr_search =      0x70;
int clr_search_txt =  0xf0;
int clr_search_btn =  0xf9;
int clr_tab =         0x70;
int clr_edit_text =   0x1f;
int clr_edit_border = 0x1a;
int clr_scrl_arr =    0xb2;
int clr_scrl_space =  0x1e;
int clr_scrl_thumb =  0x2a;
int clr_status =      0x70;

unsigned char colors[] =
{
	0x1a, //título
	0x70, //barra de menús
	0x74, //barra de menús, tecla
	0x20, //barra de menús, seleccionado
	0x24, //barra de menús, seleccionado, tecla
	0x70, //menú
	0x74, //menú, tecla
	0x20, //menú, seleccionado
	0x24, //menú, seleccionado, tecla
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

enum
{
	CLR_TITLE,
	CLR_MENUBAR,
	CLR_MENUBAR_KEY,
	CLR_MENUBAR_SEL,
	CLR_MENUBAR_SEL_KEY,
	CLR_MENU,
	CLR_MENU_KEY,
	CLR_MENU_SEL,
	CLR_MENU_SELKEY,
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
