#ifndef _WAL_H_
#define _WAL_H_

void wal_init();
void add_entry(const char *buffer);
void wal_close();
#endif