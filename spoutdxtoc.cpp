#include "spoutdxtoc.h"

#include <algorithm>
#include <string>
#include <vector>

/* Include SpoutDX and ignore its headers warnings */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunknown-pragmas"
#pragma GCC diagnostic ignored "-Wunused-function"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include "SpoutDX.h"

#pragma GCC diagnostic pop

SPOUT_DX* __stdcall SpoutDXToCNew (void)
{
  return new spoutDX();
}

void __stdcall SpoutDXToCFree (SPOUT_DX *self)
{
  assert(self != NULL);

  delete reinterpret_cast<spoutDX*>(self);
}

int __stdcall SpoutDXToCGetSenderCount(SPOUT_DX *self)
{
  assert(self != NULL);

  return reinterpret_cast<spoutDX*>(self)->GetSenderCount();
}

#define NAME_MAX_SIZE 256

bool __stdcall SpoutDXToCGetSender(SPOUT_DX *self, int64_t index, char **sendername)
{
  assert(self != NULL);
  assert(sendername != NULL && *sendername == NULL);

  *sendername = (char *)calloc(1, NAME_MAX_SIZE*sizeof(char));

  if (reinterpret_cast<spoutDX*>(self)->GetSender((int)index, *sendername, NAME_MAX_SIZE))
    return true;

  free(*sendername);
  *sendername = NULL;

  return false;
}

static void vec_to_null_term_clist(std::vector<std::string> &vector, char ***list)
{
  char **name;

  *list = (char **)calloc(vector.size() + 1, sizeof(char *));

  name = *list;
  for (std::string &s: vector)
  {
    *name = strdup(s.c_str());
    name++;
  }

  name = *list;
  assert(name[vector.size()] == NULL);
}

char** __stdcall SpoutDXToCGetSenderListSimple(SPOUT_DX *self, uint32_t *ret_count)
{
  std::vector<std::string> senderlist;
  char **list = NULL;

  assert(self != NULL);

  senderlist = reinterpret_cast<spoutDX*>(self)->GetSenderList();
  vec_to_null_term_clist(senderlist, &list);

  if (ret_count != NULL)
    *ret_count = senderlist.size();

  return list;
}

void __stdcall SpoutDXToCNamelistClear(SPOUT_DX_NAMELIST *namelist)
{
  assert (namelist != NULL);

  if (namelist->list == NULL)
    return;

  for (uint32_t i = 0; namelist->list[i] != NULL; i++)
    free(namelist->list[i]);

  free(namelist->list);
  namelist->list = NULL;
}

bool __stdcall SpoutDXToCGetSenderList(SPOUT_DX *self, const char **old_list, SPOUT_DX_NAMELIST *ret_senders, SPOUT_DX_NAMELIST *ret_added, SPOUT_DX_NAMELIST *ret_removed)
{
  std::vector<std::string> senderlist, list, removed;

  assert(self != NULL);

  if (ret_senders == NULL)
  {
    assert(ret_added != NULL && ret_added->list == NULL);
    assert(ret_removed != NULL && ret_removed->list == NULL);
  }
  else {
    assert(ret_senders->list == NULL);
    assert(ret_added == NULL || ret_added->list == NULL);
    assert(ret_removed == NULL || ret_removed->list == NULL);
  }

  list = (senderlist = reinterpret_cast<spoutDX*>(self)->GetSenderList());
  for (size_t i = 0; old_list != NULL && old_list[i] != NULL; i++)
  {
    std::string sender(old_list[i]);
    auto it = std::find(list.begin(), list.end(), sender);

    if (it != list.end())
      list.erase(it);
    else
      removed.push_back(sender);
  }

  if (list.empty() && removed.empty())
    return false;

  if (ret_senders != NULL)
  {
    vec_to_null_term_clist(senderlist, &ret_senders->list);
    ret_senders->count = senderlist.size();
  }

  if (ret_added != NULL)
  {
    vec_to_null_term_clist(list, &ret_added->list);
    ret_added->count = list.size();
  }

  if (ret_removed != NULL)
  {
    vec_to_null_term_clist(removed, &ret_removed->list);
    ret_removed->count = removed.size();
  }
  
  return true;
}
