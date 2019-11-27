#include <stdlib.h>
#include "tui.hpp"
#include "term.hpp"

////TODO:
////2 búferes
////dibujar controles
////eventos de controles

//mostrar título
byte wndShowTitle = SHOWTITLE_NAME;
//mostrar barra de búsqueda
byte wndShowSearchBar = SHOWSEARCHBAR_ACTIVE;
//mostrar barra de pestañas
byte wndShowTabs = SHOWTABS_ACTIVE;
//mostrar barra de estado
byte wndShowStatus = SHOWSTATUS_ACTIVE;
//ubicación de la posición
byte wndPosLocation = POSLOCATION_LSTATUS;

//colores de los elementos
byte colors[] =
{
	0x0a, 0x01, //título
	0x00, 0x07, //barra de menús
	0x00, 0x02, //barra de menús, seleccionado
	0x04, 0x07, //barra de menús, tecla
	0x04, 0x02, //barra de menús, tecla, seleccionado
	0x08, 0x07, //barra de menús, deshabilitado
	0x08, 0x02, //barra de menús, deshabilitado, seleccionado
	0x00, 0x07, //menú
	0x0f, 0x01, //menú, seleccionado
	0x01, 0x07, //menú, tecla
	0x0b, 0x01, //menú, tecla, seleccionado
	0x08, 0x07, //menú, deshabilitado
	0x08, 0x01, //menú, deshabilitado, seleccionado
	0x00, 0x07, //barra de búsqueda
	0x00, 0x0f, //barra de búsqueda, texto
	0x04, 0x0f, //barra de búsqueda, borrar
	0x0b, 0x03, //barra de búsqueda, botón
	0x00, 0x08, //barra de pestañas
	0x00, 0x0b, //barra de pestañas, pestaña activa
	0x00, 0x07, //barra de pestañas, pestaña inactiva
	0x0f, 0x0c, //barra de pestañas, botón cerrar
	0x01, 0x0f, //barra de pestañas, flecha habilitada
	0x08, 0x07, //barra de pestañas, flecha deshabilitada
	0x07, 0x01, //editor, texto
	0x01, 0x07, //editor, texto seleccionado
	0x0a, 0x01, //editor, borde
	0x07, 0x00, //editor, barra, flecha
	0x07, 0x01, //editor, barra, espacio
	0x07, 0x00, //editor, barra, control
	0x0f, 0x03  //barra de estado
};

//caracteres gráficos
const char *gchars[] =
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
const char *modkeynames[HK_KEY_NUMMODS] = {"Ctrl", "Alt", "Mayús"};
//nombres de las teclas de acceso rápido (HK_GETC(x) - 1)
const char *keynames[HK_KEY_NUMKEYS] =
{
	"F1", "F2", "F3", "F4", "F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12",
	"Ins", "Supr", "Inicio", "Fin", "RePág", "AvPág",
	"Arriba", "Abajo", "Izquierda", "Derecha",
	"Intro", "Retroceso", "Tab", "Escape"
};

//convierte un punto de código UTF-32 en una cadena UTF-8
//dest=cadena donde se escribirá el punto de código UTF-8
//src=punto de código UTF-32
//devuelve la longitud de la cadena sin el terminador nulo (de 0 a 4)
//en la cadena se escribirán entre 1 y 5 bytes (incluyendo el terminador nulo)
//si src está fuera de rango, se devolverá el punto de código 0xfffd (carácter de reemplazo)
int UTF32_UTF8(char *dest, unsigned int src)
{
	if(src < 0x80)
	{
		*dest = src;
		if(src)
		{
			dest[1] = 0;
			return(1);
		}
		return(0);
	}
	if(src < 0x800)
	{
		*dest++ = 0xc0 | (src >> 6);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return(2);
	}
	if(src < 0x10000)
	{
		*dest++ = 0xe0 | (src >> 12);
		*dest++ = 0x80 | ((src >> 6) & 0x3f);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return(3);
	}
	if(src < 0x110000)
	{
		*dest++ = 0xf0 | (src >> 18);
		*dest++ = 0x80 | ((src >> 12) & 0x3f);
		*dest++ = 0x80 | ((src >> 6) & 0x3f);
		*dest++ = 0x80 | (src & 0x3f);
		*dest = 0;
		return(4);
	}
	return(UTF32_UTF8(dest, 0xfffd));
}

