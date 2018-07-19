#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int continuer = 1;
  SDL_Init(SDL_INIT_AUDIO);
  Mix_Init(MIX_INIT_MP3);

  int i, count = SDL_GetNumAudioDevices(0);

  printf("-----------------------------------------\n");
  printf("Available audio devices:\n");
  for (i = 0; i < count; ++i)
  {
    printf("    - Audio device %d: %s\n", i, SDL_GetAudioDeviceName(i, 0));
  }
  printf("\n");

  if (argc <= 2)
  {
    return EXIT_FAILURE;
  }

  char *deviceName = argv[1];

  if (Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024,
                          deviceName, SDL_AUDIO_ALLOW_ANY_CHANGE) == -1)
  {
    printf("[SDL ERROR] :: %s\n\n", Mix_GetError());
    return EXIT_FAILURE;
  }

  printf("Audio device \"%s\" opened with success\n", deviceName);
  printf("-----------------------------------------\n\n");

  int MAX_SOUNDS = argc - 3;
  int currentSound = 0;
  char **files; // = {"file1.wav", "ding.wav"};

  files = (char **)malloc(MAX_SOUNDS * sizeof(char *));

  for (int i = 0; i < MAX_SOUNDS; ++i)
  {
    files[i] = argv[3 + i];
  }

  Mix_AllocateChannels(1);

  if (!Mix_SetPanning(0, argv[2][0] == '1' ? 255 : 0,
                      argv[2][0] == '1' ? 0 : 255))
  {
    printf("[SDL ERROR] :: Mix_SetPanning: %s\n", Mix_GetError());
  }

  Mix_Chunk *sounds[MAX_SOUNDS];

  for (int i = 0; i < MAX_SOUNDS; ++i)
  {
    sounds[i] = Mix_LoadWAV(files[i]);
    Mix_VolumeChunk(sounds[i], MIX_MAX_VOLUME);
  }

  Mix_PlayChannel(0, sounds[0], 0);
  bool playing = true;

  while (playing)
  {
    SDL_WaitEvent(NULL);
    playing = Mix_Playing(0) != 0;

    if (!playing && currentSound < MAX_SOUNDS)
    {
      ++currentSound;
      playing = true;

      if (!Mix_SetPanning(0, argv[2][0] == '1' ? 255 : 0,
                          argv[2][0] == '1' ? 0 : 255))
      {
        printf("[SDL ERROR] :: Mix_SetPanning: %s\n", Mix_GetError());
      }

      Mix_PlayChannel(0, sounds[currentSound], 0);
    }

    printf("Channel 0: %s\r", playing ? "PLAYING" : "STOPPED");
  }

  for (int i = 0; i < MAX_SOUNDS; ++i)
  {
    Mix_FreeChunk(sounds[i]);
  }

  Mix_CloseAudio(); // Fermeture de l'API

  printf("\n");

  return EXIT_SUCCESS;
}
