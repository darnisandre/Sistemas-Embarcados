#include <prototypes.h>

void task(void){
        int32_t tid;

        tid = HF_CurrentTaskId();
        for (;;){
                //printf("T%d", tid);
        }
}

void ApplicationMain(void){

HF_AddPeriodicTask( task,8,2,8 , "T1" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,15,5,15 , "T2" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,18,3,18  , "T3" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,25,6,25 , "T4" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,20,6,20 , "T5" , 2048 , 0 , 0 ) ;

HF_Start();

}
