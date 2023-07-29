#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pocket.h"

//******************************************************************************

void init(Cube *cube)
{
  (*cube)[top][0][0] = (*cube)[top][0][1] = (*cube)[top][1][0] = (*cube)[top][1][1] = yellow;
  (*cube)[left][0][0] = (*cube)[left][0][1] = (*cube)[left][1][0] = (*cube)[left][1][1] = blue;
  (*cube)[front][0][0] = (*cube)[front][0][1] = (*cube)[front][1][0] = (*cube)[front][1][1] = red;
  (*cube)[right][0][0] = (*cube)[right][0][1] = (*cube)[right][1][0] = (*cube)[right][1][1] = green;
  (*cube)[rear][0][0] = (*cube)[rear][0][1] = (*cube)[rear][1][0] = (*cube)[rear][1][1] = purple;
  (*cube)[bottom][0][0] = (*cube)[bottom][0][1] = (*cube)[bottom][1][0] = (*cube)[bottom][1][1] = white;
}

//******************************************************************************

int main()
{
  static Cube cubes[N];
  init(&cubes[0]);

  static int queue[N];
  queue[0] = 0;
  int head = 1;
  int tail = 0;

  static int prev[N];
  memset(prev, 0xFF, N * sizeof(int));
  static Turn turns[N];
  static int len[N];
  len[0] = 0;
  static int final[N];
  memset(final, 0, N * sizeof(int));

  while (tail < head)
  {
    int src = queue[tail];
    tail++;
    fprintf(stderr, "> %d   \r", N - tail);

    int count = 0;
    for (Turn t = rightDown; t <= frontCounterClockwise; t++)
    {
      if (src && t == (turns[src] ^ 1))
	continue;

      Cube c;
      memcpy(c, cubes[src], sizeof(Cube));
      turn(&c, t);
      int dst = hash(&c);

      if (!dst || prev[dst] >= 0)
	continue;
      count++;

      memcpy(cubes[dst], c, sizeof(Cube));
      prev[dst] = src;
      turns[dst] = t;
      len[dst] = len[src] + 1;

      assert(head < N);
      queue[head] = dst;
      head++;
    }
    if (!count)
      final[src] = 1;
  }

  int count = 0;
  for (int i = 0; i < N; i++)
    if (!i || prev[i] >= 0)
      count++;
  assert(count == N);

  int max = 0;
  for (int i = 0; i < N; i++)
    if (len[i] > max)
      max = len[i];
  fprintf(stderr, "max=%d\n", max);

  int ff = 0;
  for (int l = 0; l <= max; l++)
  {
    int count = 0;
    int finals = 0;
    for (int i = 0; i < N; i++)
      if (len[i] == l)
      {
	count++;
	if (final[i])
	  finals++;
      }
    fprintf(stderr, "len=%d count=%d finals=%d\n", l, count, finals);
    ff += finals;
  }

// generate pocket-14
#if TESTDATA
  for (int i = 0; i < N; i++)
    if (len[i] == max && final[i])
      print(&cubes[i]);
#endif

#if 0
  // include "solver.h"
  for (int i = 0; i < N; i++)
  {
    char trace[20];
    char *p = trace;
    for (int j = i; j; j = prev[j])
    {
      *p = turn2char(turns[j] ^ 1);
      p++;
    }
    *p = '\0';

    int s = solution[i];
    //printf("trace=%s\n", trace);
    //printf("solut=%s\n", str + s);
    assert(!strcmp(trace, str + s));
  }
#endif

#if 1
  static int index[N];
  memset(index, 0xFF, N * sizeof(int));

  int k = 0;
  printf("const char *str =");
  for (int i = 0; i < N; i++)
    if (final[i])
    {
      char trace[20];
      char *p = trace;
      for (int j = i; j; j = prev[j])
      {
	if (index[j] < 0)
	  index[j] = p - trace + k;
	*p = turn2char(turns[j] ^ 1);
	p++;
      }
      *p = '\0';
      printf("\n  \"%s\\0\"", trace);
      k += len[i] + 1;
    }
  printf(";\n\n");
  index[0] = k;

  printf("int solution[%d] =\n", N);
  printf("{\n");
  for (int i = 0; i < N; i++)
    printf("  %d,\n", index[i]);
  printf("};\n");
#endif

  return 0;
}

//******************************************************************************
