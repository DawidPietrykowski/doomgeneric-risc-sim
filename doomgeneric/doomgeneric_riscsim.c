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

static uint32_t s_KeyQueue[KEYQUEUE_SIZE];
static unsigned int s_KeyQueueReadIndex = 0;

#define SCREEN_ADDR (0x1000000 - 4)
#define KEY_QUEUE_ADDR (0x1000000 - 8)
uint32_t* const screen_addr = (uint32_t*)SCREEN_ADDR;
uint32_t* const key_queue_addr = (uint32_t*)KEY_QUEUE_ADDR;
uint32_t ms_sim = 0;
struct timeval stop, start;

void DG_Init()
{
  gettimeofday(&start, NULL);
}


void DG_DrawFrame()
{
  *screen_addr = (uint32_t)DG_ScreenBuffer;
}


void DG_SleepMs(uint32_t ms)
{
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
  uint32_t keyData = s_KeyQueue[s_KeyQueueReadIndex];
  if (keyData != 0xFFFFFFFF) {
    s_KeyQueueReadIndex++;
    s_KeyQueueReadIndex %= KEYQUEUE_SIZE;

    *pressed = keyData >> 31;
    *doomKey = keyData & 0xFF;

    return 1;
  }

  s_KeyQueue[0] = 0xFFFFFFFF;
  s_KeyQueueReadIndex = 0;
  return 0;
}

void DG_SetWindowTitle(const char * title)
{

}

int main(int argc, char **argv)
{
    doomgeneric_Create(argc, argv);
    *key_queue_addr = (uint32_t)s_KeyQueue;

    for (int i = 0; ; i++)
    {
        doomgeneric_Tick();
    }

    printf("doomgeneric_Destroy\n");
    
    return 0;
}
