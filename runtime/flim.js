function metronome(bpm) {
  var bpm = bpm,
      beatLength = 60.0 / bpm,
      start = now()

  return {
    time: function(numberOfBeats) {
      return numberOfBeats * beatLength * 1000
    },

    beat: function(quantize) {
      return Math.ceil((now() - start) / (1000 * beatLength))
    }
  }
}

function makeMetre(nominators, denominator) {
  var metreLength = _.inject(nominators, function(sum, n) {
    return sum += n
  }, 0)

  return function(time, beat) {
    var qtime = (time / denominator) % metreLength,
        metre = nominators,
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
