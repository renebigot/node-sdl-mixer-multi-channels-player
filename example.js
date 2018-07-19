const fs = require('fs')
const MultiChannelsPlayer = require('./')

console.log(MultiChannelsPlayer.availableDevices())

const player = new MultiChannelsPlayer({
  channels: [
    {channel: 0, debug: true},
    {channel: 1, debug: true, logger: (...args) => console.log('[D1C2]', ...args)}
  ]
})

const file = 'test.wav'

// Play file on channel 0
player.playFile(0, file)

// 500ms after app start play file on channel 0
setTimeout(() => {
  player.playFile(0, file)
}, 500)

// 800ms after app start play buffer on channel 1
const buffer = Buffer.from(fs.readFileSync(file))
setTimeout(() => {
  player.playBuffer(1, buffer)
}, 800)
