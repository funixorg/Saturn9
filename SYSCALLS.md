# SATURN9 SYSCALLS

| **%RAX** | **System Call** | **%RBX**      | **%R8**    | **%R9**    | **%R10**   | **%R11**   | **%RBP**   | **RETURN** |
|----------|-----------------|---------------|------------|------------|------------|------------|------------|------------|
| 0x90     | clear           |               |            |            |            |            |            |            |
| 0x91     | print           | char*/int buf |            |            |            |            |            |            |
| 0x92     | readline        |               |            |            |            |            |            | %RSI       |
| 0x93     | readfile        | char* path    |            |            |            |            |            | %RSI       |
| 0x94     | putpixel        |               |  int x     | int y      |            |            | int color  |            |
| 0x95     | drawrect        |               |  int x     | int y      | int width  | int height | int color  |            |