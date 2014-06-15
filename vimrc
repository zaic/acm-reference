filetype plugin indent on
set number showcmd autowrite
set expandtab tabstop=4 softtabstop=4 shiftwidth=4

map <F5> :make %:r %:r.run<CR>
map <F6> :w\|make CXXFLAGS:="-Og -ggdb" %:r<CR>
imap <F5> <C-o><F5>
imap <F6> <C-o><F6>
