#include <stdio.h>
#include <stdlib.h>

struct array_entry {
	int value;
	struct array_entry *next;
};

typedef struct array_entry *xarray;

xarray new_array(int first)
{
	xarray ar = malloc(sizeof(struct array_entry));
	ar->value = first;
	ar->next = NULL;
	return ar;
}

int sum_array(xarray ar)
{
	int sum = 0;
	
	while (ar != NULL) {
		sum += ar->value;
		ar = ar->next;
	}
	
	return sum;
}

void append_array(xarray ar, int val)
{
	while (ar->next != NULL) {
		ar = ar->next;
	}

	ar->next = malloc(sizeof(struct array_entry));
	ar->next->value = val;
	ar->next->next = NULL;
}

int delete_last(xarray ar)
{
	if (ar->next == NULL) {
		free(ar);
		return 1;
	}
	
	while (ar->next->next != NULL) {
		ar = ar->next;
	}

	free(ar->next);
	ar->next = NULL;

	return 0;
}

void delete_array(xarray ar)
{
	while (!delete_last(ar));
}

int main(int argc, char *argv[])
{
	xarray ar = new_array(0);
	unsigned int n = 0;
	int value;

	do {
		printf("Enter value #%u or 0 when done: ", ++n);
		scanf("%d", &value);
		if (value != 0) append_array(ar, value);
	} while (value != 0);

	printf("\nTotal: %d\n", sum_array(ar));

	delete_array(ar);
	return 0;
}
