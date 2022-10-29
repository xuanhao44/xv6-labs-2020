struct buf {
  struct spinlock spinlock;
  struct sleeplock sleeplock;
  int valid;   // has data been read from disk?
  int disk;    // does disk "own" buf?
  uint dev;
  uint blockno;
  uint refcnt;
  uint timestamp;
  uchar data[BSIZE];
};

