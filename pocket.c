#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "pocket.h"

//******************************************************************************

Color char2color(char c)
{
  switch (c)
  {
  case 'Y': return yellow;
  case 'B': return blue;
  case 'R': return red;
  case 'G': return green;
  case 'P': return purple;
  case 'W': return white;
  }
  abort();
}

char color2char(Color c)
{
  static char colors[] = "YRGWBP";
  return colors[c];
}

Turn char2turn(char c)
{
  switch (c)
  {
  case 'D': return rightDown;
  case 'U': return rightUp;
  case 'L': return topLeft;
  case 'R': return topRight;
  case 'C': return frontClockwise;
  case 'X': return frontCounterClockwise;
  }
  abort();
}

char turn2char(Turn t)
{
  static char turns[] = "DULRCX";
  return turns[t];
}

//******************************************************************************

static int perms[24][6] =
{
  { yellow, red, green, white, blue, purple },
  { red, green, yellow, purple, white, blue },
  { green, yellow, red, blue, purple, white },

  { yellow, green, purple, white, red, blue },
  { green, purple, yellow, blue, white, red },
  { purple, yellow, green, red, blue, white },

  { yellow, purple, blue, white, green, red },
  { purple, blue, yellow, red, white, green },
  { blue, yellow, purple, green, red, white },

  { yellow, blue, red, white, purple, green },
  { blue, red, yellow, green, white, purple },
  { red, yellow, blue, purple, green, white },

  { white, red, blue, yellow, green, purple },
  { red, blue, white, purple, yellow, green },
  { blue, white, red, green, purple, yellow },

  { white, green, red, yellow, purple, blue },
  { green, red, white, blue, yellow, purple },
  { red, white, green, purple, blue, yellow },

  { white, purple, green, yellow, blue, red },
  { purple, green, white, red, yellow, blue },
  { green, white, purple, blue, red, yellow },

  { white, blue, purple, yellow, red, green },
  { blue, purple, white, green, yellow, red },
  { purple, white, blue, red, green, yellow },
};

void norm(Cube *cube)
{
  int perm;
  for (perm = 0; perm < 24; perm++)
    if (perms[perm][(*cube)[bottom][1][0]] == white
      && perms[perm][(*cube)[left][1][0]] == blue
      && perms[perm][(*cube)[rear][1][1]] == purple)
        break;
  assert(perm < 24);

  for (int face = top; face <= bottom; face++)
    for (int row = 0; row <= 1; row++)
      for (int col = 0; col <= 1; col++)
	(*cube)[face][row][col] = perms[perm][(*cube)[face][row][col]];
}

