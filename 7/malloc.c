//
// >>>> malloc challenge! <<<<
//
// Your task is to improve utilization and speed of the following malloc
// implementation.
// Initial implementation is the same as the one implemented in simple_malloc.c.
// For the detailed explanation, please refer to simple_malloc.c.

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//
// Interfaces to get memory pages from OS
//

// システムからメモリ領域を割り当てる
void *mmap_from_system(size_t size);
// メモリをシステムに解放する
void munmap_to_system(void *ptr, size_t size);

//
// Struct definitions
//

// 連結リスト
typedef struct my_metadata_t
{
  // サイズ
  size_t size;
  // つぎのmetadataへのポインタ
  struct my_metadata_t *next;
} my_metadata_t;

typedef struct my_heap_t
{
  // freeリストの先頭の配列
  my_metadata_t *free_head[5];
  // ダミー
  my_metadata_t dummy[5];
} my_heap_t;

//
// Static variables (DO NOT ADD ANOTHER STATIC VARIABLES!)
//

my_heap_t my_heap;

//
// Helper functions (feel free to add/remove/edit!)
//
// おたすけ関数、編集OK

// indexを計算する関数
int calc_index(size_t size)
{
  int index = 0;
  if (size < 32)
  {
    index = 0;
  }
  else if (size < 128)
  {
    index = 1;
  }
  else if (size < 512)
  {
    index = 2;
  }
  else if (size < 2048)
  {
    index = 3;
  }
  else
  {
    index = 4;
  }
  return index;
}

// binの中身を出力する関数
void print_free_bin()
{

  my_metadata_t *metadata;
  for (int i = 0; i < 5; i++)
  {
    metadata = my_heap.free_head[i];
    while (1)
    {
      if (!metadata)
      {
        printf("i:%d NULL\n", i);
        break;
      }
      printf("i:%d size:%lu\n", i, metadata->size);
      if (metadata->size == 0)
      {
        break;
      }
      metadata = metadata->next;
    }
  }
}

// metadataをfreelistの先頭にしてる？
void my_add_to_free_list(my_metadata_t *metadata)
{
  // nextがなければOK？
  //(下の関数より、使用中のやつはnextがNULLになっている)
  assert(!metadata->next);

  // printf("my_add_to_free_list i:%d size:%lu\n", i, metadata->size);
  // iをきめる
  int i = calc_index(metadata->size);

  // printf("before add i:%d size:%lu\n",i,my_heap.free_head[i]->size);
  // printf("before add\n");
  // print_free_bin();

  // nextをmy_heapのfree_headにしてる...
  // 受け取った要素をfree_listの先頭にしてる？
  metadata->next = my_heap.free_head[i];
  my_heap.free_head[i] = metadata;
  // printf("after add i:%d size:%lu\n",i,my_heap.free_head[i]->size);
  //  if(my_heap.free_head[i]->next){
  //   printf("??? i:%d size:%lu\n",i,my_heap.free_head[i]->next->size);
  //  }
  // printf("after add\n");
  // print_free_bin();
}

// metadataをfree_listから取っ払っている？
void my_remove_from_free_list(my_metadata_t *metadata, my_metadata_t *prev)
{
  // printf("before remove\n");
  // print_free_bin();

  // iをきめる
  int i = calc_index(metadata->size);

  // prevがなんかある場合
  if (prev)
  {
    // prevのnextをmetadataのnextにする
    prev->next = metadata->next;
  }
  else
  { // なんもなかった場合(先頭だった場合？)
    // free_headをmetadataのnextにする
    // freelist[i]の先頭を変更している
    my_heap.free_head[i] = metadata->next;
  }
  // metadataのnextをNULLにしている
  // 使用中のやつはnextがNULLになっている？
  metadata->next = NULL;
  // printf("after remove\n");
  // print_free_bin();
}

//
// Interfaces of malloc (DO NOT RENAME FOLLOWING FUNCTIONS!)
//

// This is called at the beginning of each challenge.
void my_initialize()
{ // 初期化
  // freelistの先頭をダミーにしてる
  for (int i = 0; i < 5; i++)
  {
    my_heap.free_head[i] = &my_heap.dummy[i];
    my_heap.dummy[i].size = 0;
    my_heap.dummy[i].next = NULL;
  }
}

