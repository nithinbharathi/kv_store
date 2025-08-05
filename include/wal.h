#ifndef _WAL_H_
#define _WAL_H_

void write_wal(const char *command);
void read_wal();
void apply_wal();

#endif