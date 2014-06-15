override CXXFLAGS := -DILIKEGENTOO -std=gnu++0x -O2 -Wall -Wextra -Wshadow -Wno-unused-result -Werror ${CXXFLAGS}
%.run: %; set -o pipefail; time ./$* < $*.in |& tee $*.out
