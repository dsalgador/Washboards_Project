
typedef struct {
	_Bool filled; // 1 if this block if filled with sand and 0 if not 
	  //"X" or " " if its  filled or not, 
} BLOCK;


typedef struct {
	unsigned short height; // How many blocks has the pile 'f_max', 0 to 65,535
	int f; // -32,768 to 32,767 or -2,147,483,648 to 2,147,483,647
	BLOCK * blocks; //BLOCKs of the pile, 
} PILE;


typedef struct {
	unsigned int length; // road length 'r'	
	PILE * piles; //pointer to the columns (PILEs)
	char simbol_empty, simbol_filled;
	unsigned short n; //vehicle passes, 0 to 65,535
	_Bool occuped; //if the road is occuped by a vehicle or not
} ROAD;

typedef struct {
	unsigned int diameter; // road length 'r'	
	int x0, xf; //-32,768 to 32,767 or -2,147,483,648 to 2,147,483,647
	unsigned short elevation; //current road height where the wheel is situated (elevation), 0 to 65,535
	_Bool in_road;
	int jumps;	
} WHEEL;

