#ifndef SAMPLE_GEN_H
#define SAMPLE_GEN_H

#include <base_layer.h>

c_linkage_begin

typedef S16 WAV_Sample;

#define WAV_SAMPLE_AMPLITUDE  MAX_S16
#define WAV_SAMPLE_OFFSET     0

#define WAV_VALUE_MIN (-1.0)
#define WAV_VALUE_MAX (1.0)

WAV_Sample WAV_SampleFromValue(F64 value);
F64 WAV_SampleToValue(WAV_Sample sample);

WAV_Sample WAV_SampleCombine(WAV_Sample first, WAV_Sample second);

typedef struct WAV_Buffer
{
  MEM_Arena *arena;
  WAV_Sample *samples;
  U32 capacity;
  U32 size;
  U32 rate;
} WAV_Buffer;

WAV_Buffer WAV_BufferInit(MEM_Arena *arena, U32 rate);
void WAV_BufferFree(WAV_Buffer *buffer);

bool WAV_BufferAppend(WAV_Buffer *buffer, WAV_Sample sample);
bool WAV_BufferAppendSineWave(WAV_Buffer *buffer, F64 amplitude, F64 frequency, F64 duration);
bool WAV_BufferAppendBrownNoise(WAV_Buffer *buffer, F64 scale, F64 duration);

bool WAV_BufferSaveToFile(WAV_Buffer *buffer, const char *filename);

c_linkage_end

#endif
