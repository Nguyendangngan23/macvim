vim9script
# Support scripts for MacVim-specific functionality
# Maintainer:   Yee Cheng Chin (macvim-dev@macvim.org)
# Last Change:  2022-10-14

# Retrieves the text under the selection, without polluting the registers.
# This is easier if we could yank, but we don't know what the user has been
# doing. One way we could have accomplished this was to save the register info
# and then restore it, but this runs into problems if the unnamed register was
# pointing to the "* register as setting and restoring the system clipboard
# could be iffy (if there are non-text items in the clipboard). It's cleaner
# to just use a pure Vimscript solution without having to rely on yank.
def SelectedText(): string
  var [line_start, column_start] = getpos("'<")[1 : 2]
  var [line_end, column_end] = getpos("'>")[1 : 2]
  final lines = getline(line_start, line_end)
  if len(lines) == 0
    return ''
  endif

  const visualmode = visualmode()

  if visualmode ==# 'v'
    if line_start == line_end && column_start == column_end
      # Exclusive has a special case where you always select at least one
      # char, so just handle the case here.
      return lines[0][column_start - 1]
    endif
    if &selection ==# "exclusive"
      column_end -= 1 # exclusive selection don't count the last column (usually)
    endif
    lines[-1] = lines[-1][ : column_end - 1]
    lines[0] = lines[0][column_start - 1 : ]
  elseif visualmode ==# "\<C-V>"
    if column_end <= column_start
      # This can happen with v_O, need to swap start/end
      const temp = column_start
      column_start = column_end
      column_end = temp
      # Also, exclusive mode is weird in this state in that we don't need to
      # do column_end -= 1, and it acts like inclusive instead.
    else
      if &selection ==# "exclusive"
        column_end -= 1 # normal exclusive behavior, need to cull the last column.
      endif
    endif
    for idx in range(len(lines))
      lines[idx] = lines[idx][column_start - 1 : column_end - 1]
    endfor
  else
    # Line mode doesn't have to do anything to trim the lines
  endif
  return join(lines, "\n")
enddef


# Ask macOS to show the definition of the last selected text. Note that this
# uses '<, and therefore has to be used in normal mode where the mark has
# already been updated.
export def ShowDefinitionSelected()
  const sel_text = SelectedText()
  if len(sel_text) > 0
    const sel_start = getpos("'<")
    const sel_screenpos = win_getid()->screenpos(sel_start[1], sel_start[2])
    showdefinition(sel_text, sel_screenpos)
  endif
enddef

# Ask macOS to show the definition of the word under the cursor.
export def ShowDefinitionUnderCursor()
  call search('\<', 'bc') # Go to the beginning of a word, so that showdefinition() will show the popup at the correct location.

  const text = expand('<cword>')
  if len(text) > 0
    showdefinition(text)
  endif
enddef

# vim: set sw=2 ts=2 et :
