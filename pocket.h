//******************************************************************************

// 7! * 3^6
#define N 3674160

typedef enum
{
  yellow,
  red,
  green,
  white,
  blue,
  purple,
} Color;

typedef enum
{
  top,
  left,
  front,
  right,
  rear,
  bottom
} Face;

typedef enum
{
  topRightFront,
  topRightRear,
  topLeftRear,
  topLeftFront,
  bottomLeftFront,
  bottomRightFront,
  bottomRightRear,
  bottomLeftRear,
} Corner;

typedef enum
{
  rightDown,
  rightUp,
  topLeft,
  topRight,
  frontClockwise,
  frontCounterClockwise
} Turn;

typedef char Cube[6][2][2]; // face row col

//******************************************************************************

Color char2color(char c);
char color2char(Color c);

Turn char2turn(char c);
char turn2char(Turn t);

void norm(Cube *cube);
int hash(const Cube *cube);

void turn(Cube *cube, Turn t);
void print(Cube *cube);

//******************************************************************************
