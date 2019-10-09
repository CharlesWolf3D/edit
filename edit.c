#include "ventana.h"
#include "term.h"

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

#if 0 //setup de ncurses
//instalar ncurses (rpm): sudo yum install ncurses-devel ncurses
//instalar ncurses (deb): sudo apt-get install libncurses5-dev libncursesw5-dev
//compilar: gcc -std=c99 -O2 edit.c -o edit -lcurses
#include <stdio.h>
#include <unistd.h>
int putenv(char *string); //#include <stdlib.h>
#include <curses.h>
#include <signal.h>

int pal2[6] =
{
	000, 000, 000,
	255, 255, 255
};

int pal4[12] =
{
	000, 000, 000,
	128, 128, 128,
	192, 192, 192,
	255, 255, 255
};

int pal16[48] =
{
	  0,   0,   0,
	  0,   0, 128,
	  0, 128,   0,
	  0, 128, 128,
	128,   0,   0,
	128,   0, 128,
	128, 128,   0,
	192, 192, 192,
	128, 128, 128,
	  0,   0, 255,
	  0, 255,   0,
	  0, 255, 255,
	255,   0,   0,
	255,   0, 255,
	255, 255,   0,
	255, 255, 255
};

WINDOW *mywindow;
int setupColor(void)
{
	#define SUPCLRS 3
	int supclr[SUPCLRS] = {2, 4, 16/*, 256*/};
	int *pals[SUPCLRS] = {pal2, pal4, pal16/*, pal256*/};
	char bw;      //si es terminal monócroma
	char xcolors; //si no se pueden cambiar los colores predeterminados
	int numColors;
	int *pal;
	
	//obtener capacidades de color
	bw = !has_colors();
	xcolors = !can_change_color();
	//habilitar color
	start_color();
	//obtener número de colores soportados
	numColors = supclr[0];
	pal = pals[0];
	if(!bw && (COLOR_PAIRS >= numColors * numColors))
		for(int i = 1; i < SUPCLRS; i++)
			if(COLORS >= supclr[i])
			{
				numColors = supclr[i];
				pal = pals[i];
			}
	//establecer paleta
	if(!xcolors)
		for(int i = 0; i < numColors; i++)
			init_color(i, pal[i * 3] * 1000 / 255, pal[i * 3 + 1] * 1000 / 255, pal[i * 3 + 2] * 1000 / 255);
	//establecer pares
	if(numColors == 16 && xcolors)
	{
		for(int j = 0; j < numColors; j++)
			for(int i = 0; i < numColors; i++)
				init_pair(j * numColors + i, subst[i], subst[j]);
	}
	else
	{
		for(int j = 0; j < numColors; j++)
			for(int i = 0; i < numColors; i++)
				init_pair(j * numColors + i, i, j);
	}
	return(numColors);
}

void clearScr(void)
{
	for(int j = 0; j < LINES; j++)
	{
		move(j, 0);
		for(int i = 0; i < COLS; i++)
		{
			echochar(' ' | COLOR_PAIR(0x71));
		}
	}
}

void cursesFinish(int sig)
{
	clearScr();
	refresh();
	endwin();
}

int startText(void (*resizeHandler)(int sig))
{
	char *vars[] =
	{
		"xterm-16color",
		"xterm-256color",
		"rxvt-16color",
		"konsole-16color",
		NULL
	};
	int numColors;
	int index = 0;
	putenv("TERM=xterm-256color");
	if(resizeHandler != NULL)
		signal(SIGWINCH, resizeHandler); //establecer manejador de cambio de tamaño
	signal(SIGINT, cursesFinish);
	mywindow = initscr();
	numColors = setupColor();
	while((numColors < 16) && (vars[index] != NULL))
	{
		endwin();
		newterm(vars[index++], stdout, stdin);
		mywindow = initscr();
		numColors = setupColor();
	}
	cbreak(); //no poner caracteres entrantes en búfer
	noecho(); //no mostrar caracteres entrantes automáticamente
	keypad(stdscr, TRUE); //permitir recibir teclas especiales
	nodelay(stdscr, TRUE); //hacer que getch no espere (y devuelva ERR si no se ha pulsado una tecla)
	return(numColors);
}

void endText(void)
{
	signal(SIGWINCH, NULL);
	clearScr();
	refresh();
	endwin();
}

volatile int resized = 0;

void resizeHandler(int sig)
{
	endwin();                 //parar y reiniciar ncurses porque no es capaz de enviar más de una señal de cambio de tamaño
	startText(resizeHandler); //si no se reinicia
	//clear();
	//refresh(); //ncurses no refresca la pantalla si esta línea no se pone, aunque la refresquemos luego

	//attr_set(0, 0x17, NULL);
	//border('|', '|', '-', '-', '/', '\\', '\\', '/');
	
	//move(2, 2);
	//attr_set(0, 0x1a, NULL);
	//printw("%4i*%4i", COLS, LINES);
	//refresh();
	resized = 1;
}
#endif


int main(int argc, char *argv[])
{
	int oldw = -1, oldh = -1;
	wndInit();
	while(1)
	{
		if(kbhit())
		{
			if(getchr() == 27)
				break;
		}
		getterminalsize(&wndW, &wndH);
		if(oldw != wndW || oldh != wndH)
		{
			oldw = wndW;
			oldh = wndH;
			wndRedraw();
		}
		usleep(50 * 1000);
	}
	wndEnd();
	return(0);
	
	#if 0 //main de ncurses
	int numColors = startText(resizeHandler);
	resized = 1;
	if(numColors < 16)
	{
		endText();
		printf("La terminal no admite 16 colores.\nAsegúrese de que la terminal es compatible con \"TERM=xterm-256color\".\n");
	}
	//int w,h;
	//getmaxyx(stdscr, h, w);
	//move(w, h);
	//refresh();
	//while(getch()==ERR);
	int key;
	while(1)
	{
		if(resized)
		{
			//clearScr();
			move(0, 0);
			attr_set(0, 0x17, NULL);
			border('|', '|', '-', '-', '/', '\\', '\\', '/');
			for(int j = 0; j < numColors; j++)
			{
				move(j + 2, 2);
				for(int i = 0; i < numColors; i++)
				{
					attr_set(0, i + j * numColors, NULL);
					printw("%02x",i + j * numColors);
					//addch("0123456789ABCDEF"[j] | COLOR_PAIR(i + j * numColors));
					//addch("0123456789ABCDEF"[i] | COLOR_PAIR(i + j * numColors));
				}
				printw("\n");
			}
			resized = 0;
			refresh();
		}
		key=getch();
		if(key == ERR)continue;
		if(key == 'q')break;
		if(key == 'w'){move(0, 0);continue;}
		if(key == 'e'){move(LINES - 1, COLS - 1);continue;}
		if(key >= 'a' && key <= 'z')
		{
			attr_set(0, 0x1a, NULL);
			printw("I am highlighted-");
			echochar(key|COLOR_PAIR(0xa1));//addch(key|COLOR_PAIR(0xa1));
			refresh();
		}
	}
	endText();
	return(0);
	#endif
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