//convierte un punto de código UTF-8 en UTF-32
//str=cadena UTF-8 de la cual se obtendrá el primer punto de código
//offset=aquí se devolverá el número de bytes que avanzar para llegar al inicio del siguiente punto de código en la cadena
//devuelve el punto de código en UTF-32
//las secuencias de bytes no válidas serán devueltas como 0xfffd (carácter de reemplazo)
unsigned int UTF8_UTF32(const char *str, int *offset)
{
	unsigned int code;
	int numread = 0, num = 1;
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
					code = 0xfffd;
				}
			}
		}
		while(numread)
		{
			str++;
			if((*str & 0xc0) != 0x80)
			{
				code = 0xfffd;
				break;
			}
			num++;
			code <<= 6;
			code |= *str & 0x3f;
			numread--;
		}
	}
	if(offset)
		*offset = num;
	return(code);
}

//devuelve el número de puntos de código UTF-8 que tiene una cadena
int strlen_cp(const char *str)
{
	int offset, r = 0;
	while(UTF8_UTF32(str, &offset))
	{
		r++;
		str += offset;
	}
	return(r);
}

//imprime texto en un búfer de celdas
//buffer=puntero al array de celdas, deberá haber w*h celdas
//x=coordenada x
//y=coordenada y
//w=ancho del búfer
//h=alto del búfer
//str=puntero a la cadena
//fg=color de primer plano
//bg=color de fondo
void cellPrint(cell_t *buffer, int x, int y, int w, int h, const char *str, byte fg, byte bg)
{
	unsigned int code;
	int offset;
	if(y < 0 || y >= h)
		return;
	while(x < 0 && *str)
	{
		UTF8_UTF32(str, &offset);
		str += offset;
		x++;
	}
	buffer += x + y * w;
	while(*str)
	{
		if(x >= w)
			return;
		code = UTF8_UTF32(str, &offset);
		str += offset;
		buffer->fg = fg;
		buffer->bg = bg;
		buffer->chr = code;
		buffer++;
		x++;
	}
}

//imprime el texto de un menú
//buffer=puntero al array de celdas, deberá haber w*h celdas
//x=coordenada x
//y=coordenada y
//w=ancho del búfer
//h=alto del búfer
//str=puntero a la cadena
//textClr=color del texto
//keyClr=color del carácter enfatizado
//devuelve la longitud en puntos de código del texto sin el ampersand
int printmn(cell_t *buffer, int x, int y, int w, int h, const char *str, byte textFG, byte textBG, byte keyFG, byte keyBG)
{
	unsigned int code;
	int offset, len = 0;
	byte fg = textFG, bg = textBG;
	while(x < 0 && *str)
	{
		if(UTF8_UTF32(str, &offset) != '&')
		{
			len++;
			x++;
			fg = textFG;
			bg = textBG;
		}
		else
		{
			fg = keyFG;
			bg = keyBG;
		}
		str += offset;
	}
	buffer += x + y * w;
	while(*str)
	{
		code = UTF8_UTF32(str, &offset);
		str += offset;
		if(code  != '&')
		{
			if((x < w) && (y >= 0) && (y < h))
			{
				buffer->chr = code;
				buffer->fg = fg;
				buffer->bg = bg;
				buffer++;
				len++;
				fg = textFG;
				bg = textBG;
			}
			x++;
		}
		else
		{
			fg = keyFG;
			bg = keyBG;
		}
	}
	return(len);
}

