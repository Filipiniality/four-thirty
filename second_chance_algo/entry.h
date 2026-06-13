#ifndef ENTRY_H
#define ENTRY_H

class Entry {
 public:
  bool reference = false; // true whenever this block as accessed. false by 2nd chance alg.
  bool dirty = false; // true whenever this block is written. false upon a writeback.
  int pNumber = -1; // contains the disk block number. -1 if this entry has no block.
};
#endif // ENTRY_H
