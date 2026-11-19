if exists("b:current_syntax")
    finish
endif

syntax match bfError "\d\+"
syntax match bfUnmatchedBracket "[\]}]"
syntax match bfArithmeticOperator "[+-]" nextgroup=bfArithmeticAmount
syntax match bfArithmeticAmount   "\d\{3}\d\@!" contained
syntax match bfPointer       "[<>]" nextgroup=bfPointerAmount
syntax match bfPointerAmount "\d\{3}\d\@!" contained
syntax match bfIO "[.,]"
syntax match bfJumpError    "#\(\d\{4}\d\@!\)\@!"
syntax match bfJumpOperator "#\(\d\{4}\d\@!\)\@=" nextgroup=bfJumpTarget
syntax match bfJumpTarget   "\d\{4}" contained
syntax match bfLoopReverseFalse    "0\(\d\{3}\d\@!\)\@=" contained nextgroup=bfLoopCompare,bfLoopCompareRangeErr
syntax match bfLoopReverseTrue     "[1-9]\(\d\{3}\d\@!\)\@=" contained nextgroup=bfLoopCompare,bfLoopCompareRangeErr
syntax match bfLoopCompare         "\d\{3}" contained
syntax match bfLoopCompareRangeErr "\(25[6-9]\|2[6-9]\d\|[3-9]\d\{2}\)" contained
syntax match bfIfCompare         "\d\{3}" contained
syntax match bfIfCompareRangeErr "\(25[6-9]\|2[6-9]\d\|[3-9]\d\{2}\)" contained
syntax match bfIfSeparator       ":" contained

syntax cluster bfAllCommands contains=bfError,bfArithmeticOperator,bfPointer,bfIO,bfJumpError,bfJumpOperator,bfLoopRegion,bfIfRegion

syntax region bfLoopRegion
    \ matchgroup=bfLoopDelimiter
    \ start="\[" nextgroup=bfLoopReverseFalse,bfLoopReverseTrue
    \ end="\]"
    \ contains=@bfAllCommands,bfLoopReverseFalse,bfLoopReverseTrue

syntax region bfIfErrorRegion
    \ matchgroup=bfIfError
    \ start="{\(\d\{3}\d\@!\)\@!"
    \ end="}"
    \ contains=@bfAllCommands,bfIfSeparator

syntax region bfIfRegion
    \ matchgroup=bfIfDelimiter
    \ start="{\(\d\{3}\d\@!\)\@="
    \ end="}"
    \ contains=@bfAllCommands,bfIfCompare,bfIfCompareRangeErr,bfIfSeparator

function! s:ApplyBFPlusColors()
    highlight bfArithmeticOperator guifg=#ffcc66 ctermfg=221 gui=bold cterm=bold
    highlight bfArithmeticAmount   guifg=#a6e3a1 ctermfg=114
    highlight bfPointer            guifg=#89dceb ctermfg=117 gui=bold cterm=bold
    highlight bfPointerAmount      guifg=#94e2d5 ctermfg=116
    highlight bfIO                 guifg=#f5c2e7 ctermfg=218 gui=bold cterm=bold
    highlight bfJumpOperator       guifg=#eba0ac ctermfg=217 gui=bold cterm=bold
    highlight bfJumpTarget         guifg=#f2cdcd ctermfg=224
    highlight bfLoopDelimiter      guifg=#f9e2af ctermfg=223 gui=bold cterm=bold
    highlight bfLoopReverseFalse   guifg=#9399b2 ctermfg=246
    highlight bfLoopReverseTrue    guifg=#fab387 ctermfg=215 gui=bold cterm=bold
    highlight bfLoopCompare        guifg=#89b4fa ctermfg=111 gui=bold cterm=bold
    highlight bfIfDelimiter        guifg=#cba6f7 ctermfg=183 gui=bold cterm=bold
    highlight bfIfSeparator        guifg=#f38ba8 ctermfg=210 gui=bold cterm=bold
    highlight bfIfCompare          guifg=#b4befe ctermfg=147 gui=bold cterm=bold
    highlight bfError              guifg=#11111b guibg=#f38ba8 ctermfg=233 ctermbg=210 gui=bold cterm=bold
    highlight link bfUnmatchedBracket    bfError
    highlight link bfJumpError           bfError
    highlight link bfIfError             bfError
    highlight link bfLoopCompareRangeErr bfError
    highlight link bfIfCompareRangeErr   bfError
endfunction

call s:ApplyBFPlusColors()

augroup BFPlusColors
    autocmd!
    autocmd ColorScheme * call s:ApplyBFPlusColors()
augroup END

let b:current_syntax = "bfplus"
