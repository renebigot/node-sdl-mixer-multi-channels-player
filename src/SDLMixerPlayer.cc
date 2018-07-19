#include <nan.h>
#include <unistd.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>

#define NO_LOOP 0
#define CHANNEL_ID 0

using Nan::ObjectWrap;

class SDLMixerPlayer : public Nan::ObjectWrap
{
public:
  static NAN_MODULE_INIT(Init)
  {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("SDLMixerPlayer").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "playFile", PlayFile);
    Nan::SetPrototypeMethod(tpl, "playBuffer", PlayBuffer);
    Nan::SetPrototypeMethod(tpl, "stop", Stop);
    Nan::SetPrototypeMethod(tpl, "playing", Playing);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("SDLMixerPlayer").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
  }

  static NAN_METHOD(Playing)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());
    info.GetReturnValue().Set(self->_audioOpened &&
                              Mix_Playing(CHANNEL_ID) > 0);
  }

  static NAN_METHOD(PlayBuffer)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());
    if (info.Length() < 1)
    {
      return Nan::ThrowError("Invalid number of arguments");
    }

    if (!info[0]->IsUint8Array())
    {
      return Nan::ThrowError(
          "Argument for \"sound buffer\" must be a Buffer");
    }

    self->OpenAudioDevice(info);

    v8::Local<v8::Uint8Array> view = info[0].As<v8::Uint8Array>();
    int8_t *buffer = static_cast<int8_t*>(view->Buffer()->GetContents().Data());

    SDL_RWops *sdlBuffer = SDL_RWFromMem(buffer, view->Length());
    self->_sound = Mix_LoadWAV_RW(sdlBuffer, 1);

    self->PlayChunk(info);
  }

  static NAN_METHOD(PlayFile)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());

    if (info.Length() < 1)
    {
      return Nan::ThrowError("Invalid number of arguments");
    }

    if (!info[0]->IsString())
    {
      return Nan::ThrowError(
          "Argument for \"sound file name\" must be a String");
    }

    self->OpenAudioDevice(info);

    Nan::Utf8String nanFileName(info[0]->ToString());
    std::string stdFilename(*nanFileName);

    self->_sound = Mix_LoadWAV(stdFilename.c_str());

    self->PlayChunk(info);
  }

  static NAN_METHOD(Stop)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());

    if (self->_audioOpened)
    {

      self->_audioOpened = false;
      Mix_HaltChannel(CHANNEL_ID);
      Mix_CloseAudio();
    }
  }

private:
  explicit SDLMixerPlayer(v8::Local<v8::Value> deviceName, int channel = 0)
  {
    Nan::Utf8String nanDeviceName(deviceName);
    std::string stdDeviceName(*nanDeviceName);

    this->_deviceName = (char *)malloc(stdDeviceName.length() * sizeof(char));
    this->_channel = channel;

    strcpy(this->_deviceName, stdDeviceName.c_str());
  }

  explicit SDLMixerPlayer(int channel = 0)
  {
    this->_deviceName = NULL;
    this->_channel = channel;
  }

  ~SDLMixerPlayer() {}

  static NAN_METHOD(PlayChunk)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());

    Mix_VolumeChunk(self->_sound, MIX_MAX_VOLUME);

    int left = self->_channel == 0 ? 255 : 0;
    int right = self->_channel == 0 ? 0 : 255;

    if (!Mix_SetPanning(CHANNEL_ID, left, right))
    {
      Nan::ThrowError(Mix_GetError());
    }

    Mix_PlayChannel(CHANNEL_ID, self->_sound, NO_LOOP);
  }

  static NAN_METHOD(OpenAudioDevice)
  {
    SDLMixerPlayer *self = Unwrap<SDLMixerPlayer>(info.Holder());

    if (Mix_OpenAudioDevice(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS,
                            1024, self->_deviceName,
                            SDL_AUDIO_ALLOW_ANY_CHANGE) == -1)
    {

      printf("-------------------------------------------------\n");
      printf("Can't open audio device. Please, use a valid one:\n");
      int count = SDL_GetNumAudioDevices(0);
      for (int i = 0; i < count; ++i)
      {
        printf("    - \"%s\"\n", SDL_GetAudioDeviceName(i, 0));
      }
      printf("-------------------------------------------------\n");

      Nan::ThrowError(Mix_GetError());
      exit(EXIT_FAILURE);
    }

    self->_audioOpened = true;
    Mix_AllocateChannels(1);
  }

  static NAN_METHOD(New)
  {
    SDL_Init(SDL_INIT_AUDIO);

    if (info.IsConstructCall())
    {
      if (info.Length() != 2)
      {
        return Nan::ThrowError("Invalid number of arguments");
      }

      if (!info[0]->IsString() && !info[0]->IsNull())
      {
        return Nan::ThrowError("Argument for \"device name\" must be a string");
      }

      if (!info[1]->IsNumber())
      {
        return Nan::ThrowError("Argument for \"channel\" must be a number");
      }

      v8::Local<v8::String> deviceName = info[0]->ToString();
      int channel = info[1]->Uint32Value();

      SDLMixerPlayer *obj = info[0]->IsNull()
                                ? new SDLMixerPlayer(channel)
                                : new SDLMixerPlayer(deviceName, channel);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }
    else
    {
      const int argc = 2;
      v8::Local<v8::Value> argv[argc] = {info[0], info[1]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(
          Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static inline Nan::Persistent<v8::Function> &constructor()
  {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  char *_deviceName;
  Mix_Chunk *_sound;
  int _channel;
  bool _audioOpened;
};

NODE_MODULE(objectwrapper, SDLMixerPlayer::Init)
