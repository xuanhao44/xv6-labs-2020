struct buf {
  struct sleeplock lock;
  int valid;   // has data been read from disk?
  int disk;    // does disk "own" buf?
  uint dev;
  uint blockno;
  uint refcnt;
  struct buf *prev;
  struct buf *next;
  uint timestamp;
  uchar data[BSIZE];
};