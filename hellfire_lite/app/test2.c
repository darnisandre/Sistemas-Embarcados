#include <prototypes.h>

void task(void){
        int32_t tid;

        tid = HF_CurrentTaskId();
        for (;;){
                //printf("T%d", tid);
        }
}

void ApplicationMain(void){


HF_AddPeriodicTask( task,16,2,16 , "T1" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,20,5,20 , "T2" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,21,3,21  , "T3" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,30,5,30 , "T4" , 2048 , 0 , 0 ) ;
HF_AddPeriodicTask( task,20,4,20 , "T5" , 2048 , 0 , 0 ) ;

HF_Start();

}
