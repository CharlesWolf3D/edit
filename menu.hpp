#ifndef MENU_HPP_
#define MENU_HPP_

//opciones para elementos de menú
#define MNFL_NORM 0x00000000 //sin opciones
#define MNFL_DIS  0x00000001 //deshabilitar
#define MNFL_INV  0x00000002 //no visible
#define MNFL_CHK  0x00000004 //con icono de verificación
#define MNFL_OPT  0x00000008 //con icono de opción
#define MNFL_NOHK 0x00000010 //no responder a la tecla de acceso rápido

//elemento de definición de menú
typedef struct
{
	const char *caption;                 //el & indica el carácter enfatizado, cada tab al principio es un nivel de anidación
	unsigned int hk;               //tecla de acceso rápido
	unsigned int id;               //identificador para modificar el elemento
	unsigned int flags;            //opciones
	void (*func)(unsigned int id); //callback
} menudef_t;

#endif //MENU_HPP_
