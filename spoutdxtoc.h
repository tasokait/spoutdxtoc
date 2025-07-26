#pragma once

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void SPOUT_DX;

typedef struct {
    char **list;
    uint32_t count;
} SPOUT_DX_NAMELIST;

/* Returns NULL if Wine stub in use */
SPOUT_DX* SpoutDXToCNew(void);

void SpoutDXToCFree(SPOUT_DX *self);

int SpoutDXToCGetSenderCount(SPOUT_DX *self);

bool SpoutDXToCGetSender(SPOUT_DX *self, int64_t index, char **sendername);

/* Returns a NULL-terminated list */
char** SpoutDXToCGetSenderListSimple(SPOUT_DX *self, uint32_t *ret_count);

void SpoutDXToCNamelistClear(SPOUT_DX_NAMELIST *namelist);

/* TODO: Document it */
bool SpoutDXToCGetSenderList(SPOUT_DX *self, const char **old_list, SPOUT_DX_NAMELIST *ret_senders, SPOUT_DX_NAMELIST *ret_added, SPOUT_DX_NAMELIST *ret_removed);

#ifdef __cplusplus
}
#endif
