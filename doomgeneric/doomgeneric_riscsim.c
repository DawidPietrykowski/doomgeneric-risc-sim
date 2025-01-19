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
#include "i_video.h"

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

#define SCREEN_ADDR (0x1000000 - 4)
uint32_t* const screen_addr = (uint32_t*)SCREEN_ADDR;
struct timeval stop, start;

void DG_Init()
{
  gettimeofday(&start, NULL);
  printf("dg init\n");
}


void DG_DrawFrame()
{
  *screen_addr = (uint32_t)DG_ScreenBuffer;
  // printf("Set addr: %x\n", *(uint32_t*)SCREEN_ADDR);
  // uint32_t size = DOOMGENERIC_RESX*DOOMGENERIC_RESY*sizeof(uint32_t);
  // memcpy(SCREEN_ADDR, DG_ScreenBuffer, size);
  // memset(SCREEN_ADDR + size, 1, 1);
}

uint32_t ms_sim = 0;

void DG_SleepMs(uint32_t ms)
{
  // memset(0x12345678, 0, 100); // temp delay
  uint32_t ticks = DG_GetTicksMs();
  while (DG_GetTicksMs() - ticks < ms) {};
}
uint32_t DG_GetTicksMs()
{
    gettimeofday(&stop, NULL);
    uint32_t s = (stop.tv_sec - start.tv_sec);
    uint32_t us = (stop.tv_usec - start.tv_usec);

    if (stop.tv_usec < start.tv_usec) {
      s = s - 1;
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
