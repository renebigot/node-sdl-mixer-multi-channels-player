const Channel = require('./channel')

class MultiChannelsPlayer {
  static get availableDevices () { return require('bindings')('sdl-mixer-player').availableDevices }
  static get LEFT () { return 0 }
  static get RIGHT () { return 1 }

  constructor ({ channels = [{}] }) {
    this._channels = channels.map(el => {
      return new Channel(el)
    })
  }

  playFile (channel, filename) {
    this._channels[channel] && this._channels[channel].playFile(filename)
  }

  playBuffer (channel, buffer) {
    this._channels[channel] && this._channels[channel].playBuffer(buffer)
  }

  stop (channel) {
    this._channels[channel] && this._channels[channel].stop()
  }

  playing (channel) {
    return this._channels[channel] && this._channels[channel].playing()
  }
}

module.exports = MultiChannelsPlayer
