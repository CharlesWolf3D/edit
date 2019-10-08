#ifndef VENTANA_H_
#define VENTANA_H_

extern int wndW, wndH; //dimensiones de la ventana

//mostrar título
#define SHOWTITLE_NONE       0 //no mostrar
#define SHOWTITLE_NAME       1 //nombre
#define SHOWTITLE_FULL       2 //ruta y nombre
extern char wndShowTitle;
//mostrar barra de búsqueda
#define SHOWSEARCHBAR_NONE   0 //no mostrar
#define SHOWSEARCHBAR_ACTIVE 1 //mostrar
extern char wndShowSearchBar;
//mostrar barra de pestañas
#define SHOWTABS_NONE        0 //no mostrar
#define SHOWTABS_ACTIVE      1 //mostrar
extern char wndShowTabs;
//mostrar barra de estado
#define SHOWSTATUS_NONE      0 //no mostrar
#define SHOWSTATUS_ACTIVE    1 //mostrar
extern char wndShowStatus;
//ubicación de la posición
#define POSLOCATION_NONE     0 //no mostrar
#define POSLOCATION_EDIT     1 //barra de desplazamiento del editor
#define POSLOCATION_LSTATUS  2 //barra de estado, izquierda
#define POSLOCATION_RSTATUS  3 //barra de estado, derecha
#define POSLOCATION_LTITLE   4 //barra de título, izquierda
#define POSLOCATION_RTITLE   5 //barra de título, derecha
#define POSLOCATION_MENU     6 //barra de menú
extern char wndPosLocation;

extern char *c1;         //arriba izquierda
extern char *c2;         //arriba derecha
extern char *c3;         //abajo izquierda
extern char *c4;         //abajo derecha
extern char *top;        //arriba
extern char *bottom;     //abajo
extern char *left;       //izquierda
extern char *right;      //derecha
extern char *sh;         //sombra
extern char *scrl_up;    //barra despl., flecha arriba
extern char *scrl_down;  //barra despl., flecha abajo
extern char *scrl_left;  //barra despl., flecha izquierda
extern char *scrl_right; //barra despl., flecha derecha
extern char *scrl_space; //barra despl., espacio
extern char *scrl_thumb; //barra despl., control

//inicializa la ventana del editor
void wndInit(void);

//termina la ventana del editor
void wndEnd(void);

//redibuja la ventana del programa
//las dimensiones vienen dadas por wndW y wndH
void wndRedraw(void);

#endif // VENTANA_H_
