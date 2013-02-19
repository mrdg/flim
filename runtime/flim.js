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