//menú principal
menudef_t menudefs_test[] = ////
{  //Texto                                Tecla rápida       ID             Opciones   Función
	{"&Archivo",                                    HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Nuevo",                          HK_C   |      'N', 0,             MNFL_NORM, NULL},
	{"	&Abrir...",                       HK_C   |      'O', 0,             MNFL_NORM, NULL},
	{"	&Guardar",                        HK_C   |      'S', 0,             MNFL_NORM, NULL},
	{"	Guardar &como...",                HK_A   |      'S', 0,             MNFL_NORM, NULL},
	{"	Guardar copia co&mo...",          HK_CA  |      'S', 0,             MNFL_NORM, NULL},
	{"	&Renombrar...",                               HK_F2, 0,             MNFL_NORM, NULL},
	{"	Ce&rrar",                         HK_C   |    HK_F4, 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Salir",                          HK_C   |      'Q', 0,             MNFL_NORM, NULL},
	{"&Edición",                                    HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Deshacer",                       HK_C   |      'Z', 0,             MNFL_NORM, NULL},
	{"	&Rehacer",                        HK_C   |      'Y', 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	C&ortar",                         HK_C   |      'X', 0,             MNFL_NORM, NULL},
	{"	&Copiar",                         HK_C   |      'C', 0,             MNFL_NORM, NULL},//funciona, pero no Ctrl+Ins
	{"	&Pegar",                          HK_C   |      'V', 0,             MNFL_NORM, NULL},//funciona, pero no Mayús+Ins
	{"	&Eliminar",                                  HK_DEL, 0,             MNFL_NOHK, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Reemplazar caracteres",                     HK_INS, 0,             MNFL_NOHK, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	Seleccionar &todo",               HK_C   |      'A', 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	Co&mandos",                                 HK_NONE, 0,             MNFL_NORM, NULL},
	{"		&Aumentar sangría",                      HK_TAB, 0,             MNFL_NOHK, NULL},
	{"		&Reducir sangría",            HK_S   |   HK_TAB, 0,             MNFL_NOHK, NULL},
	{"		-",                                     HK_NONE, 0,             MNFL_NORM, NULL},
	{"		&Borrar línea",               HK_C   |      'K', 0,             MNFL_NORM, NULL},
	{"		&Duplicar línea o selección", HK_C   |      'D', 0,             MNFL_NORM, NULL},
	{"		-",                                     HK_NONE, 0,             MNFL_NORM, NULL},
	{"		&Insertar línea debajo",      HK_C   | HK_ENTER, 0,             MNFL_NORM, NULL},
	{"		I&nsertar línea encima",      HK_A   | HK_ENTER, 0,             MNFL_NORM, NULL},
	{"		-",                                     HK_NONE, 0,             MNFL_NORM, NULL},
	{"		&Mover líneas hacia arriba",  HK_A   |  HK_PGUP, 0,             MNFL_NORM, NULL},
	{"		M&over líneas hacia abajo",   HK_A   |  HK_PGDN, 0,             MNFL_NORM, NULL},
	{"&Buscar",                                     HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Buscar...",                      HK_C   |      'F', 0,             MNFL_NORM, NULL},
	{"	&Siguiente",                                  HK_F3, 0,             MNFL_NORM, NULL},
	{"	&Anterior",                       HK_S   |    HK_F3, 0,             MNFL_NORM, NULL},
	{"	S&iguiente seleccionado",         HK_C   |    HK_F3, 0,             MNFL_NORM, NULL},
	{"	A&nterior seleccionado",          HK_CS  |    HK_F3, 0,             MNFL_NORM, NULL},
	{"	Ree&mplazar...",                  HK_C   |      'H', 0,             MNFL_NORM, NULL},
	{"&Ver",                                        HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Espacios",                                 HK_NONE, 0,             MNFL_NORM, NULL},//no funciona
	{"	&Tabuladores",                              HK_NONE, 0,             MNFL_NORM, NULL},//no funciona
	{"	&Nuevas líneas",                            HK_NONE, 0,             MNFL_NORM, NULL},
	{"	T&odos los caracteres",                     HK_NONE, 0,             MNFL_NORM, NULL},//no funciona
	{"&Configuración",                              HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Preferencias...",                          HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Colores...",                               HK_NONE, 0,             MNFL_NORM, NULL},
	{"Ve&ntana",                                    HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Siguiente ventana",                          HK_F6, 0,             MNFL_NORM, NULL},
	{"	Ventana &anterior",               HK_S   |    HK_F6, 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	S&iguiente división",             HK_C   |    HK_F6, 0,             MNFL_NORM, NULL},
	{"	División a&nterior",              HK_CS  |    HK_F6, 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	Si&guiente pestaña",              HK_C   |   HK_F12, 0,             MNFL_NORM, NULL},
	{"	Pestaña an&terior",               HK_C   |   HK_F11, 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Lista de ventanas...",           HK_A   |      '0', 0,             MNFL_NORM, NULL},
	{"-",                                           HK_NONE, 0,             MNFL_NORM, NULL},
	{"A&yuda",                                      HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Temas de ayuda",                             HK_F1, 0,             MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, 0,             MNFL_NORM, NULL},
	{"	&Acerca de...",                   HK_S   |    HK_F1, 0,             MNFL_NORM, NULL},
	{NULL,                                          HK_NONE, 0,             MNFL_NORM, NULL}
};

// Inicializa el entorno de texto.
// Devuelve 0 si se pudo, 1 si no hay memoria suficiente.
byte TTui::Start(void)
{
	cell_t *temp;
	maxW = 256;
	maxH = 256;
	term.Start();
	term.Clear();
	term.Refresh();
	term.GetSize(&wndW, &wndH);
	temp = (cell_t *)malloc(wndW * wndH * sizeof(cell_t) * 2);
	if(temp == NULL)
	{
		term.End();
		return(1);
	}
	screenBuffer1 = temp;
	screenBuffer2 = temp + wndW * wndH;
	return(0);
}

// Cierra el entorno de texto.
void TTui::End(void)
{
	term.SetFgColor(-1);
	term.SetBgColor(-1);
	term.Clear();
	term.End();
	if(screenBuffer1 != NULL)
	{
		free(screenBuffer1);
		screenBuffer1 = NULL;
		screenBuffer2 = NULL;
	}
}

int redraw = 0;////
// Redibuja el entorno de texto si es necesario. También comprueba si hay que redimensionar.
void TTui::Update(void)
{
	int wndW_new, wndH_new;
	cell_t *temp;
	term.GetSize(&wndW_new, &wndH_new);
	if(maxW && wndW_new > maxW)
		wndW_new = maxW;
	if(maxH && wndH_new > maxH)
		wndH_new = maxH;
	if(wndW_new != wndW || wndH_new != wndH)
	{
		temp = (cell_t *)realloc(screenBuffer1, wndW_new * wndH_new * sizeof(cell_t) * 2);
		if(temp != NULL)
		{
			wndW = wndW_new;
			wndH = wndH_new;
			screenBuffer1 = temp;
			screenBuffer2 = temp + wndW * wndH;
			redraw = 1;
		}
	}
	if(redraw)
	{
		redraw = 0;
		TEST_REDRAW();////
	}
}

int test_scroll=0;////
void TTui::TEST_REDRAW(void)
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
	buffer = screenBuffer1;
	
	//barra de título
	if(wndShowTitle != SHOWTITLE_NONE)
	{
		titleH++;
		for(int i = 0; i < titleW; i++)
			cellPrint(buffer, i+titleX, titleY, wndW, wndH, " ", colors[CLRF_TITLE], colors[CLRB_TITLE]);
		cellPrint(buffer, /*titleX + 1*/(titleW - (int)strlen_cp("Editor de texto")) / 2, titleY, wndW, wndH, "Editor de texto", colors[CLRF_TITLE], colors[CLRB_TITLE]);
	}
	menuY = titleY + titleH;
	//barra de menús
	for(int i = 0; i < menuW; i++)
		cellPrint(buffer, menuX + i, menuY, wndW, wndH, " ", colors[CLRF_MENUBAR], colors[CLRB_MENUBAR]);
	int mnid = 0;
	for(int i = 0; menudefs_test[i].caption; i++)
	{
		if(menudefs_test[i].caption[0] == 9)
			continue;
		if(menudefs_test[i].caption[0] == '-' && menudefs_test[i].caption[1] == 0)
		{
			menuX += printmn(buffer, menuX, menuY, wndW, wndH, "|", colors[CLRF_MENUBAR], colors[CLRB_MENUBAR], colors[CLRF_MENUBAR_KEY], colors[CLRB_MENUBAR_KEY]);
			continue;
		}
		unsigned char c_a_f = CLRF_MENUBAR;
		unsigned char c_a_b = CLRB_MENUBAR;
		unsigned char c_b_f = CLRF_MENUBAR_KEY;
		unsigned char c_b_b = CLRB_MENUBAR_KEY;
		if(mnid==1){c_a_f+=2;c_b_f+=2;c_a_b+=2;c_b_b+=2;}
		if(mnid==2){c_a_f+=8;c_b_f=c_a_f;c_a_b+=8;c_b_b=c_a_b;}
		if(mnid==3){c_a_f+=10;c_b_f=c_a_f;c_a_b+=10;c_b_b=c_a_b;}
		c_a_f = colors[c_a_f];
		c_b_f = colors[c_b_f];
		c_a_b = colors[c_a_b];
		c_b_b = colors[c_b_b];
		menuX += printmn(buffer, menuX, menuY, wndW, wndH, " ", c_a_f, c_a_b, c_b_f, c_b_b);
		menuX += printmn(buffer, menuX, menuY, wndW, wndH, menudefs_test[i].caption, c_a_f, c_a_b, c_b_f, c_b_b);
		menuX += printmn(buffer, menuX, menuY, wndW, wndH, " ", c_a_f, c_a_b, c_b_f, c_b_b);
		mnid++;
	}
	searchY = menuY + menuH;
	//barra de búsqueda
	if(wndShowSearchBar != SHOWSEARCHBAR_NONE)
	{
		searchH++;
		//barra
		for(int i = 0; i < searchW; i++)
			cellPrint(buffer, searchX + i, searchY, wndW, wndH, " ", colors[CLRF_SEARCHBAR], colors[CLRB_SEARCHBAR]);
		int search_x = 1;
		//caja de texto de búsqueda
		for(int i = 0; i < 16; i++)
			cellPrint(buffer, searchX + search_x + i, searchY, wndW, wndH, " ", colors[CLRF_SEARCHBAR_TXT], colors[CLRB_SEARCHBAR_TXT]);
		search_x += 16;
		//botón borrar
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, "<X ", colors[CLRF_SEARCHBAR_DEL], colors[CLRB_SEARCHBAR_DEL]);
		search_x += strlen_cp("<X ") + 1;
		//botón buscar anterior
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, " <B ", colors[CLRF_SEARCHBAR_BTN], colors[CLRB_SEARCHBAR_BTN]);
		search_x += strlen_cp(" <B ") + 1;
		//botón buscar siguiente
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, " B> ", colors[CLRF_SEARCHBAR_BTN], colors[CLRB_SEARCHBAR_BTN]);
		search_x += strlen_cp(" B> ") + 1;
		//separador
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, "|", colors[CLRF_SEARCHBAR], colors[CLRB_SEARCHBAR]);
		search_x += strlen_cp("|") + 1;
		//caja de texto de ir
		for(int i = 0; i < 8; i++)
			cellPrint(buffer, searchX + i + search_x, searchY, wndW, wndH, " ", colors[CLRF_SEARCHBAR_TXT], colors[CLRB_SEARCHBAR_TXT]);
		search_x += 8;
		//botón borrar
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, "<X ", colors[CLRF_SEARCHBAR_DEL], colors[CLRB_SEARCHBAR_DEL]);
		search_x += strlen_cp("<X ") + 1;
		//botón ir
		cellPrint(buffer, searchX + search_x, searchY, wndW, wndH, " Ir ", colors[CLRF_SEARCHBAR_BTN], colors[CLRB_SEARCHBAR_BTN]);
		search_x += strlen_cp(" Ir ") + 1;
	}
	tabY = searchY + searchH;
	//barra de pestañas
	if(wndShowTabs != SHOWTABS_NONE)
	{
		tabH++;
		//barra
		for(int i = 0; i < tabW; i++)
			cellPrint(buffer, tabX + i, tabY, wndW, wndH, " ", colors[CLRF_TABBAR], colors[CLRB_TABBAR]);
		int tab_x = 3;
		//pestañas
		for(int i = 0; i < 4; i++)
		{
			const char *list[]={" ArchivoUno.txt     ", " FicheroDos.c     ", " DocumentoTres.xml     ", " Edit.exe     "};
			cellPrint(buffer, tabX +  tab_x, tabY, wndW, wndH, list[i], colors[i==1?CLRF_TABBAR_ACTIVE:CLRF_TABBAR_INACTIVE], colors[i==1?CLRB_TABBAR_ACTIVE:CLRB_TABBAR_INACTIVE]);
			tab_x += strlen_cp(list[i]);
			cellPrint(buffer, tabX + tab_x - 4, tabY, wndW, wndH, "[X]", colors[CLRF_TABBAR_CLOSE], colors[CLRB_TABBAR_CLOSE]);
			if(i != 3)
			{
				cellPrint(buffer, tabX + tab_x, tabY, wndW, wndH, "|", colors[CLRF_TABBAR], colors[CLRB_TABBAR]);
				tab_x += 1;
			}
		}
		//flechas
		cellPrint(buffer, tabX, tabY, wndW, wndH, "[ ]", colors[CLRF_TABBAR_ARROW], colors[CLRB_TABBAR_ARROW]);
		cellPrint(buffer, tabX + 1, tabY, wndW, wndH, gchars[GCH_SCRL_LEFT], colors[CLRF_TABBAR_ARROW], colors[CLRB_TABBAR_ARROW]);
		cellPrint(buffer, tabX + tabW - 3, tabY, wndW, wndH, "[ ]", colors[CLRF_TABBAR_ARROW_DIS], colors[CLRB_TABBAR_ARROW_DIS]);
		cellPrint(buffer, tabX + tabW - 2, tabY, wndW, wndH, gchars[GCH_SCRL_RIGHT], colors[CLRF_TABBAR_ARROW_DIS], colors[CLRB_TABBAR_ARROW_DIS]);
	}
	editY = tabY + tabH;
	//barra de estado
	if(wndShowStatus != SHOWSTATUS_NONE)
	{
		statusH++;
		for(int i = 0; i < statusW; i++)
			cellPrint(buffer, statusX + i, statusY, wndW, wndH, " ", colors[CLRF_STATUSBAR], colors[CLRB_STATUSBAR]);
		cellPrint(buffer, statusX, statusY, wndW, wndH, "Estado", colors[CLRF_STATUSBAR], colors[CLRB_STATUSBAR]);
	}
	editH = wndH - editY - statusH;
	//editor
	for(int j = 0; j < editH - 1; j++)
	{
		//espacio de texto
		for(int i = 0; i < editW - 1; i++)
			cellPrint(buffer, editX + i, editY + j, wndW, wndH, " ", colors[CLRF_EDIT_TEXT], colors[CLRB_EDIT_TEXT]);
		//barra de desplazamiento vertical
		int len = editH / 4;
		if(test_scroll < 0)test_scroll = 0;
		int start = test_scroll + 1;
		int end = start + len;
		int diff = end - (editH - 2);
		if(diff > 0){start -= diff; end -= diff; test_scroll -= diff;}
		if((j >= start) && (j < end))
			cellPrint(buffer, editX + editW - 1, editY + j, wndW, wndH, gchars[GCH_SCRL_THUMB], colors[CLRF_EDIT_SC_THUMB], colors[CLRB_EDIT_SC_THUMB]);
		else
			cellPrint(buffer, editX + editW - 1, editY + j, wndW, wndH, gchars[GCH_SCRL_SPACE], colors[CLRF_EDIT_SC_SPACE], colors[CLRB_EDIT_SC_SPACE]);
	}
	cellPrint(buffer, editX + editW - 1, editY, wndW, wndH, gchars[GCH_SCRL_UP], colors[CLRF_EDIT_SC_ARR], colors[CLRB_EDIT_SC_ARR]); //flecha arriba
	cellPrint(buffer, editX + editW - 1, editY + editH - 2, wndW, wndH, gchars[GCH_SCRL_DOWN], colors[CLRF_EDIT_SC_ARR], colors[CLRB_EDIT_SC_ARR]); //flecha abajo
	//barra de desplazamiento horizontal
	for(int i = 0; i < editW - 3; i++)
		if(i < editW / 4)
			cellPrint(buffer, editX + i + 1, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_THUMB], colors[CLRF_EDIT_SC_THUMB], colors[CLRB_EDIT_SC_THUMB]);
		else
			cellPrint(buffer, editX + i + 1, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_SPACE], colors[CLRF_EDIT_SC_SPACE], colors[CLRB_EDIT_SC_SPACE]);
	cellPrint(buffer, editX, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_LEFT], colors[CLRF_EDIT_SC_ARR], colors[CLRB_EDIT_SC_ARR]); //flecha izquierda
	cellPrint(buffer, editX + editW - 2, editY + editH - 1, wndW, wndH, gchars[GCH_SCRL_RIGHT], colors[CLRF_EDIT_SC_ARR], colors[CLRB_EDIT_SC_ARR]); //flecha derecha
	cellPrint(buffer, editX + editW - 1, editY + editH - 1, wndW, wndH, gchars[GCH_BOX2_C4], colors[CLRF_EDIT_BORDER], colors[CLRB_EDIT_BORDER]); //borde abajo derecha
	//borrar pantalla
	//setfb(7, 0);
	//clear();
	//copiar búfer
	int index = 0;
	for(int j = 0; j < wndH; j++)
	{
		term.GotoXY(0, j);
		for(int i = 0; i < wndW; i++)
		{
			int fg, bg;
			fg = buffer[index].fg;
			bg = buffer[index].bg;
			if(fg == 255)
				fg = -1;
			if(bg == 255)
				bg = -1;
			term.SetFgColor(fg);
			term.SetBgColor(bg);
			UTF32_UTF8(str, buffer[index].chr);
			if(str[0] == 0)
				term.Print(" ");
			else
				term.Print(str);
			index++;
		}
	}
	term.Refresh();
}
