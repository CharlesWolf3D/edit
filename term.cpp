#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>

#include "types.hpp"
#include "term.hpp"

// Paleta estándar
static byte pal16[48] =
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

// Paleta de Linux
static byte pal16_linux[48] =
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

// Convierte un número entero en una cadena en base hexadecimal, utilizando siempre 2 dígitos.
static char hexdigits [16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
static void int2hex2(byte x, char *str)
{
	str[0] = hexdigits[(x >> 4) & 0xf];
	str[1] = hexdigits[(x) & 0xf];
	str[2] = 0;
}

// Convierte un número entero en una cadena en base decimal.
static void int2str(int x, char *str)
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

// Números de color que enviar mediante comandos ANSI.
static unsigned char subst[16] =  { 0,  4,  2,  6,  1,  5,  3,  7,  8, 12, 10, 14,  9, 13, 11, 15};

// Inicia el sistema de terminal.
void TAnsiTerminal::Start(void)
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
	ttystate.c_iflag &= ~ICRNL;  //no transformar 13 en 10 para entrada
	ttystate.c_oflag &= ~OPOST;  //no transformar "\n" en "\n\r" para salida
	ttystate.c_cc[VMIN] = 1;     //recibir caracteres inmediatamente
	tcsetattr(STDIN_FILENO, TCSANOW, &ttystate); //enviar atributos de stdin
	//inicializar variables
	termBufferCount = 0;
	textAttr = ATTR_NONE;
	textFG = textBG = -1;
	//enviar comandos a la terminal
	Print
	(
	"\x1b[?7l"         //disablewrap
	"\x1b[0m"          //setattr(ATTR_NONE)
	"\x1b[39m\x1b[49m" //resetcolor
	"\x1b[?25l"        //hidecursor
	"\x1b%G"           //codificación en UTF-8
	"\x1b F"           //códigos C1 de 7 bits
	"\x1b[?1003h"      //habilitar eventos de ratón (todos)
	"\x1b[?1007h"      //habilitar rueda del ratón
	);
	Refresh();
}

// Cierra el sistema de terminal.
void TAnsiTerminal::End(void)
{
	SetPal(pal16_linux);
	//enviar comandos a la terminal
	Print
	(
	"\x1b[?1007l"      //deshabilitar rueda del ratón
	"\x1b[?1003l"      //deshabilitar eventos de ratón (todos)
	"\x1b[?7h"         //enablewrap
	"\x1b[0m"          //setattr(ATTR_NONE)
	"\x1b[39m\x1b[49m" //resetcolor
	"\x1b[?25h"        //showcursor
	);
	Refresh();
	//restaurar atributos de stdin salvados
	tcsetattr(STDIN_FILENO, TCSANOW, &savedTtyState);
}

// Escribe una cadena (texto o comandos) en la terminal.
// La almacena en un búfer y sólo escribe en el dispositivo cuando éste se llena o se llama a Refresh().
void TAnsiTerminal::Print(const char *str)
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

// Escribe en la terminal la parte del búfer que todavía no ha sido escrita.
void TAnsiTerminal::Refresh(void)
{
	if(termBufferCount)
	{
		write(STDOUT_FILENO, termBuffer, termBufferCount);
		termBufferCount = 0;
	}
}

// Borra la pantalla y pone el cursor en 0,0.
void TAnsiTerminal::Clear(void)
{
	Print("\x1b[2J\x1b[1;1H");
}

// Pone el cursor en x,y.
void TAnsiTerminal::GotoXY(int32 x, int32 y)
{
	char num[8];
	Print("\x1b[");
	int2str(y + 1, num);
	Print(num);
	Print(";");
	int2str(x + 1, num);
	Print(num);
	Print("H");
}

// Obtiene el tamaño de la terminal
void TAnsiTerminal::GetSize(int32 *w, int32 *h)
{
	struct winsize windowsz;
	ioctl(STDOUT_FILENO, TIOCGWINSZ, &windowsz);
	if(w)
		*w = windowsz.ws_col;
	if(h)
		*h = windowsz.ws_row;
}

