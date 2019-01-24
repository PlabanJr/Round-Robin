#include <iostream>
#include <ctime>
#include <stdlib.h>

#define INITIAL_PROCESS_COUNT 3
#define MAX_PROCESS_INCOMING_TIME 60
#define MIN_PROCESS_INCOMING_TIME 10
#define MAX_CPU_LIFE_TIME 15000
#define MIN_CPU_LIFE_TIME 500
#define MAX_TIME_QUANTUM 50
#define MIN_TIME_QUANTUM 10
#define PROCESS_INCOMING_TIME (rand() % (MAX_PROCESS_INCOMING_TIME + 1 - MIN_PROCESS_INCOMING_TIME) + MIN_PROCESS_INCOMING_TIME) 
using namespace std;


struct gantt_chart
{
    int p_id,total_time;
};

struct process
{
    int p_id;
    int burst_time;
    struct process *next;
    struct process *prev;
};

void insert_process(struct process **);

void print_information(int CPU_TOTAL_TIME,int TIME_QUANTUM)
{
    cout<<endl;
    cout<<"* Initial process count   :   "<<INITIAL_PROCESS_COUNT<<endl;
    cout<<"* Total CPU lifetime      :   "<<CPU_TOTAL_TIME<<endl;
    cout<<"* Process incoming time   :   "<<PROCESS_INCOMING_TIME<<endl;
    cout<<"* Time quantum            :   "<<TIME_QUANTUM<<endl<<endl;
}

void delete_process(struct process **cpu)
{
    struct process *curr_process = *cpu;

    if(curr_process->next == *cpu)
    {
        *cpu = NULL;
    }
    else
    {
        *cpu = curr_process->next;
        curr_process->prev->next = *cpu;
        (*cpu)->prev = curr_process->prev;
    }
    delete curr_process;
}

void allot_cpu(process **cpu,int TIME_QUANTUM)
{
    if((*cpu)->burst_time <= TIME_QUANTUM)
    {
        delete_process(cpu);
    }
    else
    {
        (*cpu)->burst_time -= TIME_QUANTUM;
        (*cpu) = (*cpu)->next;
    }
}

int processing(process **cpu,gantt_chart *g_chart,int CPU_TOTAL_TIME,int TIME_QUANTUM)
{
    static int size;
    int timer = 0;
    do
    {   
        if((*cpu) != NULL)
        {   
            if((*cpu)->burst_time <= TIME_QUANTUM)
            {
                timer += (*cpu)->burst_time;
            }
            else
            {
                timer += TIME_QUANTUM;
            }

            if(timer >= PROCESS_INCOMING_TIME)
            {
                insert_process(cpu);
            }
        
            if(timer + (*cpu)->burst_time >= CPU_TOTAL_TIME)
            {
                break;
            }

            g_chart[size].p_id = (*cpu)->p_id;
            g_chart[size++].total_time = timer;

            allot_cpu(cpu,TIME_QUANTUM);
        }
        else
        {
            break;
        }
    }
    while(timer < CPU_TOTAL_TIME);
    return size;
}

void insert_process(struct process **cpu)
{
    struct process *new_process;
    new_process = new process;
    static int initial_p_id = 1;
    new_process->p_id = initial_p_id++;
    new_process->burst_time = rand() % 60 + 1;
    if(*cpu)
    {
        new_process->next = *cpu;
        new_process->prev = (*cpu)->prev;
        (*cpu)->prev->next = new_process;
        (*cpu)->prev = new_process;
    }   
    else
    {
        *cpu = new_process;
        (*cpu)->next = new_process;
        (*cpu)->prev = new_process;
    }
    
}

process *initial_process_queue()
{
    process *head = NULL;
    srand(time(NULL));

    for(int i = 0;i < INITIAL_PROCESS_COUNT;i++)
    {
        insert_process(&head);
    }
    return head;
}

void print_g_chart(gantt_chart *g_chart,int size)
{
    cout<<"<---  GANTT CHART  --->"<<endl<<endl;
    cout<<"-------------------------"<<endl;
    cout<<" Process_ID     |"<<" Time"<<endl;
    cout<<"-------------------------"<<endl;
    for(int i=0;i<size;i++)
    {
        cout<<"    "<<g_chart[i].p_id<<"\t\t|  "<<g_chart[i].total_time<<"      "<<endl;
    }
        
    cout<<" -------------------------"<<endl;
}

int main()
{
    system("clear");
    srand(time(NULL));
    int CPU_TOTAL_TIME = (rand() % (MAX_CPU_LIFE_TIME + 1 - MIN_CPU_LIFE_TIME) + MIN_CPU_LIFE_TIME); 
    int TIME_QUANTUM = (rand() % (MAX_TIME_QUANTUM + 1 - MIN_TIME_QUANTUM) + MIN_TIME_QUANTUM); 


    struct process *cpu = initial_process_queue();

    print_information(CPU_TOTAL_TIME,TIME_QUANTUM);

    gantt_chart *g_chart = new gantt_chart[2 * (CPU_TOTAL_TIME / TIME_QUANTUM)];
    int size = processing(&cpu,g_chart,CPU_TOTAL_TIME,TIME_QUANTUM);


    print_g_chart(g_chart,size);

    cout<<"\n------------END--------------"<<endl<<endl;

    return 0;
}