#include <prototypes.h>

#define N		5
#define LEFT		(i-1) % N
#define RIGHT		(i+1) % N
#define THINKING	0
#define HUNGRY		1
#define EATING		2

volatile int32_t state[N];
semaphore mtx;
semaphore s[N];

void test(int i){
	if (state[i] == HUNGRY && state[LEFT] != EATING && state[RIGHT] != EATING){
		state[i] = EATING;
		HF_SemPost(&s[i]);
	}
}

void take_forks(int i){
	HF_SemWait(&mtx);
	state[i] = HUNGRY;
	test(i);
	HF_SemPost(&mtx);
	HF_SemWait(&s[i]);
}

void put_forks(int i){
	HF_SemWait(&mtx);
	state[i] = THINKING;
	test(LEFT);
	test(RIGHT);
	HF_SemPost(&mtx);
}

void think(void){
	static int32_t c = 0;

	printf("(%d) philosopher %d thinking...\n", c++, HF_CurrentTaskId());
}

void eat(void){
	static int32_t c = 0;

	printf("(%d) philosopher %d eating...\n", c++, HF_CurrentTaskId());
}

void philosopher(void){
	int32_t i;

	i = HF_CurrentTaskId() - 1;

	while (1){
		think();
		take_forks(i);
		eat();
		put_forks(i);
	}
}

void ApplicationMain(void){
	int32_t i;
	int8_t buf[30], buf2[20];

	HF_SemInit(&mtx, 1);
	for (i = 0; i < N; i++)
		HF_SemInit(&s[i], 0);

	for (i = 0; i < N; i++){
		strcpy(buf, "phi_");
		itoa(i, buf2, 10);
		strcat(buf, buf2);
		HF_AddTask(philosopher, buf, 2048, 30, 0);
	}

	HF_Start();
}

