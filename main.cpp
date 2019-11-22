#include <unistd.h>

#include "tui.hpp"

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
	MNID_CTXDELETE,
	MNID_WNDMOVE,
	MNID_WNDSIZE,
	MNID_WNDMIN,
	MNID_WNDMAX,
	MNID_WNDCLOSE
};

/*
Combinaciones de teclas:

Combinación          | Descripción                                               | Jodido | Alternativa si está jodido
---------------------|-----------------------------------------------------------|--------|----------------------------
Flechas              | Mover cursor                                              |        | 
Mayús+Flechas        | Mover cursor seleccionando                                |        | 
Ctrl+Flechas         | Mover cursor a siguiente palabra o párrafo                |        | 
Ctrl+Mayús+Flechas   | Mover cursor a siguiente palabra o párrafo seleccionando  | Sí     | Alt+WSAD
Alt+Flechas          | Desplazar vista                                           | Sí     | Alt+IKJL
Alt+Mayús+Flechas    | Selección en bloque                                       |        | 
---------------------|-----------------------------------------------------------|--------|----------------------------
Ins                  | Modo insertar                                             |        | 
Supr                 | Eliminar carácter a la derecha                            |        | 
Retroceso            | Eliminar carácter a la izquierda                          |        | 
---------------------|-----------------------------------------------------------|--------|----------------------------
Inicio               | Ir al principio de la línea                               |        | 
Fin                  | Ir al final de la línea                                   |        | 
RePág                | Ir a la página anterior                                   |        | 
AvPág                | Ir a la página siguiente                                  |        | 
Mayús+Inicio         | Ir al principio de la línea seleccionando                 | Sí     | Alt+,
Mayús+Fin            | Ir al final de la línea seleccionando                     | Sí     | Alt+.
Mayús+RePág          | Ir a la página anterior seleccionando                     | Sí     | Alt+N
Mayús+AvPág          | Ir a la página siguiente seleccionando                    | Sí     | Alt+M
Ctrl+Inicio          | Ir al principio del documento                             | Sí     | Alt+RePág
Ctrl+Fin             | Ir al final del documento                                 | Sí     | Alt+AvPág
---------------------|-----------------------------------------------------------|--------|----------------------------
F10                  | Seleccionar barra de menús                                | Sí     | F4
F7                   | Seleccionar buscar/ir/editor                              |        | 
F5                   | Actualizar (explorador)                                   |        | 
Ctrl+F5              | Restablecer colores                                       |        | 
Alt+-                | Menú de ventana                                           |        | (Mayús+F4 queda mejor)
---------------------|-----------------------------------------------------------|--------|----------------------------
Ctrl+N               | Archivo -> Nuevo                                          |        | 
Ctrl+O               | Archivo -> Abrir...                                       |        | 
Ctrl+S               | Archivo -> Guardar                                        |        | 
Ctrl+Mayús+S         | Archivo -> Guardar como...                                | Sí     | Alt+S
Ctrl+Alt+S           | Archivo -> Guardar copia como...                          |        | 
F2                   | Archivo -> Renombrar...                                   |        | 
Ctrl+F4              | Archivo -> Cerrar                                         |        | 
Alt+F4               | Archivo -> Salir                                          | Sí     | Ctrl+Q
---------------------|-----------------------------------------------------------|--------|----------------------------
Ctrl+Z               | Edición -> Deshacer                                       |        | 
Ctrl+Y               | Edición -> Rehacer                                        |        | 
Ctrl+X               | Edición -> Cortar                                         |        | 
Ctrl+C               | Edición -> Copiar                                         |        | 
Ctrl+V               | Edición -> Pegar                                          |        | 
Ctrl+A               | Edición -> Seleccionar todo                               |        | 
---------------------|-----------------------------------------------------------|--------|----------------------------
Tab                  | Edición -> Comandos -> Aumentar sangría                   |        | 
Mayús+Tab            | Edición -> Comandos -> Reducir sangría                    |        | 
Ctrl+K               | Edición -> Comandos -> Borrar línea                       |        | 
Ctrl+D               | Edición -> Comandos -> Duplicar línea o selección         |        | 
Ctrl+Intro           | Edición -> Comandos -> Insertar línea debajo              |        | 
Ctrl+Mayús+Intro     | Edición -> Comandos -> Insertar línea encima              | Sí     | Alt+Intro
Alt+RePág            | Edición -> Comandos -> Mover líneas hacia arriba          |        | 
Alt+AvPág            | Edición -> Comandos -> Mover líneas hacia abajo           |        | 
---------------------|-----------------------------------------------------------|--------|----------------------------
Ctrl+F               | Buscar -> Buscar...                                       |        | 
F3                   | Buscar -> Siguiente                                       |        | 
Mayús+F3             | Buscar -> Anterior                                        |        | 
Ctrl+F3              | Buscar -> Siguiente Seleccionado                          |        | 
Ctrl+Mayús+F3        | Buscar -> Anterior Seleccionado                           |        | 
Ctrl+H               | Buscar -> Reemplazar...                                   |        | 
---------------------|-----------------------------------------------------------|--------|----------------------------
Ctrl+Espacio         | Ver -> Espacios                                           | Sí     | Ajo y agua
Ctrl+Mayús+Tab       | Ver -> Tabuladores                                        | Sí     | Ajo y agua
Ctrl+Intro           | Ver -> Nuevas líneas                                      |        | 
Ctrl+.               | Ver -> Todos los caracteres                               | Sí     | Ajo y agua
---------------------|-----------------------------------------------------------|--------|----------------------------
F6                   | Ventana -> Siguiente ventana                              |        | 
Mayús+F6             | Ventana -> Ventana anterior                               |        | 
Ctrl+F6              | Ventana -> Siguiente división                             |        | 
Ctrl+Mayús+F6        | Ventana -> División Anterior                              |        | 
Ctrl+AvPág           | Ventana -> Siguiente pestaña                              | Sí     | Ctrl+F12
Ctrl+RePág           | Ventana -> Pestaña anterior                               | Sí     | Ctrl+F11
Ctrl+Tab             | Ventana -> Lista de ventanas...                           | Sí     | Alt+0
---------------------|-----------------------------------------------------------|--------|----------------------------
F1                   | Ayuda -> Temas de ayuda                                   | Sí     | Ajo y agua
Alt+F1               | Ayuda -> Acerca de...                                     | Sí     | Mayús+F1

Para abrir un menú con el teclado:
	Pulsar F4, seleccionar con las flechas, pulsar Intro.
	Pulsar F4, seleccionar y abrir con la letra del menú.
Ya que Alt+Letra no funciona porque algunas letras se utilizan en otros comandos.

*/

