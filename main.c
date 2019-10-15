#include <unistd.h>

#include "tui.h"

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

/*
modos:
normal, editor enfocado
normal, barra de búsqueda enfocada
menú seleccionado
mostrando cuadro de diálogo
	abrir
	guardar como
	renombrar
	buscar
	reemplazar
	preferencias
	colores
	lista de ventanas
	temas de ayuda
	acerca de
	error
		al guardar
	error+diálogo
		al abrir
		al guardar como
		al renombrar

--Titulo------------------------------------------------------------------------
-Archivo--Edición--Buscar--Ver--Configuración--Ventana--Ayuda-------------------
-_________<[Buscar]--_____<[Ir]-------------------------------------------------
<-Pestaña1--Pestaña2----------------------------------------------------------->
-------------------------------------------------------------------------------^
-------------------------------------------------------------------------------#
-------------------------------------------------------------------------------#
-------------------------------------------------------------------------------V
<#############################################################################>-
-LIN:COL------------------------------------------------------------------------

Titulo: nombre (barra), ruta y nombre (barra), desactivado
Menú: activado
Búsqueda: activado, desactivado
Pestañas: activado, desactivado
Editor: activado (puede tener posición en barra de desplazamiento)
Estado: Posición, bloques
Posición: izquierda del titulo, derecha del titulo, derecha del menú, barra de desplazamiento, barra de estado (barra)

*/

int main(int argc, char *argv[])
{
	int oldw = -1, oldh = -1;
	int ch;
	int finish = 0;
	int modo = 0;
	char str[64];
	wndInit();
	while(!finish)
	{
		if(kbhit())
		{
			if(modo==0)
			{
				ch = getchr();
				switch(ch)
				{
				case 'q':
					finish = 1;
					break;
				case 'w':
					clear();
					refresh();
					break;
				case 'z':
					modo = 1;
					tputs("Entrando en modo procesado.\n\r");
					refresh();
					break;
				default:
					int2hex2(ch,str);
					tputs("Char ");
					tputs(str);
					switch(ch)
					{
					case 13:
					case 10:
					case 9:
					case 27:
					case 0:
						break;
					default:
						tputs(" (");
						str[0]=ch;str[1]=0;
						tputs(str);
						tputs(")");
					}
					tputs("\n\r");
					refresh();
				}
			}
			else
			{
				unsigned int key = getKey();
				if(key == HK_ESC)finish = 1;
				if(key == 'z' || key == 'Z'){tputs("Entrando en modo sin procesar.\n\r");modo = 0;}
				if(key&HK_C)tputs("Ctrl+");
				if(key&HK_A)tputs("Alt+");
				if(key&HK_S)tputs("Mayús+");
				unsigned char chr = (key & HK_KMASK) >> 16;
				if(chr)
				{
					chr--;
					char*names[]={"F1","F2","F3","F4","F5","F6","F7","F8","F9","F10","F11","F12","Ins","Supr","Inicio","Fin","RePág","AvPág","Arriba","Abajo","Izquierda","Derecha","Intro","Retroceso","Tab","Esc"};
					tputs(names[chr]);
				}
				else
				{
					chr = key & HK_CHMASK;
					if(chr >= 32 && chr <= 126)
					{
						if(chr == 32)
							tputs("Espacio");
						else
						{
							str[0] = chr;
							str[1] = 0;
							tputs(str);
						}
					}
					else
					{
						tputs("<");
						int2str(chr, str);
						tputs(str);
						tputs(">");
					}
				}
				tputs("\n\r");
				refresh();
			}
		}
		else
			usleep(50 * 1000);
		/*getterminalsize(&wndW, &wndH);
		if(oldw != wndW || oldh != wndH)
		{
			oldw = wndW;
			oldh = wndH;
			wndRedraw();
		}*/
	}
	wndEnd();
	return(0);
}

