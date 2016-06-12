#include "common.h"

#define MEM_TAG 110
int g_number = 0;//链表的全局标号

LIST_ENTRY		my_list_head;
KSPIN_LOCK	my_list_lock;


void EventListInit()
{
	InitializeListHead(&my_list_head);//初始化链表头
	KeInitializeSpinLock(&my_list_lock);
}

NTSTATUS AddEventToList(PMY_EVENT pEvent)
{

	ExInterlockedInsertTailList(&my_list_head, pEvent, &my_list_lock);//从链表尾部插入
	return STATUS_SUCCESS;
}
PMY_EVENT RemoveEventFromList()//从链表头部删除,并返回删除的结构体
{
	PLIST_ENTRY pEntry;
	PMY_EVENT pEvent;
	pEntry=ExInterlockedRemoveHeadList(&my_list_head, &my_list_lock);//获取删除的链表地址
	pEvent = CONTAINING_RECORD(pEntry,MY_EVENT, list_enty);//从链表地址计算出MY_EVENT结构体地址

	return pEvent;
}

void ShowList()
{
	int i = 0;
	PLIST_ENTRY p;
	for (p = my_list_head.Flink; p != &my_list_head.Flink; p = p->Flink)
	{
		PMY_EVENT pEvent = CONTAINING_RECORD(p, MY_EVENT, list_enty);
		KdPrint(("i:%d,pEvent->type=%d", i++, pEvent->nType));
	}
}

void add()
{
	NTSTATUS Status;
	PMY_EVENT pEvent = (PMY_EVENT)ExAllocatePoolWithTag(PagedPool, sizeof(MY_EVENT), MEM_TAG);
	if (pEvent == NULL)
	{
		return STATUS_UNSUCCESSFUL;
	}


	pEvent->nType = g_number++;
	Status = AddEventToList(pEvent);
	return Status;
}
void remove()
{
	RemoveEventFromList();
}

