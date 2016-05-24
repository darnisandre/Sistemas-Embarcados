#include <prototypes.h>

mailbox mail, mail2, mail3, mail4;
semaphore wait;

void task0(void){
	int8_t buf[50], buf2[10];
	int32_t i, j;

	while(1){
		// send a string message to mailbox mail1
		for(i=0; i<5; i++){
			itoa(i, buf2, 10);
			strcpy(buf, "Hey, i can count => ");
			strcat(buf, buf2);
			HF_MboxSend(&mail, buf);
			HF_SemWait(&wait);			// let the other task take the message before we screw it! buf contains the message, but as
								// we pass a *pointer* to the message on the mailbox, at the time the other task read it, it
								// may be already modified (as it happens in the loop in this exemple).
		}

		// send a value message to mailbox mail2
		for(i=0; i<5; i++)				
			HF_MboxSend(&mail2, (void *)i);		// the problem does not happen here (we are passing a *copy* - it's just a variable
								// buried in a void pointer. we are using the pointer to hold actual data here.

		// send another string (directly) to mailbox mail3
		HF_MboxSend(&mail3, "hello world!");

		// send something to task 5
		HF_MboxSend(&mail4, (void *)12345);
	}
}

void task1(void){
	void *msg;

	while(1){
		msg = HF_MboxRecv(&mail);
		printf("\ntask 1: %s", msg);
		HF_SemPost(&wait);				// tell the sender we have consumed the message
	}
}

void task2(void){
	void *msg;

	while(1){
		msg = HF_MboxRecv(&mail2);
		printf("\ntask 2: Value received: %d", (int32_t)msg);
	}
}

void task3(void){
	void *msg;

	while(1){
		msg = HF_MboxRecv(&mail3);
		printf("\ntask 3: %s", msg);
	}
}

void task4(void){
	void *msg;

	while(1){
		msg = HF_MboxRecv(&mail2);
		printf("\ntask 4: same value as task 2: %d", (int)msg);
	}
}

void task5(void){
	void *msg;

	while(1){
		msg = HF_MboxAccept(&mail4);
		if (msg){
			printf("\ntask 5: just received a message! ==> %d", (int32_t)msg);
		}else{
			printf("\ntask 5: nothing for me.. =( i will wait a bit before checking again..");
			sleep(100);
		}
	}
}

void ApplicationMain(void){
	HF_AddTask(task0, "task 0", 2048, 50, 0);
	HF_AddTask(task1, "task 1", 2048, 50, 0);
	HF_AddTask(task2, "task 2", 2048, 50, 0);
	HF_AddTask(task3, "task 3", 2048, 50, 0);
	HF_AddTask(task4, "task 4", 2048, 50, 0);
	HF_AddTask(task5, "task 5", 2048, 50, 0);
	
	HF_MboxInit(&mail, 1);					// just task 1 will receive messages in this mailbox
	HF_MboxInit(&mail2, 2);					// tasks 2 and 4 will receive messages in this mailbox
	HF_MboxInit(&mail3, 1);					// just task 3 will receive messages from in this mailbox
	HF_MboxInit(&mail4, 1);					// just task 4 will receive messages from in this mailbox
	HF_SemInit(&wait, 0);

	HF_Start();
}

