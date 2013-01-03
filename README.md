Notes
=====

Different time args for callback and callbacked function

```
function loop(time)
  note(time, 64, 80, 100)
  callback(time + 250, "loop", time + 500)
end
```

This will make sure the callback is scheduled before the actual work (playing the note) should be done.
Otherwise, the note will always be very slightly late in the case of this function. With functions that
do more work, the difference will become more noticable.
