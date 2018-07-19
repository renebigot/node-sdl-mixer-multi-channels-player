const SDLMixerPlayer = require('bindings')('sdl-mixer-player').SDLMixerPlayer

let _player = null
let _interval = null
let _lastState = null

function updateState () {
  const state = _player.playing()

  if (state !== _lastState) {
    process.send({action: 'playing', state})
    _lastState = state

    if (!state) {
      clearInterval(_interval)
    }
  }
}

process.on('message', msg => {
  switch (msg.action) {
    case 'init':
      _player = new SDLMixerPlayer(msg.deviceName, msg.channel)
      break

    case 'play-file':
      _player.stop()
      _player.playFile(msg.filename)

      if (_interval) {
        clearInterval(_interval)
      }
      _interval = setInterval(() => {
        updateState()
      }, 100)
      break

    case 'play-buffer':
      _player.stop()
      _player.playBuffer(new Uint8Array(msg.buffer.data))

      if (_interval) {
        clearInterval(_interval)
      }
      _interval = setInterval(() => {
        updateState()
      }, 100)
      break

    case 'stop':
      _player.stop()
      break
  }
})
