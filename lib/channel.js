const fork = require('child_process').fork
const path = require('path')

class Channel {
  constructor ({deviceName = null, channel = 0, debug = false, logger = console.log}) {
    this._log = (debug && logger) || (() => {})
    this._channel = channel
    this._deviceName = deviceName

    this._log(`Creating channel "${deviceName} :: ${channel}"`)

    this._playerWorker = fork(path.resolve(__dirname, 'player-worker.js'))
    this._playerWorker.send({action: 'init', deviceName, channel})

    this._playerWorker.on('message', msg => {
      if (msg.action === 'playing') {
        this._playing = msg.state
        this._log(`${this._playing ? 'Sound is playing' : 'Sound has been stopped'}`)
      }
    })
  }

  playFile (filename) {
    this._log(`Play file "${filename}"`)
    this._playerWorker.send({action: 'play-file', filename})
  }

  playBuffer (buffer) {
    this._log(`Play buffer`)
    this._playerWorker.send({action: 'play-buffer', buffer})
  }

  stop () {
    this._log(`Stop`)
    this._playerWorker.send({
      action: 'stop'
    })
  }

  playing () {
    this._log(`${this._playing ? 'Playing' : 'Not playing'}`)
    return this._playing
  }
}

module.exports = Channel
