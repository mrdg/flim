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
