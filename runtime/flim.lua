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

function array_to_str(array)
  local str = "["
  for _, v in ipairs(array) do
    str = str .. tostring(v)
  end
  str = str .. "]"
  return str
end

function sum(array)
  local result = 0
  for _,v in ipairs(array) do
    result = result + v
  end
  return result
end

function rest(array)
  local dup = clone(array)
  table.remove(dup, 1)
  return dup
end

function clone(array)
  local c = {}
  for _, value in ipairs(array) do
    table.insert(c, value)
  end
  return c
end

function make_metre(metre, base)
  local metre_length = sum(metre)
  return function(time, beat)
    local qtime = (time / base) % metre_length
    local metre = metre
    local a = metre[1]
    local b = 0
    while (qtime >= a) do
      a = a + metre[2]
      b = b + metre[1]
      metre = rest(metre)
    end
    return 1.0 + (qtime - b) == beat
  end
end
