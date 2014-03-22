function! FlimSend(str)
  call system('./bin/flimc', a:str)
endfunction

function! FlimFlash(expr)
  call matchadd('Visual', a:expr)
  redraw
  sleep 75m
  call clearmatches()
endfunction

function! FlimLineEval()
  normal! "ayy
  call FlimSend(@a)
  let current_line = line('.')
  call FlimFlash('\%'.current_line.'l')
endfunction

function! FlimFunctionEval()
  let old_pos  = getpos('.')
  let line_nr  = line('.')
  let line_txt = getline(line_nr)
  let indent   = indent(line_nr)

  if line_txt =~? '^function'
    normal! 0
    let first = getpos('.')[1]
  elseif line_txt =~? '^}' || indent > 0
    if search('^function', 'bW') > 0
      let first = getpos('.')[1]
    else
      echo "Couldn't find function start"
      return
    endif
  else
    echo "Not a function"
    return
  endif

  if search('^}', 'W') > 0
    let last = getpos('.')[1]
    silent exec 'normal! '. first . 'gg' . ((last - first) + 1) .'"ayy'
    call FlimSend(@a)
    call FlimFlash('\%>'.(first - 1).'l\%<'.(last + 1).'l')
  else
    echo "Couldn't find function end"
  endif
  call setpos('.', old_pos)
endfunction

map <silent> <leader>c :call FlimFunctionEval() <CR>
map <C-@> <C-Space>
map <C-Space> :call FlimLineEval()<CR>
