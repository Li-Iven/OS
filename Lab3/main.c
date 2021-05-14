#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
	int **mat;
	int order;
} matrix;

typedef struct {
	matrix *m;
	matrix *new_m;
	int begin;
	int end;
	int window_size;
} Args;

int max(int a, int b){
	return (a > b ? a : b);
}

int min(int a, int b){
	return (a < b ? a : b);
}

void sort(int *arr, int arr_size){
	for (int i = 0; i < arr_size; i++){
		for (int j = arr_size - 1; j > 1; j--){
			if (arr[j - 1] > arr[j]) {
				int tmp = arr[j - 1];
				arr[j - 1] = arr[j];
				arr[j] = tmp;
			}
		}
	}
}

void *Median_Filtr(void *token) {
	Args *args = (Args*)token;
	int l = args->begin;
	int r = args->end;
	int ws = args->window_size;
	matrix *M = args->m;
	matrix *new_M = args->new_m;
	int *tmp = (int*) malloc(sizeof(int) * (2 * ws + 1) * (2 * ws + 1));
	for (int i = l; i < r; ++i) {
		for (int j = 0; j < M->order; ++j) {
			int tmp_index = 0;
			for (int a = max(0, i - ws); a < min(M->order, i + ws); ++a) {
				for (int b = max(0, j - ws); b < min(M->order, j + ws); ++b) {
					tmp[tmp_index] = M->mat[a][b];
					++tmp_index;
				}
			}
			sort(tmp, tmp_index);
			new_M->mat[i][j] = tmp[(tmp_index) / 2];
		}
	}
	free(tmp);
	return NULL;
}

void matrix_init(matrix *m, int n){
	m->order = n;
	m->mat = (int**)malloc(n * sizeof(int*));
	if (m->mat == NULL){
		printf("Can't allocate memory!\n");
		exit(2);
	}
	for (int i = 0; i < n; ++i){
		m->mat[i] = (int*)malloc(n * sizeof(int));
		if (m->mat[i] == NULL){
			printf("Can't allocate memory!\n");
			exit(2);
		}
	}
}

void matrix_input(matrix *m){
	for (int i = 0; i < m->order; ++i){
		for (int j = 0; j < m->order; ++j){
			scanf("%d", &m->mat[i][j]);
		}
	}
}

void matrix_delete(matrix *m){
	for (int i = 0; i < m->order; ++i){
		free(m->mat[i]);
	}
	free(m->mat);
}

int main(int argc, char **argv){
	if (argc != 2){
		printf("No argument for main function!\n");
		exit(1);
	}

	int thread_amount = atoi(argv[1]);
	if (thread_amount < 0){
		printf("Number of threads must be greater than zero");
	}

	int n;
	matrix M, new_M;
	printf("Enter size of matrix: ");
	scanf("%d", &n);
	matrix_init(&M, n);
	matrix_input(&M);
	matrix_init(&new_M, n);

	int ws;
	printf("Enter window size: ");
	scanf("%d", &ws);

	int K;
	printf("Enter amount of median filters: ");
	scanf("%d", &K);

	if (thread_amount > M.order){
		thread_amount = M.order;
	}

	Args arr[thread_amount];
	pthread_t threads[thread_amount];
	int strings_per_thread = (M.order + 1)/ thread_amount;
	for (int i = 0; i < thread_amount; ++i) {
		int l = strings_per_thread * i;
		int r = min(M.order, strings_per_thread * (i + 1));
		arr[i].m = &M;
		arr[i].new_m = &new_M;
		arr[i].begin = l;
		arr[i].end = r;
		arr[i].window_size = ws;
		pthread_create(&threads[i],NULL,*Median_Filtr, (void*)&arr[i]);
	}
	// Ожидание потоков
	for (int i = 0; i < thread_amount; ++i) {
		pthread_join(threads[i],NULL);
	}
	// Вывод резульата
	for (int i = 0; i < n; ++i){
		for (int j = 0; j < n; ++j){
			printf("%d ", new_M.mat[i][j]);
		}
		printf("\n");
	}
	// Освобождаем память
	matrix_delete(&M);
	matrix_delete(&new_M);
	return 0;
}
