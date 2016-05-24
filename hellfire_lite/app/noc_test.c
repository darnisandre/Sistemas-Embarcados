#include <prototypes.h>

#define QUERY_TASK_ID	1
#define PARALLEL_QUERY	1

void task_send(void){
	uint32_t i, time, run_time, query_time;
	uint8_t buf[512] = "hello world from node 0! \\o/";
	int32_t id[N_CORES], tid, err;
	uint16_t cpu;

	run_time = MemoryRead(COUNTER_REG);

	for(i=0;i<N_CORES;i++)
		printf("\ncore %d, mapped to router %x", i, HF_Core(i));

	while(1){
#if QUERY_TASK_ID == 1
		query_time = MemoryRead(COUNTER_REG);
#if PARALLEL_QUERY == 0
		for (i = 0; i < N_CORES; i++){
			if (i != CPU_ID){
				id[i] = HF_QueryTaskId(HF_Core(i), "receiver");
				if (id[i] < 0){
					printf("\nerror: %d", id);
					HF_Panic(0);
				}else{
					printf("\ntask 'receiver' has id %d on core %d", id[i], i);
				}
			}
		}
#else
		for (i = 0; i < N_CORES; i++){
			if (i != CPU_ID){
				HF_NB_QueryTaskId(HF_Core(i), "receiver");
			}
		}

		for (i = 0; i < N_CORES; i++){
			if (i != CPU_ID){
				err = HF_NB_RecvTaskId(&cpu, &tid);
				if (err){
					printf("\nerror: %d", err);
					HF_Panic(0);
				}else{
					printf("\ntask 'receiver' has id %d on core %d", tid, cpu);
					id[cpu] = tid;
				}
			}
		}
#endif

		query_time = MemoryRead(COUNTER_REG) - query_time;
		printf("\n%d CPUs queried in %d ms\n", N_CORES-1, query_time / (CPU_SPEED / 1000));
#endif
		for (i = 0; i < N_CORES; i++){
			if (i != CPU_ID){
				time = MemoryRead(COUNTER_REG);
#if QUERY_TASK_ID == 1
				HF_Send(HF_Core(i), id[i], buf, sizeof(buf));
#else
				HF_Send(HF_Core(i), 2, buf, sizeof(buf));
#endif
				time = MemoryRead(COUNTER_REG) - time;
				printf("\nnode 0 to node %d -> %d cycles, %d Kbps", i, time, ((sizeof(buf) * 8 * 1000) / (time / (CPU_SPEED/1000000))));
			}
		}

		run_time = MemoryRead(COUNTER_REG) - run_time;

		printf("\ntime: %d ms\n", run_time / (CPU_SPEED / 1000));
		
		HF_Panic(0);
	}
}

void task_recv(void){
	uint16_t source_cpu;
	uint8_t source_id;
	uint16_t size;
	// messages coming from the other core are big and composed of several packets.
	uint8_t buf[4096];
	uint32_t time;

	while(1){
		while(HF_PacketsQueued(HF_CurrentTaskId()) == 0);
		time = MemoryRead(COUNTER_REG);
		HF_Receive(&source_cpu, &source_id, buf, &size);
		time = MemoryRead(COUNTER_REG) - time;
		printf("\nthread %d -> message from node %d thread %d size %d, time %d cycles -> %s", HF_CurrentTaskId(), source_cpu, source_id, size, time, buf);
		HF_Panic(0);
	}
}

void bogus(void){
	while(1){
	}
}

void ApplicationMain(void){
	int32_t i;

#if CPU_ID == 1
	HF_AddTask(task_send, "sender", 6000, 10, TASK_DEFAULT);
#else
#if QUERY_TASK_ID == 1
//	for (i = 1; i < CPU_ID; i++)
//		HF_AddTask(bogus, "bogus", 1024, 128, TASK_DEFAULT);
#endif
	HF_AddTask(task_recv, "receiver", 6000, 10, TASK_DEFAULT);
#endif

	HF_Start();
}

