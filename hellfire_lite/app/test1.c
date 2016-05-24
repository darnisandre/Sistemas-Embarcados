#include <prototypes.h>

void task(void){
        int32_t tid;

        tid = HF_CurrentTaskId();
        for (;;){
              //printf("T%d", tid);
        }
}

void ApplicationMain(void){

HF_AddPeriodicTask( task ,10,1,10 , "T1" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,18,7,18 , "T2" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task ,21,2,21 , "T3" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task ,21,4,21 , "T4" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task ,16,3,16 , "T5" , 2048 , 0 , 0 ) ;

HF_Start();

}
