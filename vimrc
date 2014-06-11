set number              " show lines numbers
set et                  " using spaces instead of tabs
set ts=4 sts=4 sw=4     " set tabulation to 4 spaces
set cindent             " indent

nmap <F5> :wa<CR>:!g++ -std=g++0x -DILIKEGENTOO -Wall -Wextra -Wshadow -Werror -O2 -g %:t:r.cpp -o %:t:r && time ./%:t:r <%:t:r.in >%:t:r.out && cat %:t:r.out<CR>
nmap <F6> :wa<CR>:!g++ -std=g++0x -Wall -Wextra -Wshadow -Werror -O2 %:t:r.cpp -o %:t:r && time ./%:t:r <%:t:r.in >%:t:r.out && cat %:t:r.out<CR>
nmap <F7> :wa<CR>:!g++ -std=g++0x -Wall -Wextra -Wshadow -O0 -g %:t:r.cpp -o %:t:r && time ./%:t:r <%:t:r.in >%:t:r.out && cat %:t:r.out<CR>
imap <F5> <ESC><F5>
imap <F6> <ESC><F6>
