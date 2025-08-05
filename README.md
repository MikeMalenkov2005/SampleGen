# SampleGen

SampleGen is a sound sample generation library for C.

Generated samples can be saved to a `.wav` file.

## Supported Generators

- **Sine Wave**: `WAV_BufferAppendSineWave` function appends a sine wave with
given amplitude, duration and frequency to the `WAV_Buffer`.
- **Brown Noise**: `WAV_BufferAppendBrownNoise` function appends a brown noise
with given duration and scaling to the `WAV_Buffer`.

## TODO List

- Add more different generators.
