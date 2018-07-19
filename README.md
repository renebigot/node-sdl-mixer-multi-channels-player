# node-sdl-mixer-multi-channels-player


The purpose of `node-sdl-mixer-multi-channels-player` is to allow mono sound routing through any channel (left or right only) of any audio devices, in order to create audio automation devices.

`node-sdl-mixer-multi-channels-player` contains a NodeJS C++ addon to drive `SDL2_Mixer`. 

Due to `SDL2_Mixer` limitations, you can only play WAV files but more formats could be supported through libraries like `node-lame` wich can convert MP3 to WAV.

## Installation 

`yarn add node-sdl-mixer-multi-channels-player`

or 

`npm install --save node-sdl-mixer-multi-channels-player`

## Usage

### Create a player 

Create a multi-channel player :

```javascript
const MultiChannelsPlayer = require('sdl-mixer-multi-channels-player')

const channels = new MultiChannelsPlayer({
  channels: [
    {channel: MultiChannelsPlayer.LEFT},
    {channel: MultiChannelsPlayer.RIGHT}  // Right
  ]
})
```

This player can dispatch sound to the system default audio device `left` or `right` channels.

### Dispatch sound

To dispatch sound to a specific sound device, put `deviceName` in `channels` :

```javascript
const player = new MultiChannelsPlayer({
  channels: [
    {deviceName: 'Built-in Output', channel: MultiChannelsPlayer.LEFT},
    {deviceName: 'Built-in Output', channel: MultiChannelsPlayer.RIGHT},
    {deviceName: 'USB audio CODEC', channel: MultiChannelsPlayer.LEFT},
    {deviceName: 'USB audio CODEC', channel: MultiChannelsPlayer.RIGHT}
  ]
})
```

### List sound devices

You may want to list your available sound devices names:

```javascript
console.log(MultiChannelsPlayer.availableDevices())
```

### Play a sound from file

The player can play WAV sound files only !

```javascript
const file = 'test.wav'
player.playFile(0, file)
```

0 here means that you want to play `test.wav` through the first channel which is `LEFT` channel of `Built-in Output`.

If you play a file through a busy channel, the sound playing will be stopped and the new one will be play.

### Stopping sound

To stop, just call `stop(channel_id)`

```javascript
player.playFile(0, file)
// Stop after 1s
setTimeout(() => { player.stop(0) }, 1000)
```

### Check if channel is playing sound (busy)

```javascript
// Check if channel 0 is playing sound
console.log(player.playing(0) ? 'Playing' : 'Not playing')
```

### Log events for debug

If you want to log the channels states: 

```javascript
const player = new MultiChannelsPlayer({
  channels: [
    {
      deviceName: 'Built-in Output',
      channel: MultiChannelsPlayer.LEFT,
      debug: true
    }
  ]
})
```

Every actions executed by the channel will be log to `STDOUT`. If you want to use a custom logger : 

```javascript
const player = new MultiChannelsPlayer({
  channels: [
    {
      deviceName: 'Built-in Output',
      channel: MultiChannelsPlayer.LEFT,
      debug: true,
      logger: (...args) => {
        console.log('[Defaut::Ch.1]', ...args)
      }
    }
  ]
})
```

### Play sound from buffer

The player can play WAV sound buffer.

```javascript
const fs = require('fs')
const buffer = Buffer.from(fs.readFileSync(file))
player.playBuffer(1, buffer)
```

We've created a buffer from a WAV file. It's not very useful here. But you could use `node-lame` to create a WAV buffer from a MP3 file !!!

```javascript
const Lame = require('node-lame').Lame
const decoder = new Lame({
  'output': 'buffer'
}).setFile('my-sound.mp3')

decoder.decode()
  .then(() => {
    player.playBuffer(0, decoder.getBuffer())
  })
``` 

## Author

René BIGOT

## License

MIT : http://opensource.org/licenses/MIT

