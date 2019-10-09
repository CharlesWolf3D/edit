#include <stdlib.h>
#include "tui.h"
#include "term.h"

////TODO:
////2 búferes
////dibujar controles
////eventos de controles

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
	0xf0, //barra de búsqueda, texto
	0xf4, //barra de búsqueda, borrar
	0x3b, //barra de búsqueda, botón
	0x80, //barra de pestañas
	0xb0, //pestaña activa
	0x70, //pestaña inactiva
	0xcf, //botón de cerrar pestaña
	0x17, //editor, texto
	0x71, //editor, texto seleccionado
	0x1a, //editor, borde
	0xb3, //editor, barra, flecha
	0x0f, //editor, barra, espacio
	0xb0, //editor, barra, control
	0x3f  //barra de estado
};

//caracteres gráficos
char *gchars[] =
{
	"\u250c", //caja1, arriba izquierda
	"\u2510", //caja1, arriba derecha
	"\u2514", //caja1, abajo izquierda
	"\u2518", //caja1, abajo derecha
	"\u2500", //caja1, arriba
	"\u2500", //caja1, abajo
	"\u2502", //caja1, izquierda
	"\u2502", //caja1, derecha
	"\u252c", //caja1, T abajo
	"\u2534", //caja1, T arriba
	"\u251c", //caja1, T derecha
	"\u2524", //caja1, T izquierda
	"\u2554", //caja2, arriba izquierda
	"\u2557", //caja2, arriba derecha
	"\u255a", //caja2, abajo izquierda
	"\u255d", //caja2, abajo derecha
	"\u2550", //caja2, arriba
	"\u2550", //caja2, abajo
	"\u2551", //caja2, izquierda
	"\u2551", //caja2, derecha
	"\u2566", //caja2, T abajo
	"\u2569", //caja2, T arriba
	"\u2560", //caja2, T derecha
	"\u2562", //caja2, T izquierda
	"\u25b2", //barra despl., flecha arriba
	"\u25bc", //barra despl., flecha abajo
	"\u25c4", //barra despl., flecha izquierda
	"\u25ba", //barra despl., flecha derecha
	"\u2591", //barra despl., espacio
	"\u2588", //barra despl., control
	"\u25a0", //menú, icono de verificación
	"\u2022", //menú, icono de opción
	"\u2591", //progreso, vacío
	"\u2588"  //progreso, lleno
};

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
		cellPrint(buffer, titleX + 1/*(titleW - strlen("Editor de texto")) / 2*/, titleY, wndW, wndH, "Editor de texto", colors[CLR_TITLE]);
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
		//barra
		for(int i = 0; i < searchW; i++)
			cellPrint(buffer, searchX + i, searchY, wndW, wndH, " ", colors[CLR_SEARCHBAR]);
		//caja de texto de búsqueda
		for(int i = 0; i < 16; i++)
			cellPrint(buffer, searchX + i + 1, searchY, wndW, wndH, " ", colors[CLR_SEARCHBAR_TXT]);
		//botón borrar
		cellPrint(buffer, searchX + 17, searchY, wndW, wndH, "<X ", colors[CLR_SEARCHBAR_DEL]);
		//botón buscar anterior
		cellPrint(buffer, searchX + 21, searchY, wndW, wndH, " <B ", colors[CLR_SEARCHBAR_BTN]);
		//botón buscar siguiente
		cellPrint(buffer, searchX + 26, searchY, wndW, wndH, " B> ", colors[CLR_SEARCHBAR_BTN]);
		//separador
		cellPrint(buffer, searchX + 31, searchY, wndW, wndH, "|", colors[CLR_SEARCHBAR]);
		//caja de texto de ir
		for(int i = 0; i < 8; i++)
			cellPrint(buffer, searchX + i + 33, searchY, wndW, wndH, " ", colors[CLR_SEARCHBAR_TXT]);
		//botón borrar
		cellPrint(buffer, searchX + 41, searchY, wndW, wndH, "<X ", colors[CLR_SEARCHBAR_DEL]);
		//botón ir
		cellPrint(buffer, searchX + 45, searchY, wndW, wndH, " Ir ", colors[CLR_SEARCHBAR_BTN]);
	}
	tabY = searchY + searchH;
	//barra de pestañas
	if(wndShowTabs != SHOWTABS_NONE)
	{
		tabH++;
		for(int i = 0; i < tabW; i++)
			cellPrint(buffer, tabX + i, tabY, wndW, wndH, " ", colors[CLR_TABBAR]);
		cellPrint(buffer, tabX +  0, tabY, wndW, wndH, " ArchivoUno.txt     ", colors[CLR_TABBAR_INACTIVE]);
		cellPrint(buffer, tabX + 16, tabY, wndW, wndH, "[X]", colors[CLR_TABBAR_CLOSE]);
		cellPrint(buffer, tabX + 20, tabY, wndW, wndH, " FicheroDos.c     ", colors[CLR_TABBAR_ACTIVE]);
		cellPrint(buffer, tabX + 34, tabY, wndW, wndH, "[X]", colors[CLR_TABBAR_CLOSE]);
		cellPrint(buffer, tabX + 38, tabY, wndW, wndH, " DocumentoTres.xml     ", colors[CLR_TABBAR_INACTIVE]);
		cellPrint(buffer, tabX + 57, tabY, wndW, wndH, "[X]", colors[CLR_TABBAR_CLOSE]);
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
		cellPrint(buffer, editX + editW - 1, editY + j, wndW, wndH, gchars[GCH_SCRL_SPACE], colors[CLR_EDIT_SC_SPACE]);
	}
	cellPrint(buffer, editX + editW - 1, editY, wndW, wndH, gchars[GCH_SCRL_UP], colors[CLR_EDIT_SC_ARR]); //flecha arriba
	cellPrint(buffer, editX + editW - 1, editY + editH - 2, wndW, wndH, gchars[GCH_SCRL_DOWN], colors[CLR_EDIT_SC_ARR]); //flecha abajo
	//espacio de barra de desplazamiento horizontal
	for(int i = 0; i < editW - 3; i++)
		cellPrint(buffer, editX + i + 1, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_SPACE], colors[CLR_EDIT_SC_SPACE]);
	cellPrint(buffer, editX, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_LEFT], colors[CLR_EDIT_SC_ARR]); //flecha izquierda
	cellPrint(buffer, editX + editW - 2, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_RIGHT], colors[CLR_EDIT_SC_ARR]); //flecha derecha
	cellPrint(buffer, editX + editW - 1, editY + editH - 1, wndW, wndH, gchars[GCH_BOX2_C4], colors[CLR_EDIT_BORDER]); //borde abajo derecha
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