// Muestra u oculta el cursor de texto.
void TAnsiTerminal::ShowCursor(byte show)
{
	Print(show ? "\x1b[?25h" : "\x1b[?25l");
}

// Habilita o deshabilita la nueva línea automática al imprimir más allá del fin de la línea.
void TAnsiTerminal::EnableWrap(byte enable)
{
	Print(enable ? "\x1b[?7h" : "\x1b[?7l");
}

// Establece los atributos de los caracteres que se van a escribir.
void TAnsiTerminal::SetAttr(byte attr)
{
	byte change;
	change = attr ^ textAttr;
	if(!change)
		return;
	if(change & ATTR_BOLD)
		Print(attr & ATTR_BOLD ?      "\x1b[1m" : "\x1b[22m");
	if(change & ATTR_ITALIC)
		Print(attr & ATTR_ITALIC ?    "\x1b[3m" : "\x1b[23m");
	if(change & ATTR_UNDERLINE)
		Print(attr & ATTR_UNDERLINE ? "\x1b[4m" : "\x1b[24m");
	if(change & ATTR_STRIKE)
		Print(attr & ATTR_STRIKE ?    "\x1b[9m" : "\x1b[29m");
	textAttr = attr;
}

// Establece el color de primer plano.
// El valor -1 establece el color predeterminado de la terminal.
void TAnsiTerminal::SetFgColor(int8 clr)
{
	char num[8];
	if(clr == textFG)
		return;
	if(clr == -1)
	{
		textFG = -1;
		Print("\x1b[39m");
		return;
	}
	textFG = clr & 0xf;
	Print("\x1b[38;5;");
	int2str(subst[textFG], num);
	Print(num);
	Print("m");
}

// Establece el color de fondo.
// El valor -1 establece el color predeterminado de la terminal.
void TAnsiTerminal::SetBgColor(int8 clr)
{
	char num[8];
	if(clr == textBG)
		return;
	if(clr == -1)
	{
		textBG = -1;
		Print("\x1b[49m");
		return;
	}
	textBG = clr & 0xf;
	Print("\x1b[48;5;");
	int2str(subst[textBG], num);
	Print(num);
	Print("m");
}

// Establece los 16 colores de la paleta.
void TAnsiTerminal::SetPal(byte *pal)
{
	char num[8];
	byte idx;
	Print("\x1b]4");
	for(int i = 0; i < 16; i++)
	{
		idx = subst[i] * 3;
		Print(";");
		int2str(i, num); Print(num);
		Print(";rgb:");
		int2hex2(pal[idx],     num); Print(num); Print("/");
		int2hex2(pal[idx + 1], num); Print(num); Print("/");
		int2hex2(pal[idx + 2], num); Print(num);
	}
	Print("\x1b\\");
}

// Devuelve si hay datos disponibles en stdin.
byte TAnsiTerminal::KbHit(void)
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

// Lee un carácter de stdin (0..255).
// Devuelve -1 si hubo un error al leer.
int TAnsiTerminal::GetChr(void)
{
	byte ch;
	if(read(STDIN_FILENO, &ch, 1) != 1)
		return(-1);
	return(ch);
}

