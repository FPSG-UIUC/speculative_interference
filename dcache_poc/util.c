
#include "util.h"

int list_length(struct Node *head)
{
	int len = 0;
	while (head != NULL) {
		head = head->next;
		len += 1;
	}
	return len;
}

/* Appends the addr to the end of the list */
void list_append(struct Node **head, void *addr)
{
	struct Node *current = *head;

	// Create the new node to append to the linked list
	struct Node *new_node = malloc(sizeof(*new_node));
	new_node->address = addr;
	new_node->next = NULL;

	// If the linked list is empty, just make the head to be this new node
	if (current == NULL)
		*head = new_node;

	// Otherwise, go till the last node and append the new node after it
	else {
		while (current->next != NULL)
			current = current->next;

		current->next = new_node;
	}
}

/* Add the addr to the head of the list */
void list_push(struct Node **head, void *addr)
{
	// Create the new head of the linked list
	struct Node *new_node = malloc(sizeof(*new_node));
	new_node->address = addr;
	new_node->next = *head;
	*head = new_node;
}

/* Removes and returns the first element of list */
void *list_pop(struct Node **head)
{
	struct Node *current = *head;
	if (current == NULL) {
		return NULL;
	}

	*head = current->next;
	return current->address;
}

/* concat chunk of elements to the end of the list */
void list_concat(struct Node **ptr, struct Node *chunk)
{
	struct Node *tmp = *ptr;
	if (!tmp) {
		*ptr = chunk;
		return;
	}

	while (tmp->next != NULL) {
		tmp = tmp->next;
	}

	tmp->next = chunk;
}

void list_from_chunks(struct Node **ptr, struct Node **chunks, int avoid, int len)
{
	int next = (avoid + 1) % len;
	if (!(*ptr) || !chunks || !chunks[next])
	{
		return;
	}
	// Disconnect avoided chunk
	struct Node *tmp = chunks[avoid];
	while (tmp && tmp->next != NULL && tmp->next != chunks[next])
	{
		tmp = tmp->next;
	}
	if (tmp)
	{
		tmp->next = NULL;
	}
	// Link rest starting from next
	tmp = *ptr = chunks[next];
	while (next != avoid && chunks[next] != NULL)
	{
		next = (next + 1) % len;
		while (tmp && tmp->next != NULL && tmp->next != chunks[next])
		{
			tmp = tmp->next;
		}
		if (tmp)
		{
			tmp->next = chunks[next];
		}
	}
	if (tmp)
	{
		tmp->next = NULL;
	}
}

void list_split(struct Node *ptr, struct Node **chunks, int n)
{
	if (!ptr) {
		return;
	}
	int len = list_length (ptr);
	int k = len / n;
	int i = 0, j = 0;

	while (j < n)
	{
		i = 0;
		chunks[j] = ptr;
		while (ptr != NULL && ((++i < k) || (j == n-1)))
		{
			ptr = ptr->next;
		}
		if (ptr)
		{
			struct Node *prev = ptr;
			ptr = ptr->next;
			if (ptr && prev) {
				prev->next = NULL;
			}
		}
		j++;
	}
}

void shuffle(int *array, size_t n)
{
	size_t i;
	if (n > 1)
	{
		for (i = 0; i < n - 1; i++)
		{
			size_t j = i + rand() / (RAND_MAX / (n - i) + 1);
			int t = array[j];
			array[j] = array[i];
			array[i] = t;
		}
	}
}

/*
 * The argument addr should be the physical address, but in some cases it can be
 * the virtual address and this will still work. Here is why.
 *
 * Normal pages are 4KB (2^12) in size, meaning that the rightmost 12 bits of
 * the virtual address have to be the same in the physical address since they
 * are needed as offset for page table in the translation.
 * 
 * Huge pages have a size of 2MB (2^21), meaning that the rightmost 21 bits of
 * the virtual address have to be the same in the physical address since they
 * are needed as offset for page table in the translation. 
 * 
 * Since to find the set in the L1 we only need bits [11-6], then the virtual
 * address, either with normal or huge pages, is enough to get the set index.
 * 
 * Since to find the set in the L2 and LLC we only need bits [15-6], then the
 * virtual address, with huge pages, is enough to get the set index.
 * 
 * To visually understand why, see the presentations here:
 *  https://cs.adelaide.edu.au/~yval/Mastik/
 */
uint64_t get_cache_set_index(uint64_t addr, int cache_level)
{
	uint64_t index;

	if (cache_level == 1) {
		index = (addr)&L1_SET_INDEX_MASK;

	} else if (cache_level == 2) {
		index = (addr)&L2_SET_INDEX_MASK;

	} else if (cache_level == 3) {
		index = (addr)&LLC_SET_INDEX_PER_SLICE_MASK;

	} else {
		exit(EXIT_FAILURE);
	}

	return index >> CACHE_BLOCK_SIZE_LOG;
}