// my_malloc() is called every time an object is allocated.
// |size| is guaranteed to be a multiple of 8 bytes and meets 8 <= |size| <=
// 4000. You are not allowed to use any library functions other than
// mmap_from_system() / munmap_to_system().
// 割り当てのたびに呼び出される
// サイズの大きさは8以上4000以下である
// ライブラリ関数つかっちゃだめ
void *my_malloc(size_t size)
{ // このサイズのわりあておねがいしますってしている
  // freelistの先頭
  int index = calc_index(size);

  my_metadata_t *metadata = my_heap.free_head[index];
  // 1個前の要素？
  my_metadata_t *prev = NULL;

  // bestな要素を保存するやつ
  my_metadata_t *best = NULL;
  my_metadata_t *best_prev = NULL;

  // First-fit: Find the first free slot the object fits.
  // TODO: Update this logic to Best-fit!
  // 最初に見つけたフィットするものをそのまま割り当てている
  // ちょうどいいやつを見つけるように変更しよう

  // freelistをひとつずつ見ていく
  // metadataの大きさが要求サイズ以上であれば止まる
  for (int i = index; i < 5; i++)
  {
    metadata = my_heap.free_head[i];
    prev = NULL;

    while (metadata)
    {

      if (metadata->size == size)
      {
        best = metadata;
        best_prev = prev;
        break;
      }
      else if (metadata->size > size)
      {
        if (!best)
        {
          best = metadata;
          best_prev = prev;
        }
        else if (metadata->size < best->size)
        {
          best = metadata;
          best_prev = prev;
        }
      }
      prev = metadata;
      metadata = metadata->next;
    }

    // bestなものを見つけられていたら探索おわり
    if (best)
    {
      // printf("best:%lu\n",best->size);
      break;
    }
  }

  prev = best_prev;
  metadata = best;
  // now, metadata points to the first free slot
  // and prev is the previous entry.

  if (!metadata)
  {
    // 空きが見つからなかったら新しいメモリをシステムからもらう
    // There was no free slot available. We need to request a new memory region
    // from the system by calling mmap_from_system().
    //
    //     | metadata | free slot |
    //     ^
    //     metadata
    //     <---------------------->
    //            buffer_size
    size_t buffer_size = 4096;
    my_metadata_t *metadata = (my_metadata_t *)mmap_from_system(buffer_size);
    metadata->size = buffer_size - sizeof(my_metadata_t);
    metadata->next = NULL;
    // Add the memory region to the free list.
    my_add_to_free_list(metadata);
    // Now, try my_malloc() again. This should succeed.
    return my_malloc(size);
  }

  // |ptr| is the beginning of the allocated object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  void *ptr = metadata + 1;
  // 新しい空き領域のサイズ？
  size_t remaining_size = metadata->size - size;
  // Remove the free slot from the free list.
  my_remove_from_free_list(metadata, prev);

  // 新しい空き領域のサイズが連結リストのサイズよりも大きい(？)
  if (remaining_size > sizeof(my_metadata_t))
  {
    // Shrink the metadata for the allocated object
    // to separate the rest of the region corresponding to remaining_size.
    // If the remaining_size is not large enough to make a new metadata,
    // this code path will not be taken and the region will be managed
    // as a part of the allocated object.

    // ここでbinに入れた空き領域のサイズが更新されて、
    // binにいれたいサイズの幅と一致しなくなっている？
    // ここでなんでsizeを更新するのかはよくわからない

    // printf("before resize\n");
    // print_free_bin();

    if (calc_index(metadata->size) < index)
    {
      my_remove_from_free_list(metadata, prev);
      metadata->size = size;
      my_add_to_free_list(metadata);
    }
    else
    {
      metadata->size = size;
    }
    // printf("after resize\n");
    // print_free_bin();

    // Create a new metadata for the remaining free slot.
    //
    // ... | metadata | object | metadata | free slot | ...
    //     ^          ^        ^
    //     metadata   ptr      new_metadata
    //                 <------><---------------------->
    //                   size       remaining size

    // あたらしい領域
    my_metadata_t *new_metadata = (my_metadata_t *)((char *)ptr + size);
    new_metadata->size = remaining_size - sizeof(my_metadata_t);
    new_metadata->next = NULL;
    // Add the remaining free slot to the free list.
    // あたらしい領域をフリーリストに追加
    my_add_to_free_list(new_metadata);
  }
  return ptr;
}

// This is called every time an object is freed.  You are not allowed to
// use any library functions other than mmap_from_system / munmap_to_system.
// フリーのたびに呼ばれている
void my_free(void *ptr)
{ // objectのさいしょのポインタを受け取っている？
  // Look up the metadata. The metadata is placed just prior to the object.
  //
  // ... | metadata | object | ...
  //     ^          ^
  //     metadata   ptr
  my_metadata_t *metadata = (my_metadata_t *)ptr - 1;

  // Add the free slot to the free list.
  // フリーリストにmetadata足してる
  my_add_to_free_list(metadata);
}

// This is called at the end of each challenge.
void my_finalize()
{
  // Nothing is here for now.
  // feel free to add something if you want!
}

void test()
{
  // Implement here!
  // my_initialize();
  // print_free_bin();
  // void *ptr = (my_malloc(400));
  // printf("malloc done\n");
  // print_free_bin();
  // printf("free start\n");
  // my_free(ptr);
  // printf("free done\n");
  // print_free_bin();
  assert(1 == 1); /* 1 is 1. That's always true! (You can remove this.) */
}
