#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>

#include "pocket.h"

int main(int ac, char *av[])
{
  assert(ac == 2);
  int n = atoi(av[1]);
  assert(n);

  {
    unsigned int pid = getpid();
    unsigned int tim = time (NULL);
    srand(tim ^ pid << 8);
  }

  for (int i = 0; i < n; i++)
  {
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

    int perm = rand() % 24;
    Cube cube;
    cube[top][0][0] = cube[top][0][1] = cube[top][1][0] = cube[top][1][1] = perms[perm][0];
    cube[front][0][0] = cube[front][0][1] = cube[front][1][0] = cube[front][1][1] = perms[perm][1];
    cube[right][0][0] = cube[right][0][1] = cube[right][1][0] = cube[right][1][1] = perms[perm][2];
    cube[bottom][0][0] = cube[bottom][0][1] = cube[bottom][1][0] = cube[bottom][1][1] = perms[perm][3];
    cube[left][0][0] = cube[left][0][1] = cube[left][1][0] = cube[left][1][1] = perms[perm][4];
    cube[rear][0][0] = cube[rear][0][1] = cube[rear][1][0] = cube[rear][1][1] = perms[perm][5];

    for (int k = 0; k < 10000; k++)
    {
      Turn t = rand() % 6;
      turn(&cube, t);
    }

    print(&cube);
  }

  return 0;
}
