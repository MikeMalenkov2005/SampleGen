#include <sample_gen.h>

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

WAV_Sample WAV_SampleFromValue(F64 value)
{
  value = Clamp(WAV_VALUE_MIN, value, WAV_VALUE_MAX);
  return (WAV_Sample)((F64)WAV_SAMPLE_AMPLITUDE * value + (F64)WAV_SAMPLE_OFFSET);
}

F64 WAV_SampleToValue(WAV_Sample sample)
{
  return ((F64)sample - (F64)WAV_SAMPLE_OFFSET) / (F64)WAV_SAMPLE_AMPLITUDE;
}

static const U32 WAV_CapacityIncrement = MEM_FastAlignUp(sizeof(WAV_Sample), MEM_ARENA_ALIGNMENT) / sizeof(WAV_Sample);

WAV_Buffer WAV_BufferInit(MEM_Arena *arena, U32 rate)
{
  if (!rate) return (WAV_Buffer) { null };
  WAV_Buffer buffer =
  {
    .arena = arena,
    .samples = MEM_ArenaAllocate(arena, WAV_CapacityIncrement * sizeof(WAV_Sample)),
    .capacity = WAV_CapacityIncrement,
    .rate = rate,
  };
  if (!buffer.samples) MemoryZeroStruct(&buffer);
  return buffer;
}

void WAV_BufferFree(WAV_Buffer *buffer)
{
  if (buffer->arena && buffer->samples)
  {
    MEM_ArenaDeallocate(buffer->arena, buffer->samples);
  }
  MemoryZeroStruct(buffer);
}

bool WAV_BufferAppend(WAV_Buffer *buffer, WAV_Sample sample)
{
  if (!buffer || !buffer->samples) return false;
  if (buffer->size >= buffer->capacity)
  {
    if (!MEM_ArenaAllocate(buffer->arena, WAV_CapacityIncrement * sizeof(WAV_Sample))) return false;
    buffer->capacity += WAV_CapacityIncrement;
  }
  buffer->samples[buffer->size++] = sample;
  return true;
}

bool WAV_BufferAppendSineWave(WAV_Buffer *buffer, F64 amplitude, F64 frequency, F64 duration)
{
  if (!buffer || !buffer->samples) return false;
  UZ count = (UZ)(duration * buffer->rate);
  F64 phase = 0;
  F64 offset = 0;
  if (buffer->size)
  {
    if (amplitude != 0)
    {
      F64 value = WAV_SampleToValue(buffer->samples[buffer->size - 1]) / amplitude;
      if (value < WAV_VALUE_MIN)
      {
        offset = value - WAV_VALUE_MIN;
        value = WAV_VALUE_MIN;
      }
      if (value < WAV_VALUE_MAX)
      {
        offset = value - WAV_VALUE_MAX;
        value = WAV_VALUE_MAX;
      }
      phase = asin(value) + duration / buffer->rate;
    }
    else offset = WAV_SampleToValue(buffer->samples[buffer->size - 1]);
  }
  bool result = true;
  for (UZ i = 0; i < count && result; ++i)
  {
    F64 time = i / (F64)buffer->rate;
    F64 value = amplitude * sin(2 * M_PI * time * frequency + phase) + offset;
    result = WAV_BufferAppend(buffer, WAV_SampleFromValue(value));
  }
  return result;
}

bool WAV_BufferAppendBrownNoise(WAV_Buffer *buffer, F64 scale, F64 duration)
{
  if (!buffer || !buffer->samples) return false;
  F64 value = buffer->size ? WAV_SampleToValue(buffer->samples[buffer->size - 1]) : 0;
  UZ count = (UZ)(duration * buffer->rate);
  scale = 1000 * scale / buffer->rate;
  bool result = true;
  for (UZ i = 0; i < count && result; ++i)
  {
    value += scale * (2 * (F64)rand() / RAND_MAX - 1 - value);
    result = WAV_BufferAppend(buffer, WAV_SampleFromValue(value));
  }
  return result;
}

typedef struct WAV_FileHeader
{
  struct { U8 id[4]; U32 size; } riff;
  U8 wave[4];
  struct { U8 id[4]; U32 size; } fmt;
  U16 format;
  U16 channels;
  U32 sampleRate;
  U32 dataRate;
  U16 blockSize;
  U16 sampleBits;
  struct { U8 id[4]; U32 size; } data;
} WAV_FileHeader;

bool WAV_BufferSaveToFile(WAV_Buffer *buffer, const char *filename)
{
  if (!buffer || !buffer->samples || !buffer->size) return false;

  FILE *file = fopen(filename, "wb");
  if (!file) return false;

  U32 size = buffer->size * sizeof(WAV_Sample);
  size = MEM_FastAlignUp(size, 2);

  WAV_FileHeader header =
  {
    .riff.id = "RIFF",
    .riff.size = sizeof(header) - sizeof(header.riff) + size,
    .wave = "WAVE",
    .fmt.id = "fmt ",
    .fmt.size = sizeof(header) - sizeof(header.riff) - sizeof(header.wave) - sizeof(header.fmt) - sizeof(header.data),
    .format = 1,
    .channels = 1,
    .sampleRate = buffer->rate,
    .dataRate = buffer->rate * sizeof(WAV_Sample),
    .blockSize = sizeof(WAV_Sample),
    .sampleBits = (sizeof(WAV_Sample) << 3),
    .data.id = "data",
    .data.size = size,
  };

  fwrite(&header, sizeof(header), 1, file);
  fwrite(buffer->samples, sizeof(WAV_Sample), buffer->size, file);

  if (size > buffer->size)
  {
    U8 zero = 0;
    fwrite(&zero, 1, 1, file);
  }

  fclose(file);
  return true;
}