int hash(const Cube *cube)
{
  int corners[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };
  int rotations[8] = { -1, -1, -1, -1, -1, -1, -1, -1 };

  int perm;
  for (perm = 0; perm < 24; perm++)
  {
    if ((*cube)[top][1][1] == perms[perm][0]
      && (*cube)[front][0][1] == perms[perm][1]
      && (*cube)[right][0][0] == perms[perm][2])
    {
      corners[topRightFront] = perm / 3;
      rotations[topRightFront] = perm % 3;
    }

    if ((*cube)[top][0][1] == perms[perm][0]
      && (*cube)[right][0][1] == perms[perm][1]
      && (*cube)[rear][0][0] == perms[perm][2])
    {
      corners[topRightRear] = perm / 3;
      rotations[topRightRear] = perm % 3;
    }

    if ((*cube)[top][0][0] == perms[perm][0]
      && (*cube)[rear][0][1] == perms[perm][1]
      && (*cube)[left][0][0] == perms[perm][2])
    {
      corners[topLeftRear] = perm / 3;
      rotations[topLeftRear] = perm % 3;
    }

    if ((*cube)[top][1][0] == perms[perm][0]
      && (*cube)[left][0][1] == perms[perm][1]
      && (*cube)[front][0][0] == perms[perm][2])
    {
      corners[topLeftFront] = perm / 3;
      rotations[topLeftFront] = perm % 3;
    }

    if ((*cube)[bottom][0][0] == perms[perm][0]
      && (*cube)[front][1][0] == perms[perm][1]
      && (*cube)[left][1][1] == perms[perm][2])
    {
      corners[bottomLeftFront] = perm / 3;
      rotations[bottomLeftFront] = perm % 3;
    }

    if ((*cube)[bottom][0][1] == perms[perm][0]
      && (*cube)[right][1][0] == perms[perm][1]
      && (*cube)[front][1][1] == perms[perm][2])
    {
      corners[bottomRightFront] = perm / 3;
      rotations[bottomRightFront] = perm % 3;
    }

    if ((*cube)[bottom][1][1] == perms[perm][0]
      && (*cube)[rear][1][0] == perms[perm][1]
      && (*cube)[right][1][1] == perms[perm][2])
    {
      corners[bottomRightRear] = perm / 3;
      rotations[bottomRightRear] = perm % 3;
    }

    if ((*cube)[bottom][1][0] == perms[perm][0]
      && (*cube)[left][1][0] == perms[perm][1]
      && (*cube)[rear][1][1] == perms[perm][2])
    {
      corners[bottomLeftRear] = perm / 3;
      rotations[bottomLeftRear] = perm % 3;
    }
  }
  assert(corners[bottomLeftRear] == bottomLeftRear);
  assert(rotations[bottomLeftRear] == 0);

  int rot = rotations[0] + rotations[1] + rotations[2]
    + rotations[3] + rotations[4] + rotations[5] + rotations[6];
  assert(rot % 3 == 0);

  int reduced[7];
  for (Corner c = topRightFront; c < bottomLeftRear; c++)
  {
    reduced[c] = corners[c];
    for (Corner cc = topRightFront; cc < c; cc++)
      if (corners[cc] < corners[c])
	reduced[c]--;
  }
  assert(reduced[bottomRightRear] == 0);

  int hash =
    ((((((((((reduced[0]
	      * 6 + reduced[1])
	     * 5 + reduced[2])
	    * 4 + reduced[3])
	   * 3 + reduced[4])
	  * 2 + reduced[5])
	 * 3 + rotations[0])
	* 3 + rotations[1])
       * 3 + rotations[2])
      * 3 + rotations[3])
     * 3 + rotations[4])
    * 3 + rotations[5];
  assert(hash >= 0);
  assert(hash < N);

  return hash;
}

//******************************************************************************