//menú principal
menudef_t menudefs[] =
{  //Texto                                Tecla rápida       ID             Opciones   Función
	{"&Archivo",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Nuevo",                          HK_C   |      'N', MNID_NONE,     MNFL_NORM, NULL},
	{"	&Abrir...",                       HK_C   |      'O', MNID_NONE,     MNFL_NORM, NULL},
	{"	&Guardar",                        HK_C   |      'S', MNID_SAVE,     MNFL_NORM, NULL},
	{"	Guardar &como...",                HK_A   |      'S', MNID_SAVEAS,   MNFL_NORM, NULL},
	{"	Guardar copia co&mo...",          HK_CA  |      'S', MNID_SAVECOPY, MNFL_NORM, NULL},
	{"	&Renombrar...",                               HK_F2, MNID_RENAME,   MNFL_NORM, NULL},
	{"	Ce&rrar",                         HK_C   |    HK_F4, MNID_CLOSE,    MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Salir",                          HK_C   |      'Q', MNID_NONE,     MNFL_NORM, NULL},
	{"&Edición",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Deshacer",                       HK_C   |      'Z', MNID_UNDO,     MNFL_NORM, NULL},
	{"	&Rehacer",                        HK_C   |      'Y', MNID_REDO,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	C&ortar",                         HK_C   |      'X', MNID_CUT,      MNFL_NORM, NULL},
	{"	&Copiar",                         HK_C   |      'C', MNID_COPY,     MNFL_NORM, NULL},//funciona, pero no Ctrl+Ins
	{"	&Pegar",                          HK_C   |      'V', MNID_PASTE,    MNFL_NORM, NULL},//funciona, pero no Mayús+Ins
	{"	&Eliminar",                                  HK_DEL, MNID_DELETE,   MNFL_NOHK, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Reemplazar caracteres",                     HK_INS, MNID_DELETE,   MNFL_NOHK, NULL},
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
	{"		I&nsertar línea encima",      HK_A   | HK_ENTER, MNID_NONE,     MNFL_NORM, NULL},
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
	{"	&Espacios",                                 HK_NONE, MNID_SPACES,   MNFL_NORM, NULL},
	{"	&Tabuladores",                              HK_NONE, MNID_TABS,     MNFL_NORM, NULL},
	{"	&Nuevas líneas",                            HK_NONE, MNID_NEWLS,    MNFL_NORM, NULL},
	{"	T&odos los caracteres",                     HK_NONE, MNID_ALLCHARS, MNFL_NORM, NULL},
	{"&Configuración",                              HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Preferencias...",                          HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Colores...",                               HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"Ve&ntana",                                    HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Siguiente ventana",                          HK_F6, MNID_NONE,     MNFL_NORM, NULL},
	{"	Ventana &anterior",               HK_S   |    HK_F6, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	S&iguiente división",             HK_C   |    HK_F6, MNID_NONE,     MNFL_NORM, NULL},
	{"	División a&nterior",              HK_CS  |    HK_F6, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	Si&guiente pestaña",              HK_C   |   HK_F12, MNID_NONE,     MNFL_NORM, NULL},
	{"	Pestaña an&terior",               HK_C   |   HK_F11, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Lista de ventanas...",           HK_A   |      '0', MNID_NONE,     MNFL_NORM, NULL},
	{"A&yuda",                                      HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Temas de ayuda",                             HK_F1, MNID_NONE,     MNFL_NORM, NULL},
	{"	-",                                         HK_NONE, MNID_NONE,     MNFL_NORM, NULL},
	{"	&Acerca de...",                   HK_S   |    HK_F1, MNID_NONE,     MNFL_NORM, NULL},
	{NULL,                                          HK_NONE, MNID_NONE,     MNFL_NORM, NULL}
};

//menú contextual del editor
menudef_t menudefs_ctx[] =
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
	{"	I&nsertar línea encima",      HK_A   | HK_ENTER, MNID_NONE,      MNFL_NORM, NULL},
	{"	-",                                     HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"	&Mover líneas hacia arriba",  HK_A   |  HK_PGUP, MNID_NONE,      MNFL_NORM, NULL},
	{"	M&over líneas hacia abajo",   HK_A   |  HK_PGDN, MNID_NONE,      MNFL_NORM, NULL},
	{NULL,                                      HK_NONE, MNID_NONE,      MNFL_NORM, NULL}
};

//menú de ventana
menudef_t menudefs_wnd[] =
{  //Texto                            Tecla rápida       ID              Opciones   Función
	{"&Mover",                                  HK_NONE, MNID_WNDMOVE,   MNFL_NORM, NULL},
	{"&Tamaño",                                 HK_NONE, MNID_WNDSIZE,   MNFL_NORM, NULL},
	{"Mi&nimizar",                              HK_NONE, MNID_WNDMIN,    MNFL_NORM, NULL},
	{"Ma&ximizar",                              HK_NONE, MNID_WNDMAX,    MNFL_NORM, NULL},
	{"-",                                       HK_NONE, MNID_NONE,      MNFL_NORM, NULL},
	{"&Cerrar",                       HK_C   |    HK_F4, MNID_WNDCLOSE,  MNFL_NOHK, NULL},
	{NULL,                                      HK_NONE, MNID_NONE,      MNFL_NORM, NULL}
};

/*
hacer:
tui.c
	2 búferes
	tipo de datos control
	dibujar controles
	eventos de controles
	mejorar eventos de ratón
controls.c
	control etiqueta
	control texto
	control botón
edit.c
	control editor
menu.c
	control menú

--Titulo------------------------------------------------------------------------
-Archivo--Edición--Buscar--Ver--Configuración--Ventana--Ayuda-------------------
-________________<X_-_<B_-_B>_-|-_____<X_-_Ir_----------------------------------
[<]_Pestaña1_[X]_|_Pestaña2_[X]_---------------------------------------------[>]
-------------------------------------------------------------------------------^
-------------------------------------------------------------------------------#
-------------------------------------------------------------------------------#
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
	unsigned char chr;
	int finish = 0;
	int modo = 0;
	int redraw = 0;
	char str[64];
	unsigned int key;
	wndInit();
	while(!finish)
	{
		if(kbhit())
		{
			const char *btns[4]={"Pulsar izq","Pulsar cent","Pulsar der","Soltar"};
			switch(modo)
			{
			case 0:
				ch = getchr();
				switch(ch)
				{
				case 'q': finish = 1; break;
				case 'w': clear(); refresh(); break;
				case 27:
					clear(); refresh();
					if(!kbhit())break; ch = getchr(); if(ch != '[')break;
					if(!kbhit())break; ch = getchr(); if(ch != 'M')break;
					int n1, n2, n3;
					if(!kbhit())break; n1 = getchr();
					if(!kbhit())break; n2 = getchr();
					if(!kbhit())break; n3 = getchr();
					tputs("X = "); int2str((unsigned char)(n2 - 33), str); tputs(str); tputs("\n\r");
					tputs("Y = "); int2str((unsigned char)(n3 - 33), str); tputs(str); tputs("\n\r");
					tputs("B = "); int2hex2(n1, str); tputs(str); tputs("\n\r");
					if((n1 & 0x60) == 0x60)
						tputs((n1 & 1) ? "Rueda abajo" : "Rueda arriba");
					else
						tputs(btns[n1 & 3]);
					if(n1 & 0x04)tputs(" [Mayús]");
					if(n1 & 0x08)tputs(" [Alt]");
					if(n1 & 0x10)tputs(" [Ctrl]");
					tputs("\n\r");
					refresh();
					break;
				case ' ': tputs("Entrando en modo procesado.\n\r"); refresh(); modo = 1; break;
				default:
					clear();
					int2hex2(ch,str);
					tputs(str);
					if(ch >= 32)
					{
						tputs(" (");
						str[0] = ch; str[1] = 0;
						tputs(str);
						tputs(")");
					}
					tputs("\n\r");
					
					while(kbhit())
					{
						ch = getchr();
						int2hex2(ch,str);
						tputs(str);
						if(ch >= 32)
						{
							tputs(" (");
							str[0] = ch; str[1] = 0;
							tputs(str);
							tputs(")");
						}
						tputs("\n\r");
					}
					refresh();
				}
				break;
			case 1:
				key = getKey();
				chr = HK_GETC(key);
				if(key & HK_C)tputs("Ctrl+");
				if(key & HK_A)tputs("Alt+");
				if(key & HK_S)tputs("Mayús+");
				if(HK_TYPEC(key))
				{
					
					if(chr == ' '){redraw = 1; modo = 2;}
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
				if(HK_TYPEK(key))
				{
					if(chr == HK_ESC)finish = 1;
					if(chr)
					{
						chr--;
						tputs(keynames[chr]);
					}
				}
				if(HK_TYPEM(key))
				{
					if(key & HK_M1)tputs("M1");
					if(key & HK_M2)tputs("M2");
					if(key & HK_M3)tputs("M3");
					if(key & HK_WHUP)tputs("Rueda arriba");
					if(key & HK_WHDN)tputs("Rueda abajo");
					tputs(" X=");
					int2str(HK_MX(key),str);tputs(str);
					tputs(" Y=");
					int2str(HK_MY(key),str);tputs(str);
				}
				tputs("\n\r");
				refresh();
				break;
			case 2:
				key = getKey();
				if(HK_TYPEK(key) && HK_GET(key) == HK_ESC)finish = 1;
				if(HK_TYPEC(key) && HK_GET(key) == ' '){resetcolor(); clear(); tputs("Entrando en modo sin procesar.\n\r"); refresh(); modo = 0;}
				if(HK_TYPEM(key)){if(HK_GETC(key)==HK_WHUP){test_scroll--;redraw=1;}if(HK_GETC(key)==HK_WHDN){test_scroll++;redraw=1;}}
				break;
			}
		}
		else
			usleep(20 * 1000); //20=>50Hz, 16=>60Hz
		if(modo == 2)
		{
			getterminalsize(&wndW, &wndH);
			if(oldw != wndW || oldh != wndH || redraw)
			{
				oldw = wndW;
				oldh = wndH;
				redraw = 0;
				wndRedraw();
			}
		}
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