// Devuelve una tecla pulsada con sus modificadores.
dword TAnsiTerminal::GetKey(void)
{
	int ch;
	unsigned int mod = HK_NONE, num = 0, num2 = 0, num3;
	int use_num = 0, use_num2 = 0;
	ch = GetChr();
	if(ch == 0x09)return(HK_TK | HK_TAB);                              //Tab (o Ctrl[+Mayús]+I)
	if(ch == 0x0a)return(HK_TK | HK_C | HK_ENTER);                     //Ctrl+Intro (o Ctrl[+Mayús]+J)
	if(ch == 0x0d)return(HK_TK | HK_ENTER);                            //Intro (o Ctrl[+Mayús]+M)
	if(ch >= 0x01 && ch <= 0x1a)return(HK_TC | HK_C | (ch + 'A' - 1)); //Ctrl[+Mayús]+Letra, excluyendo I, J, y M
	if(ch == 0x1b)
	{
		if(!KbHit())return(HK_TK | HK_ESC); //Esc (o Ctrl+3)
		ch = GetChr();
		if(ch == 0x1b)return(HK_TK | HK_ESC);                               //Esc
		if(ch == 0x0a)return(HK_TK | HK_A | HK_ENTER);                      //[Ctrl+]Alt+[Mayús]Intro (o Ctrl+Alt[+Mayús]+J)
		if(ch >= 0x01 && ch <= 0x1a)return(HK_TC | HK_CA | (ch + 'A' - 1)); //Ctrl+Alt[+Mayús]+Letra, excluyendo J
		if(ch == 0x1f)return(HK_TC | HK_CAS | '-');                         //Ctrl+Alt+Mayús+-
		if(ch == '[')
		{
			if(!KbHit())return(HK_TC | HK_A | '['); //Alt+[
			ch = GetChr();
			if(ch == 'M')
			{
				if(!KbHit())return(0);
				num = GetChr();
				if(!KbHit())return(0);
				num2 = GetChr();
				if(!KbHit())return(0);
				num3 = GetChr();
				num2 = (unsigned char)(num2 - 33);
				num3 = (unsigned char)(num3 - 33);
				if(num & 0x04)mod |= HK_S;
				if(num & 0x08)mod |= HK_A;
				if(num & 0x10)mod |= HK_C;
				int btns = 0;
				if((num & 0x60) == 0x60)
				{
					if(num & 0x01)btns |= HK_WHDN;
					else btns |= HK_WHUP;
				}
				else
				{
					switch(num & 0x03)
					{
						case 0: btns |= HK_M1; break;
						case 1: btns |= HK_M3; break;
						case 2: btns |= HK_M2; break;
					}
				}
				return(HK_TM | mod | (num3 << 8) | (num2 << 16) | btns); //Ratón
			}
			if(ch >= '0' && ch <= '9')
			{
				use_num = 1;
				num = 0;
				while(1)
				{
					num *= 10;
					num += ch - '0';
					if(!KbHit())return(0);
					ch = GetChr();
					if(ch < '0' || ch > '9')break;
				}
			}
			if(ch == ';')
			{
				if(!KbHit())return(0);
				ch = GetChr();
				if(ch >= '0' && ch <= '9')
				{
					use_num2 = 1;
					num2 = 0;
					while(1)
					{
						num2 *= 10;
						num2 += ch - '0';
						if(!KbHit())return(0);
						ch = GetChr();
						if(ch < '0' || ch > '9')break;
					}
				}
			}
			if(ch == '~')
			{
				if(!use_num)return(0);
				if(use_num2)
				{
					if(num2 < 1)return(0);
					num2--;
					if(num2 & 0x01)mod |= HK_S; //mod Mayús
					if(num2 & 0x02)mod |= HK_A; //mod Alt
					if(num2 & 0x04)mod |= HK_C; //mod Ctrl
				}
				switch(num)
				{
				case 1:  case 7: return(HK_TK | mod | HK_HOME); //mod+Inicio
				case 2:  case 8: return(HK_TK | mod | HK_INS);  //mod+Ins
				case 3:          return(HK_TK | mod | HK_DEL);  //mod+Supr
				case 4:          return(HK_TK | mod | HK_END);  //mod+Fin
				case 5:          return(HK_TK | mod | HK_PGUP); //mod+RePág
				case 6:          return(HK_TK | mod | HK_PGDN); //mod+AvPág
				case 11:         return(HK_TK | mod | HK_F1);   //mod+F1
				case 12:         return(HK_TK | mod | HK_F2);   //mod+F2
				case 13:         return(HK_TK | mod | HK_F3);   //mod+F3
				case 14:         return(HK_TK | mod | HK_F4);   //mod+F4
				case 15:         return(HK_TK | mod | HK_F5);   //mod+F5
				case 17:         return(HK_TK | mod | HK_F6);   //mod+F6
				case 18:         return(HK_TK | mod | HK_F7);   //mod+F7
				case 19:         return(HK_TK | mod | HK_F8);   //mod+F8
				case 20:         return(HK_TK | mod | HK_F9);   //mod+F9
				case 21:         return(HK_TK | mod | HK_F10);  //mod+F10
				case 23:         return(HK_TK | mod | HK_F11);  //mod+F11
				case 24:         return(HK_TK | mod | HK_F12);  //mod+F12
				}
				return(0);
			}
			if(use_num)
			{
				if(use_num2)
				{
					if(num != 1)return(0);
					if(num2 < 1)return(0);
					num2--;
					if(num2 & 0x01)mod |= HK_S; //mod Mayús
					if(num2 & 0x02)mod |= HK_A; //mod Alt
					if(num2 & 0x04)mod |= HK_C; //mod Ctrl
				}
				else
				{
					if(num < 1)return(0);
					num--;
					if(num & 0x01)mod |= HK_S; //mod Mayús
					if(num & 0x02)mod |= HK_A; //mod Alt
					if(num & 0x04)mod |= HK_C; //mod Ctrl
				}
			}
			switch(ch)
			{
			case 'A': return(HK_TK | mod | HK_UP);    //mod+Arriba
			case 'B': return(HK_TK | mod | HK_DOWN);  //mod+Abajo
			case 'C': return(HK_TK | mod | HK_RIGHT); //mod+Derecha
			case 'D': return(HK_TK | mod | HK_LEFT);  //mod+Izquierda
			case 'F': return(HK_TK | mod | HK_END);   //mod+Fin
			case 'H': return(HK_TK | mod | HK_HOME);  //mod+Inicio
			case 'Z': return(HK_TK | HK_S | HK_TAB);  //Mayús+Tab
			}
			return(0);
		}
		if(ch == 'O')
		{
			if(KbHit())
			{
				ch = GetChr();
				if(ch == '1')
				{
					if(!KbHit())return(0);
					if(GetChr() != ';')return(0);
					if(!KbHit())return(0);
					ch = GetChr();
					if(ch < '1')return(0);
					ch = ch - '0' - 1;
					if(ch & 0x01)mod |= HK_S; //mod Mayús
					if(ch & 0x02)mod |= HK_A; //mod Alt
					if(ch & 0x04)mod |= HK_C; //mod Ctrl
					if(!KbHit())return(0);
					ch = GetChr();
					switch(ch)
					{
					case 'F': return(HK_TK | mod | HK_END);  //mod+Fin
					case 'H': return(HK_TK | mod | HK_HOME); //mod+Inicio
					case 'P': return(HK_TK | mod | HK_F1);   //mod+F1
					case 'Q': return(HK_TK | mod | HK_F2);   //mod+F2
					case 'R': return(HK_TK | mod | HK_F3);   //mod+F3
					case 'S': return(HK_TK | mod | HK_F4);   //mod+F4
					default: return(0);
					}
				}
				if(ch == 'F')return(HK_TK | HK_END);  //Fin
				if(ch == 'H')return(HK_TK | HK_HOME); //Inicio
				if(ch == 'P')return(HK_TK | HK_F1);   //F1
				if(ch == 'Q')return(HK_TK | HK_F2);   //F2
				if(ch == 'R')return(HK_TK | HK_F3);   //F3
				if(ch == 'S')return(HK_TK | HK_F4);   //F4
				return(0);
			}
		}
		if(ch >= '0' && ch <= '9')return(HK_TC | HK_A | ch);                 //Alt+Número
		if(ch >= 'A' && ch <= 'Z')return(HK_TC | HK_A | ch);                 //Alt[+Mayús]+Letra
		if(ch >= 'a' && ch <= 'z')return(HK_TC | HK_A | (ch - ('a' - 'A'))); //Alt[+Mayús]+Letra
		if(ch == 0x7f)return(HK_TK | HK_A | HK_BKSP);                        //[Ctrl+]Alt+[Mayús+]Retroceso
		if(ch == 31)return(HK_TC | HK_CAS | '-');                            //Ctrl+Alt+Mayús+-
		return(HK_TC | HK_A | ch);                                           //Alt+Carácter
	}
	if(ch == 0x1f)return(HK_TC | HK_CA | '-'); //Ctrl+Alt+-
	if(ch == 0x7f)return(HK_TK | HK_BKSP);     //[Ctrl+][Mayús+]Retroceso
	if(ch <= 0 || ch >= 255)return(0);
	return(HK_TC | ch);                        //Carácter
}