#if 0 //tipo de cursor y leer paleta
//tipos de cursor de texto para setcursor
#define CURSOR_DEFAULT   0x00 //predeterminado de la terminal (bloque en linux)
#define CURSOR_UNDERLINE 0x01 //subrayado
#define CURSOR_BLOCK     0x02 //bloque
#define CURSOR_BAR       0x03 //barra vertical (sólo terminales gráficas)
#define CURSOR_BLINKING  0x00 //parpadeo activado
#define CURSOR_STOPPED   0x10 //parpadeo desactivado (sólo terminales gráficas)
//establece el tipo de cursor de texto
void setcursor(unsigned char cursorType)
{
	char nums[8] = {'0', '3', '1', '5', '0', '4', '2', '6'};
	char command[6] = {27, '[', 0, ' ', 'q', 0};
	command[2] = nums[(cursorType & 0x3) | ((cursorType & 0x10) >> 2)];
	tputs(command);
}
//obtiene los 16 colores de la paleta
//devuelve 0 si se pudo, 1 si error
int getpal(unsigned char *pal)
{
	char num[8];
	unsigned char idx, col;
	unsigned char ch;
	unsigned int amount, len;
	tputs("\x1b]4");
	for(int i = 0; i < 16; i++)
	{
		idx = subst[i] * 3;
		tputs(";");
		int2str(i, num); tputs(num);
		tputs(";?");
	}
	tputs("\x1b\\");
	refresh();
	if(read(1, &ch, 1) != 1)return(1); if(ch != 27)return(1);
	if(read(1, &ch, 1) != 1)return(1); if(ch != ']')return(1);
	if(read(1, &ch, 1) != 1)return(1); if(ch != '4')return(1);
	for(int i = 0; i < 16; i++)
	{
		//;
		if(read(1, &ch, 1) != 1)return(1); if(ch != ';')return(1);
		//idx
		idx = 0;
		if(read(1, &ch, 1) != 1)return(1);
		while(ch != ';')
		{
			if((ch >= '0') && (ch <= '9'))
				ch -= '0';
			else
				return(1);
			idx *= 10; idx += ch;
			if(read(1, &ch, 1) != 1)return(1);
		}
		if(idx != i)return(1);
		idx = subst[idx];
		//rgb:
		if(read(1, &ch, 1) != 1)return(1); if(ch != 'r')return(1);
		if(read(1, &ch, 1) != 1)return(1); if(ch != 'g')return(1);
		if(read(1, &ch, 1) != 1)return(1); if(ch != 'b')return(1);
		if(read(1, &ch, 1) != 1)return(1); if(ch != ':')return(1);
		//r/g/b
		for(int x = 0; x < 3; x++)
		{
			amount = 0; len = 0;
			if(read(1, &ch, 1) != 1)return(1);
			while(ch != '/' && ch != 27 && ch != ';')
			{
				if((ch >= '0') && (ch <= '9')) ch -= '0';
				else if((ch >= 'A') && (ch <= 'F')) ch = ch - 'A' + 10;
				else if((ch >= 'a') && (ch <= 'f')) ch = ch - 'a' + 10;
				else return(1);
				amount *= 10; amount += ch;
				if(read(1, &ch, 1) != 1)return(1);
				len++;
			}
			if(ch == ';' && x != 2)return(1);
			if(ch == '/' && x == 2)return(1);
			if(ch == 27 && (x != 2 || i != 15))return(1);
			switch(len)
			{
				case 1: col = (amount & 0xf) | ((amount << 4) & 0xf0); break;
				case 2: col = amount & 0xff; break;
				case 3: col = (amount >> 4) & 0xff; break;
				case 4: col = (amount >> 8) & 0xff; break;
				default: return(1);
			}
			pal[idx * 3 + 2] = col;
		}
		//fin
		if(ch != 27)return(1);
		if(read(1, &ch, 1) != 1)return(1); if(ch != '\\')return(1);
	}
	return(0);
}
/*
ESC]4;1;rgb:ffff/ffff/0000ESC\
*/
#endif
