#include <prototypes.h>

int32_t HF_ListInit(list_t *list){
	list->next = NULL;
	list->elem = NULL;
}

int32_t HF_ListAppend(list_t *list, void *item){
	list_t *t1, *t2;

	t1 = (list_t *)malloc(sizeof(list_t));
	if (t1){
		t1->elem = item;
		t1->next = NULL;
		t2 = list;

		while (t2->next != NULL)
			t2 = t2->next;

		t2->next = t1;

		return 0;
	}else{
		return -1;
	}
}

int32_t HF_ListInsert(list_t *list, void *item, int32_t pos){
	list_t *t1, *t2;
	int32_t i = 0;

	t1 = (list_t *)malloc(sizeof(list_t));
	if (t1){
		t1->elem = item;
		t1->next = NULL;
		t2 = list;

		while (t2->next != NULL){
			t2 = t2->next;
			if (++i == pos) break;
		}

		t1->next = t2->next;
		t2->next = t1;

		return 0;
	}else{
		return -1;
	}
}

int32_t HF_ListRemove(list_t *list, int32_t pos){
	list_t *t1, *t2;
	int32_t i = 0;

	t1 = list;
	t2 = t1;
	t1 = t1->next;
	while (t1 != NULL){
		if (i++ == pos){
			t2->next = t1->next;
			free(t1);
			return 0;
		}
		t2 = t1;
		t1 = t1->next;
	}
	return -1;
}

int32_t HF_ListMove(list_t *list int32_t src_pos, int32_t dst_pos){
}

void *HF_ListGet(list_t *list, int32_t pos){
	list_t *t1;
	int32_t i = 0;

	t1 = list;
	t1 = t1->next;
	while (t1 != NULL){
		if (i++ == pos)
			return (void *)t1->elem;
		t1 = t1->next;
	}
	return 0;
}

int32_t *HF_ListCount(list_t *list){
	list_t *t1;
	int32_t i = 0;

	t1 = list;
	t1 = t1->next;
	while (t1 != NULL){
		i++;
		t1 = t1->next;
	}
	return i;
}

