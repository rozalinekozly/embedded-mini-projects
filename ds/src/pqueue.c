/*-----------------------------------------------------------------------------
submitter : Rozaline Kozly
reviewer : steve
version : 1
date : 6 Jan 2026
------------------------------------------------------------------------------*/
#include <stdlib.h>	/* malloc, free */
#include <assert.h>	/* assert */
/*----------------------------------------------------------------------------*/
/* custom - made headers*/
#include "sorted_list.h"	
#include "pqueue.h"	/*API*/

/*---------------------priority queue strucutre -----------------------------*/
struct pq
{
    sorted_list_ty* lst;
};
/*---------------------------implementations ---------------------------------*/
pq_ty* PQCreate(pq_cmp_ty cmp, const void* param)
{
    pq_ty* q = (pq_ty*)malloc(sizeof(pq_ty));
    if (NULL == q)
    {
        return NULL;
    }

    q->lst = SortedListCreate(cmp, param);
    if (NULL == q->lst)
    {
        free(q);
        q = NULL;
    }
    
    return q;
}
/*----------------------------------------------------------------------------*/
void PQDestroy(pq_ty* q)
{
    if (NULL == q)
    {
        return;
    }

    SortedListDestroy(q->lst);
    free(q);
    q = NULL;
}
/*----------------------------------------------------------------------------*/
size_t PQCount(const pq_ty* q)
{
    assert(NULL != q);
    return SortedListCount(q->lst);
}
/*----------------------------------------------------------------------------*/
pq_bool_ty PQIsEmpty(const pq_ty* q)
{
    assert(NULL != q);
    return (pq_bool_ty)SortedListIsEmpty(q->lst);
}
/*----------------------------------------------------------------------------*/
pq_status_ty PQEnqueue(pq_ty* q, void* data)
{
    sorted_list_iter_ty iter;
    
    assert(NULL != q);
    
    /* iter points to the newely inserted node (if PQ_SUCCESS) or EndIter (if PQ_PQ_FAILd) */
    iter = SortedListInsert(q->lst, data);
    if (SortedListIterIsEqual(iter, SortedListEndIter(q->lst)))
    {
        return PQ_FAIL;
    }

    return PQ_SUCCESS;
}
/*----------------------------------------------------------------------------*/
void* PQDequeue(pq_ty* q)
{
    void* data = NULL;
    
    assert(NULL != q);
    assert(!PQIsEmpty(q));

    data = SortedListIterGetData(SortedListBeginIter(q->lst));
    SortedListRemove(SortedListBeginIter(q->lst));

    return data;
}
/*----------------------------------------------------------------------------*/
void* PQPeek(const pq_ty* q)
{
    assert(NULL != q);
    assert(!PQIsEmpty(q));

    return SortedListIterGetData(SortedListBeginIter(q->lst));
}
/*----------------------------------------------------------------------------*/
void* PQRemove(pq_ty* q, pq_is_match_ty is_match, const void* param)
{
    sorted_list_iter_ty iter_to_rmv = {0};
    void* data = NULL;

    assert(NULL != q);
    assert(NULL != is_match);

    /* Search the sorted list for the specific element */
    iter_to_rmv = SortedListFindIf(SortedListBeginIter(q->lst), 
                                  SortedListEndIter(q->lst), is_match, param);

    /* If we didn't find it, return NULL */
    if (SortedListIterIsEqual(iter_to_rmv, SortedListEndIter(q->lst)))
    {
        return NULL;
    }

    data = SortedListIterGetData(iter_to_rmv);
    SortedListRemove(iter_to_rmv);

    return data;
}
