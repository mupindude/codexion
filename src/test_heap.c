#include "../codexion.h"

static void	print_request(t_request *request)
{
	printf("Coder %d | arrival: %ld | deadline: %ld\n",
		request->coder->id,
		request->arrival_time,
		request->deadline);
}

int	main(void)
{
	t_heap		heap;
	t_coder		coder1;
	t_coder		coder2;
	t_coder		coder3;
	t_coder		coder4;

	t_request	req1;
	t_request	req2;
	t_request	req3;
	t_request	req4;

	coder1.id = 1;
	coder2.id = 2;
	coder3.id = 3;
	coder4.id = 4;

	req1.coder = &coder1;
	req1.arrival_time = 40;
	req1.deadline = 100;

	req2.coder = &coder2;
	req2.arrival_time = 10;
	req2.deadline = 100;

	req3.coder = &coder3;
	req3.arrival_time = 30;
	req3.deadline = 200;

	req4.coder = &coder4;
	req4.arrival_time = 20;
	req4.deadline = 300;

	if (heap_init(&heap, 4, compare_edf))
		return (1);

	heap_push(&heap, &req1);
	heap_push(&heap, &req2);
	heap_push(&heap, &req3);
	heap_push(&heap, &req4);

	printf("PEEK:\n");
	print_request(heap_peek(&heap));

	printf("\nPOP ORDER:\n");
	while (heap.size > 0)
		print_request(heap_pop(&heap));

	// heap_destroy(&heap);
	return (0);
}