#include <sample_gen.h>

int main(void)
{
  MEM_Arena arena = MEM_ArenaInit(0);
  WAV_Buffer buffer = WAV_BufferInit(&arena, 44100);
  WAV_BufferAppendSineWave(&buffer, 1, 50, 10);
  WAV_BufferSaveToFile(&buffer, "sine50.wav");
  WAV_BufferFree(&buffer);
  buffer = WAV_BufferInit(&arena, 44100);
  WAV_BufferAppendBrownNoise(&buffer, 0.5, 10);
  WAV_BufferSaveToFile(&buffer, "brown.wav");
  WAV_BufferFree(&buffer);
  MEM_ArenaFree(&arena);
}
