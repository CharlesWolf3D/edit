#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "term.h"

unsigned char pal16[48] =
{
	  0,   0,   0,
	  0,   0, 160,
	  0, 160,   0,
	  0, 160, 160,
	160,   0,   0,
	160,   0, 160,
	160, 160,   0,
	216, 216, 216,
	160, 160, 160,
	  0,   0, 255,
	  0, 255,   0,
	  0, 255, 255,
	255,   0,   0,
	255,   0, 255,
	255, 255,   0,
	255, 255, 255
};

unsigned char pal16_linux[48] =
{
	 46,  52,  54,
	 52, 101, 164,
	 78, 154,   6,
	  6, 152, 154,
	204,   0,   0,
	117,  80, 123,
	196, 160,   0,
	211, 215, 207,
	 85,  87,  83,
	114, 159, 207,
	138, 226,  52,
	 52, 226, 226,
	239,  41,  41,
	173, 127, 168,
	252, 233,  79,
	238, 238, 236
};

//números de color que enviar mediante comandos ANSI
static unsigned char subst[16] =  { 0,  4,  2,  6,  1,  5,  3,  7,  8, 12, 10, 14,  9, 13, 11, 15};

static unsigned char textAttr = ATTR_NONE; //últimos atributos de caracteres seleccionados
static int textFG = -1; //último color de primer plano seleccionado
static int textBG = -1; //último color de fondo seleccionado

//búfer temporal para enviar datos a stdout en bloques
#define TERMBUFSZ 8192
char termBuffer[TERMBUFSZ];
int termBufferCount = 0;

static struct termios savedTtyState; //estado de stdin salvado

//escribe una cadena en la terminal
//la almacena en un búfer y sólo escribe en el dispositivo cuando éste se llena
//o se llama a refresh()
void tputs(char *str)
{
	while(*str)
	{
		if(termBufferCount == TERMBUFSZ)
		{
			termBufferCount = 0;
			write(STDOUT_FILENO, termBuffer, TERMBUFSZ);
		}
		termBuffer[termBufferCount++] = *str;
		str++;
	}
}

//escribe en la terminal la parte del búfer que todavía no ha sido escrita
void refresh(void)
{
	if(termBufferCount)
	{
		write(STDOUT_FILENO, termBuffer, termBufferCount);
		termBufferCount = 0;
	}
}

//establece los atributos de los caracteres que se van a escribir
void setattr(unsigned char attr)
{
	unsigned char change;
	change = attr ^ textAttr;
	if(!change)
		return;
	if(change & ATTR_BOLD)
		tputs(attr & ATTR_BOLD ?      "\x1b[1m" : "\x1b[22m");
	if(change & ATTR_ITALIC)
		tputs(attr & ATTR_ITALIC ?    "\x1b[3m" : "\x1b[23m");
	if(change & ATTR_UNDERLINE)
		tputs(attr & ATTR_UNDERLINE ? "\x1b[4m" : "\x1b[24m");
	if(change & ATTR_STRIKE)
		tputs(attr & ATTR_STRIKE ?    "\x1b[9m" : "\x1b[29m");
	textAttr = attr;
}

//convierte un número entero en una cadena en base hexadecimal, utilizando
//siempre 2 dígitos
static char hexdigits [16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
/*static inline*/ void int2hex2(int x, char *str)
{
	str[0] = hexdigits[(x >> 4) & 0xf];
	str[1] = hexdigits[(x) & 0xf];
	str[2] = 0;
}

//establece los 16 colores de la paleta
void setpal(unsigned char *pal)
{
	char num[8];
	unsigned char idx;
	tputs("\x1b]4");
	for(int i = 0; i < 16; i++)
	{
		idx = subst[i] * 3;
		tputs(";");
		int2str(i, num); tputs(num);
		tputs(";rgb:");
		int2hex2(pal[idx],     num); tputs(num); tputs("/");
		int2hex2(pal[idx + 1], num); tputs(num); tputs("/");
		int2hex2(pal[idx + 2], num); tputs(num);
	}
	tputs("\x1b\\");
}

//inicia el sistema de terminal
void startText(void)
{
	struct termios ttystate;
	//configurar stdin
	tcgetattr(STDIN_FILENO, &ttystate);      //obtener atributos de stdin
	tcgetattr(STDIN_FILENO, &savedTtyState); //
	ttystate.c_lflag &= ~ICANON; //no esperar a nueva línea para recibir caracteres
	ttystate.c_lflag &= ~ISIG;   //no generar señales de INTR/QUIT/SUSP/DSUSP (comentar esta línea en debug para poder salir con Ctrl+C)
	ttystate.c_lflag &= ~IEXTEN; //no tratar de forma especial Ctrl+V
	ttystate.c_lflag &= ~ECHO;   //no imprimir los caracteres entrantes
	ttystate.c_iflag &= ~IXON;   //no tratar de forma especial Ctrl+S y Ctrl+Q
	ttystate.c_iflag &= ~ICRNL;  //no tratar de forma especial Ctrl+M y Ctrl+J
	ttystate.c_oflag &= ~OPOST;  //no modificar fin de línea
	ttystate.c_cc[VMIN] = 1;     //recibir caracteres inmediatamente
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate); //enviar atributos de stdin
	//inicializar variables
	termBufferCount = 0;
	textAttr = ATTR_NONE;
	textFG = textBG = -1;
	//enviar comandos a la terminal
	tputs
	(
	"\x1b[?7l"         //disablewrap
	"\x1b[0m"          //setattr(ATTR_NONE)
	"\x1b[39m\x1b[49m" //resetcolor
	"\x1b[?25l"        //hidecursor
	);
	refresh();
}

