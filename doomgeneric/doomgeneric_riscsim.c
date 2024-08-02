//doomgeneric emscripten port

#include "doomkeys.h"
#include "m_argv.h"
#include "doomgeneric.h"

#include <stdio.h>
#include <unistd.h>

#include <stdbool.h>
#include <stddef.h>
#include <errno.h>
#include <sys/time.h>

#define KEYQUEUE_SIZE 16

static unsigned short s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueWriteIndex = 0;
static unsigned int s_KeyQueueReadIndex = 0;

static unsigned char convertToDoomKey(unsigned int key)
{
  // return key;
  return 0;
}

static void addKeyToQueue(int pressed, unsigned int keyCode)
{
  unsigned char key = convertToDoomKey(keyCode);

  unsigned short keyData = (pressed << 8) | key;

  s_KeyQueue[s_KeyQueueWriteIndex] = keyData;
  s_KeyQueueWriteIndex++;
  s_KeyQueueWriteIndex %= KEYQUEUE_SIZE;
}

#define SCREEN_ADDR 0x40000000
struct timeval stop, start;

void DG_Init()
{
  gettimeofday(&start, NULL);
  printf("dg init\n");
}


void DG_DrawFrame()
{
  // printf("draw frame\n");
  memcpy(SCREEN_ADDR, DG_ScreenBuffer, DOOMGENERIC_RESX*DOOMGENERIC_RESY*sizeof(uint32_t));
}

uint32_t ms_sim = 0;

void DG_SleepMs(uint32_t ms)
{
}
uint32_t DG_GetTicksMs()
{
    gettimeofday(&stop, NULL);
    uint32_t s = (stop.tv_sec - start.tv_sec);
    uint32_t us = (stop.tv_usec - start.tv_usec);

    if (stop.tv_usec < start.tv_usec) {
      us = 1000000 + stop.tv_usec - start.tv_usec;
    }
    uint32_t ms = us / 1000;

  // printf("ticktime %lu s %lu ms\n", s, ms);
  return (s * 1000 + ms);
}

int DG_GetKey(int* pressed, unsigned char* doomKey)
{
  // printf("get key\n");
  // handleKeyInput();
  if (s_KeyQueueReadIndex == s_KeyQueueWriteIndex)
  {
    //key queue is empty
    return 0;
  }
  else
  {
    unsigned short keyData = s_KeyQueue[s_KeyQueueReadIndex];
    s_KeyQueueReadIndex++;
    s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

    *pressed = keyData >> 8;
    *doomKey = keyData & 0xFF;

    return 1;
  }

  return 0;
}

void DG_SetWindowTitle(const char * title)
{
  printf("DG_SetWindowTitle: %s\n", title);
}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
    }

    printf("doomgeneric_Destroy\n");
    
    return 0;
}