#pragma once

#define NAMES_SAME_CONTENT_DIFF 1
#define NAMES_DIFF_CONTENT_DIFF 2
#define NAMES_DIFF_CONTENT_SAME 3
#define NAMES_SAME_CONTENT_SAME 4

#define FIRST_FILE_NOT_EXIST  -1
#define SECOND_FILE_NOT_EXIST -2

#define BLOCK_SIZE          (1 << 22)
#define BLOCK_COUNT         (1 << 4)
#define HASH_BLOCK_SIZE     qint64(BLOCK_SIZE * BLOCK_COUNT)

#define FILE_HAS_BEEN_COPIED 0
#define FILE_EXIST           1
#define FILE_HAS_BEEN_MOVED  2
