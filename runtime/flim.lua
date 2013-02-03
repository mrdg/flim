function each(fn, ...)
  for _, v in ipairs(...) do
    fn(v)
  end
end

function eachn(fn, ...)
  local arrays = {...}
  for i = 1, # arrays[1] do
    fn(unpack(map(function(arr)
      return arr[i]
    end, ...)))
  end
end

function map(fn, ...)
  local results = {}
  local array = {...}
  for i,v in ipairs(array) do
    results[i] = fn(v)
  end
  return results
end

function mapn(fn, ...)
  local results = {}
  local arrays = {...}
  for i = 1, # arrays[1] do
    results[i] = fn(unpack(map(function(arr)
      return arr[i]
    end, ...)))
  end
  return results
end

function pick(a, b, condition)
  if condition then return a else return b end
end

function chord(time, pitches, velocity, duration, channel)
  for _, p in ipairs(pitches) do
    note(time, p, velocity, duration, channel)
  end
end

function make_metro(tempo)
  local tempo = tempo
  local time = now()
  local g_tempo = 60.0 / tempo

  return function(str, ...)
    if type(str) == "number" then
      return str * g_tempo * 1000
    elseif str == 'beat' then
      return math.ceil(((now() - time) / (1000 * g_tempo)))
    end
  end
end
