const fs = require('fs')
const path = require('path')
const MultiChannelsPlayer = require('./')
const deviceName = 'Built-in Output'

const channels = new MultiChannelsPlayer({
  channels: [
    {deviceName: deviceName, channel: 0, logger: (...args) => console.log('[Channel 1]', ...args)},
    {deviceName: deviceName, channel: 1, logger: (...args) => console.log('[Channel 2]', ...args)}
  ]
})

const file = 'test.wav'
channels.playFile(0, file)
setTimeout(() => { channels.playFile(0, file) }, 500)
const buffer = Buffer.from(fs.readFileSync(file))
setTimeout(() => { channels.playBuffer(1, buffer) }, 800)
