# Source File Duplicate Detector (C)

Console app that finds exact duplicates and highly similar C source files
via multi-level comparison: exact bytes → normalized source → token LCS.

## Layout
```
src/
  main.c, file_manager.*, file_reader.*, normalizer.*,
  tokenizer.*, comparator.*, similarity.*, report.*
test_files/  duplicate1.c, duplicate2.c, formatted.c, commented.c,
             different.c, renamed_identifiers.c, empty.c,
             string_comment_syntax.c
Makefile, build.bat
```

## Build (Windows MinGW)
```
cd duplicate-detector
gcc -Wall -Wextra -std=c11 -O2 -o duplicate_detector.exe src/main.c src/file_manager.c src/file_reader.c src/normalizer.c src/tokenizer.c src/comparator.c src/similarity.c src/report.c
```
or `build.bat`. Linux: `make`.

## Run
```
./duplicate_detector.exe
```
Menu: 1 Add, 2 Remove, 3 Display, 4 Compare Two, 5 Exact, 6 Similar,
7 Matrix, 8 Report, 9 Complete Analysis, 0 Exit.

## Method
- Exact: size check `O(1)` reject, then byte-by-byte `O(L)`; djb2 hash as fast pre-filter (verified by byte compare).
- Normalization: state machine (NORMAL, IN_STRING, IN_CHAR, IN_LINE_COMMENT, IN_BLOCK_COMMENT) so `printf("// Hi")` is preserved; then all whitespace removed.
- Tokenizer: keywords / identifiers / numbers / operators (incl. `==`, `+=`, `->`...) / separators / brackets / strings / chars.
- Similarity: `LCS / max(N,M) * 100` with identifier normalization (`a,x -> ID`). Thresholds: 100 exact, >=90 highly, >=70 similar, >=50 moderately, else low.
- Pairs: `N(N-1)/2`, matrix `O(N^2)` storage, LCS `O(A*B)` time, `O(B)` memory.

## Notes
Identifier normalization is a heuristic, not semantic proof of plagiarism.
See project spec sections 38-39 for limitations / future work.
