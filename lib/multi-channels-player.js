const Channel = require('./channel')

class MultiChannelsPlayer {
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
