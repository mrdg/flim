function metronome(bpm) {
  var bpm = bpm,
      beatLength = (60.0 / bpm) * 1000.0,
      start = now()

  return {
    time: function(numberOfBeats) {
      return numberOfBeats * beatLength
    },

    beat: function() {
      return Math.ceil(now() / beatLength)
    }
  }
}

function makeMetre(numerators, denominator) {
  var metreLength = _.inject(numerators, function(sum, n) {
    return sum += n
  }, 0)

  return function(time, beat) {
    var qtime = (time / denominator) % metreLength,
        metre = numerators,
        a = metre[0],
        b = 0

    while (qtime >= a) {
      a = a + metre[1]
      b = a + metre[0]
      metre = _.rest(metre)
    }
    return 1.0 + (qtime - b) == beat
  }
}

function rand(min, max) {
  return Math.round(Math.random() * (max - min) + min)
}

function generator() {
  var args = Array.slice(arguments)
  var index = 0
  return function() {
    if (index == args.length) index = 0
    return args[index++]
  }
}

function instrument(options) {
  var options = options || {}
  var params = _.defaults(options, {
    pitch: 60,
    velocity: 80,
    duration: 100,
    channel: 1
  })
  return {
    play: function(time) {
      var args = Array.slice(arguments)
      var pitch    = args[1] || params.pitch
      var velocity = args[2] || params.velocity
      var duration = args[3] || params.duration
      var channel  = args[4] || params.channel
      note(time, pitch, velocity, duration, channel)
    }
  }
}
