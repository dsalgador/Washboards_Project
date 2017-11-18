
typedef struct {
	_Bool filled; // 1 if this block if filled with sand and 0 if not 
	  //"X" or " " if its  filled or not, 
} BLOCK;


typedef struct {
	unsigned short height; // How many blocks has the pile 'f_max'
	unsigned short f; //
	BLOCK * blocks; //BLOCKs of the pile, 
} PILE;


typedef struct {
	unsigned int length; // road length 'r'	
	PILE * piles; //pointer to the columns (PILEs)
	char simbol_empty, simbol_filled;

} ROAD;

