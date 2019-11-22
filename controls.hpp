#ifndef CONTROLS_HPP_
#define CONTROLS_HPP_

#include "types.hpp"

class TWindow
{
public:
	int x, y;            // Coordenadas relativas al contenedor
	int w, h;            // Dimensiones
	int absx, absy;      // Coordenadas absolutas
	byte color;          // Color de texto y fondo
	dword flags;         // Opciones
	TWindow *parent;     // Contenedor
	TWindow *firstchild; // Primera ventana contenida
	TWindow *next;       // Siguiente ventana en el contenedor
	// Constructor
	TWindow(void);
	// Constructor especificando contenedor
	TWindow(TContainer *parent);
	// Destructor
	~TWindow(void);
	////TODO: completar clase
private:
	
};

#endif //CONTROLS_HPP_