void turn(Cube *cube, Turn t)
{
  Color c;
  switch (t)
  {
  case topLeft:
    c = (*cube)[top][0][0];
    (*cube)[top][0][0] = (*cube)[top][1][0];
    (*cube)[top][1][0] = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[top][0][1];
    (*cube)[top][0][1] = c;

    c = (*cube)[left][0][0];
    (*cube)[left][0][0] = (*cube)[front][0][0];
    (*cube)[front][0][0] = (*cube)[right][0][0];
    (*cube)[right][0][0] = (*cube)[rear][0][0];
    (*cube)[rear][0][0] = c;

    c = (*cube)[left][0][1];
    (*cube)[left][0][1] = (*cube)[front][0][1];
    (*cube)[front][0][1] = (*cube)[right][0][1];
    (*cube)[right][0][1] = (*cube)[rear][0][1];
    (*cube)[rear][0][1] = c;
    break;

  case topRight:
    c = (*cube)[top][0][0];
    (*cube)[top][0][0] = (*cube)[top][0][1];
    (*cube)[top][0][1] = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[top][1][0];
    (*cube)[top][1][0] = c;

    c = (*cube)[left][0][0];
    (*cube)[left][0][0] = (*cube)[rear][0][0];
    (*cube)[rear][0][0] = (*cube)[right][0][0];
    (*cube)[right][0][0] = (*cube)[front][0][0];
    (*cube)[front][0][0] = c;

    c = (*cube)[left][0][1];
    (*cube)[left][0][1] = (*cube)[rear][0][1];
    (*cube)[rear][0][1] = (*cube)[right][0][1];
    (*cube)[right][0][1] = (*cube)[front][0][1];
    (*cube)[front][0][1] = c;
    break;

  case rightUp:
    c = (*cube)[right][0][0];
    (*cube)[right][0][0] = (*cube)[right][1][0];
    (*cube)[right][1][0] = (*cube)[right][1][1];
    (*cube)[right][1][1] = (*cube)[right][0][1];
    (*cube)[right][0][1] = c;

    c = (*cube)[top][0][1];
    (*cube)[top][0][1] = (*cube)[front][0][1];
    (*cube)[front][0][1] = (*cube)[bottom][0][1];
    (*cube)[bottom][0][1] = (*cube)[rear][1][0];
    (*cube)[rear][1][0] = c;

    c = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[front][1][1];
    (*cube)[front][1][1] = (*cube)[bottom][1][1];
    (*cube)[bottom][1][1] = (*cube)[rear][0][0];
    (*cube)[rear][0][0] = c;
    break;

  case rightDown:
    c = (*cube)[right][0][0];
    (*cube)[right][0][0] = (*cube)[right][0][1];
    (*cube)[right][0][1] = (*cube)[right][1][1];
    (*cube)[right][1][1] = (*cube)[right][1][0];
    (*cube)[right][1][0] = c;

    c = (*cube)[top][0][1];
    (*cube)[top][0][1] = (*cube)[rear][1][0];
    (*cube)[rear][1][0] = (*cube)[bottom][0][1];
    (*cube)[bottom][0][1] = (*cube)[front][0][1];
    (*cube)[front][0][1] = c;

    c = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[rear][0][0];
    (*cube)[rear][0][0] = (*cube)[bottom][1][1];
    (*cube)[bottom][1][1] = (*cube)[front][1][1];
    (*cube)[front][1][1] = c;
    break;

  case frontClockwise:
    c = (*cube)[front][0][0];
    (*cube)[front][0][0] = (*cube)[front][1][0];
    (*cube)[front][1][0] = (*cube)[front][1][1];
    (*cube)[front][1][1] = (*cube)[front][0][1];
    (*cube)[front][0][1] = c;

    c = (*cube)[top][1][0];
    (*cube)[top][1][0] = (*cube)[left][1][1];
    (*cube)[left][1][1] = (*cube)[bottom][0][1];
    (*cube)[bottom][0][1] = (*cube)[right][0][0];
    (*cube)[right][0][0] = c;

    c = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[left][0][1];
    (*cube)[left][0][1] = (*cube)[bottom][0][0];
    (*cube)[bottom][0][0] = (*cube)[right][1][0];
    (*cube)[right][1][0] = c;
    break;

  case frontCounterClockwise:
    c = (*cube)[front][0][0];
    (*cube)[front][0][0] = (*cube)[front][0][1];
    (*cube)[front][0][1] = (*cube)[front][1][1];
    (*cube)[front][1][1] = (*cube)[front][1][0];
    (*cube)[front][1][0] = c;

    c = (*cube)[top][1][0];
    (*cube)[top][1][0] = (*cube)[right][0][0];
    (*cube)[right][0][0] = (*cube)[bottom][0][1];
    (*cube)[bottom][0][1] = (*cube)[left][1][1];
    (*cube)[left][1][1] = c;

    c = (*cube)[top][1][1];
    (*cube)[top][1][1] = (*cube)[right][1][0];
    (*cube)[right][1][0] = (*cube)[bottom][0][0];
    (*cube)[bottom][0][0] = (*cube)[left][0][1];
    (*cube)[left][0][1] = c;
    break;
  }
}

void print(Cube *cube)
{
  printf("   %c%c\n", color2char((*cube)[top][0][0]), color2char((*cube)[top][0][1]));
  printf("   %c%c\n", color2char((*cube)[top][1][0]), color2char((*cube)[top][1][1]));
  printf("%c%c %c%c %c%c %c%c\n", color2char((*cube)[left][0][0]), color2char((*cube)[left][0][1]),
    color2char((*cube)[front][0][0]), color2char((*cube)[front][0][1]), color2char((*cube)[right][0][0]),
    color2char((*cube)[right][0][1]), color2char((*cube)[rear][0][0]), color2char((*cube)[rear][0][1]));
  printf("%c%c %c%c %c%c %c%c\n", color2char((*cube)[left][1][0]), color2char((*cube)[left][1][1]),
    color2char((*cube)[front][1][0]), color2char((*cube)[front][1][1]), color2char((*cube)[right][1][0]),
    color2char((*cube)[right][1][1]), color2char((*cube)[rear][1][0]), color2char( (*cube)[rear][1][1]));
  printf("   %c%c\n", color2char((*cube)[bottom][0][0]), color2char((*cube)[bottom][0][1]));
  printf("   %c%c\n", color2char((*cube)[bottom][1][0]), color2char((*cube)[bottom][1][1]));
  printf("\n");
}

//******************************************************************************
