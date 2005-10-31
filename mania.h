#define TYPE_CHECKPOINT 	100

#define TYPE_LOOP 		30

#define TYPE_START_E 		10
#define TYPE_START_W		11
#define TYPE_START_N 		12
#define TYPE_START_S 		13

#define TYPE_END 		20

#define TYPE_TURBO_E		2
#define TYPE_TURBO_W		3
#define TYPE_TURBO_N		4
#define TYPE_TURBO_S		5

#define TYPE_CAR		1
#define TYPE_CAR_BODY		7
#define TYPE_BALANCIER		6

#define TURBO_POWA		0.5

typedef struct Grid
    {
    char state;
    float x;
    float y;
    float z;
    int rot;
    int flags;
    int obj;
    } Grid;

typedef struct Box
    {
    char state;
    float x;
    float y;
    float z;
    float tx;
    float ty;
    float tz;
    int type;
    int id;
    } Box;
    
#define MAX_ELEMS	4096
Grid grid[MAX_ELEMS];
Box box[MAX_ELEMS];
char data[4096];
