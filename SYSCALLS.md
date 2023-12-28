# SATURN9 SYSCALLS

| **%RAX** | **System Call** | **%RBX**      | **%RDI** | **RETURN** |
|----------|-----------------|---------------|----------|------------|
| 0x90     | clear           |               |          |            |
| 0x91     | print           | char*/int buf |          |            |
| 0x92     | readline        |               |          | %RSI       |