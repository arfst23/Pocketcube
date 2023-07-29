#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pocket.h"
#include "solver.h"

//******************************************************************************

#define LENGTH 13

void scan(Cube *cube)
{
  char line[LENGTH];
  char *s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 6);
  (*cube)[top][0][0] = char2color(line[3]);
  (*cube)[top][0][1] = char2color(line[4]);
  assert(line[5] == '\n');

  s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 6);
  (*cube)[top][1][0] = char2color(line[3]);
  (*cube)[top][1][1] = char2color(line[4]);
  assert(line[5] == '\n');

  s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 12);
  (*cube)[left][0][0] = char2color(line[0]);
  (*cube)[left][0][1] = char2color(line[1]);
  (*cube)[front][0][0] = char2color(line[3]);
  (*cube)[front][0][1] = char2color(line[4]);
  (*cube)[right][0][0] = char2color(line[6]);
  (*cube)[right][0][1] = char2color(line[7]);
  (*cube)[rear][0][0] = char2color(line[9]);
  (*cube)[rear][0][1] = char2color(line[10]);
  assert(line[11] == '\n');

  s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 12);
  (*cube)[left][1][0] = char2color(line[0]);
  (*cube)[left][1][1] = char2color(line[1]);
  (*cube)[front][1][0] = char2color(line[3]);
  (*cube)[front][1][1] = char2color(line[4]);
  (*cube)[right][1][0] = char2color(line[6]);
  (*cube)[right][1][1] = char2color(line[7]);
  (*cube)[rear][1][0] = char2color(line[9]);
  (*cube)[rear][1][1] = char2color(line[10]);
  assert(line[11] == '\n');

  s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 6);
  (*cube)[bottom][0][0] = char2color(line[3]);
  (*cube)[bottom][0][1] = char2color(line[4]);
  assert(line[5] == '\n');

  s = fgets(line, LENGTH, stdin);
  assert(s);
  assert(strlen(line) == 6);
  (*cube)[bottom][1][0] = char2color(line[3]);
  (*cube)[bottom][1][1] = char2color(line[4]);
  assert(line[5] == '\n');
}

//******************************************************************************

int main(int ac, char *av[])
{
  Cube cube;

  for (;;)
  {
    scan(&cube);
    int i = hash(&cube);

    int s = solution[i];
    print(&cube);
    printf("> %s\n\n", str + s);

    if (ac > 1) // verbose
    {
      while (str[s])
      {
	printf(">> %c\n\n", str[s]);
	Turn t = char2turn(str[s]);
	turn(&cube, t);
	print(&cube);
	s++;
      }
      printf("\n");
    }

    char line[2];
    char *x = fgets(line, 2, stdin);
    if (!x)
      break;
    assert(line[0] == '\n');
  }

  return 0;
}

//******************************************************************************