//cierra el sistema de terminal
void endText(void)
{
	setpal(pal16_linux);
	//enviar comandos a la terminal
	tputs
	(
	"\x1b[?7h"         //enablewrap
	"\x1b[0m"          //setattr(ATTR_NONE)
	"\x1b[39m\x1b[49m" //resetcolor
	"\x1b[?25h"        //showcursor
	);
	refresh();
	//restaurar atributos de stdin salvados
	tcsetattr(STDIN_FILENO, TCSANOW, &savedTtyState);
}

//devuelve si hay datos disponibles en stdin
int kbhit(void)
{
	struct timeval tv;
	fd_set fds;
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);
	select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
	return(FD_ISSET(STDIN_FILENO, &fds));
}

//lee un carácter de stdin (0..255)
//devuelve -1 si hubo un error al leer
int getchr(void)
{
	unsigned char ch;
	if(read(STDIN_FILENO, &ch, 1) != 1)
		return(-1);
	return(ch);
}

//convierte un número entero en una cadena en base decimal
void int2str(int x, char *str)
{
	char buf[10];
	unsigned char i = 0;
	unsigned int ux;
	if(x == 0)
		*str++ = '0';
	else
	{
		if(x < 0)
		{
			*str++ = '-';
			x = -x;
		}
		ux = (unsigned int)x;
		while(ux)
		{
			buf[i++] = (ux % 10) + '0';
			ux /= 10;
		}
		while(i)
			*str++ = buf[--i];
	}
	*str = 0;
}

//borra la pantalla y pone el cursor en 0,0
void clear(void)
{
	tputs("\x1b[2J\x1b[1;1H");
}

//pone el cursor en x,y
void gotoxy(int x, int y)
{
	char num[8];
	tputs("\x1b[");
	int2str(y + 1, num);
	tputs(num);
	tputs(";");
	int2str(x + 1, num);
	tputs(num);
	tputs("H");
}

//establece el color de primer plano
void setfg(int clr)
{
	char num[8];
	if(clr == textFG)
		return;
	if(clr == -1)
	{
		textFG = -1;
		tputs("\x1b[39m");
		return;
	}
	textFG = clr & 0xf;
	tputs("\x1b[38;5;");
	int2str(subst[textFG], num);
	tputs(num);
	tputs("m");
}

//establece el color de fondo
void setbg(int clr)
{
	char num[8];
	if(clr == textBG)
		return;
	if(clr == -1)
	{
		textBG = -1;
		tputs("\x1b[49m");
		return;
	}
	textBG = clr & 0xf;
	tputs("\x1b[48;5;");
	int2str(subst[textBG], num);
	tputs(num);
	tputs("m");
}

//establece los colores de primer plano y fondo (dos parámetros)
void setfb(int f, int b)
{
	setfg(f);
	setbg(b);
}

//establece los colores de primer plano y fondo (un parámetro con los colores combinados, no permite poner color predeterminado)
void setcolor(int clr)
{
	setfg(clr);
	setbg(clr >> 4);
}

//selecciona el color de primer plano predeterminado
void resetfg(void)
{
	tputs("\x1b[39m");
	textFG = -1;
}

//selecciona el color de fondo predeterminado
void resetbg(void)
{
	tputs("\x1b[49m");
	textBG = -1;
}

//selecciona los colores de primer plano y fondo predeterminados
void resetcolor(void)
{
	tputs("\x1b[39m\x1b[49m");
	textFG = textBG = -1;
}

//oculta el cursor de texto
void hidecursor(void)
{
	tputs("\x1b[?25l");
}

//muestra el cursor de texto
void showcursor(void)
{
	tputs("\x1b[?25h");
}

//habilita la nueva línea automática al imprimir más allá del fin de la línea
void enablewrap(void)
{
	tputs("\x1b[?7h");
}

//deshabilita la nueva línea automática al imprimir más allá del fin de la línea
void disablewrap(void)
{
	tputs("\x1b[?7l");
}

//obtiene el tamaño de la terminal
void getterminalsize(int *w, int *h)
{
	struct winsize windowsz;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsz);
	if(w)
		*w = windowsz.ws_col;
	if(h)
		*h = windowsz.ws_row;
}
