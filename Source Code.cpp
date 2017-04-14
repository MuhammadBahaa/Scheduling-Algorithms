#include <iostream>
#include <stdio.h>
#include <algorithm>

using namespace std;

struct process
{
    int pid ;
    int flag ;
    int arrival_time ;
    int burst_time ;
    int waiting_time ;
    int turnaround_time ;
};

int numberofprocess;

void FCFS( void );
void SJF( void );
void SRTF( void );
void RR ( void) ;
void print_gantt_chart(process p[], int n );


int main()
{
    int choice ;

    cout <<"\n\t\t\t\t-------  Scheduling Algorithms  ------\n"
         << "\n\t1- First-Come, First-Served Scheduling (FCFS) \n"
         << "\n\t2- Shortest-Job-First Scheduling (SJF)\n"
         << "\n\t3- Shortest-Remaining - Time -First Scheduling(SRTF) \n"
         << "\n\t4- Round-Robin Scheduling (RR)\n"
         << "\n\t Please enter what algorithm you want to use :\t";

    cin >> choice ;


    switch ( choice )
    {
        case 1 : FCFS();
              break ;
        case 2 : SJF();
              break ;
        case 3 : SRTF();
              break ;
        case 4 :RR();
              break ;
    }

    return 0;
}


void FCFS( void )
{
    int i , j ,numberofprocess , total_burst_time = 0 , index_ready_queue = 0 , index_running_queue = 0 , flag_ready_queue = 0,flag_running_queue = 0 ;
    float  sum_turnaround_time = 0 , sum_waiting_time = 0 , total_turnaround_time = 0;
    process p [1000] , ready_queue[1000] , running_queue[1000];

    /*freopen ("SchedulingAlgorithms.txt","r",stdin);*/
    cout << "\n\tHow many processes:  " ;
    cin >> numberofprocess ;
    cout << "\n\tPlease enter  arrival time and burst time of Processes\n";
    for (  int j = 0 ; j < numberofprocess ; j++)
    {
      cout << "\tP " << j+1 << "  : ";
      cin >> p[j].arrival_time ;
      cin >> p[j].burst_time;
      p[j].pid = j+1;
      p[j].flag = 0;
      total_burst_time += p[j].burst_time;
    }

    for ( int time = 0 ; time < total_burst_time ; time++ )
     {
       if ( sum_turnaround_time == time) flag_running_queue = 0 ;
       for( i = 0 ; i <  numberofprocess; i++)
        {
          if(p[i].arrival_time == time)
          {
              if( flag_running_queue == 0 and flag_ready_queue == 0)/* check if the schedule is free */
               {
                 flag_running_queue = 1;
                 sum_turnaround_time += p[i].burst_time;
                 running_queue[index_running_queue].arrival_time = p[i].arrival_time;
                 running_queue[index_running_queue].burst_time = p[i].burst_time;
                 running_queue[index_running_queue].turnaround_time = sum_turnaround_time;
                 running_queue[index_running_queue].waiting_time = running_queue[index_running_queue].turnaround_time - running_queue[index_running_queue].burst_time - running_queue[index_running_queue].arrival_time;
                 running_queue[index_running_queue++].pid = p[i].pid;
               }
               else
               {
                 flag_ready_queue++ ;
                 ready_queue[index_ready_queue].arrival_time = p[i].arrival_time;
                 ready_queue[index_ready_queue].burst_time = p[i].burst_time;
                 ready_queue[index_ready_queue++].pid = p[i].pid;
               }
          }
          else
          {
            if ( flag_running_queue == 0 and flag_ready_queue > 1 and i == numberofprocess-1) /* check if ready queue is empty */
            {
             for ( j = 0 ; j < index_ready_queue ; j++)
             {
                 if ( ready_queue[j].flag ==0)
                    {
                     flag_ready_queue-- ;
                     flag_running_queue = 1;
                     ready_queue[j].flag = 1;
                     sum_turnaround_time += ready_queue[j].burst_time;
                     running_queue[index_running_queue].arrival_time = ready_queue[j].arrival_time;
                     running_queue[index_running_queue].burst_time = ready_queue[j].burst_time;
                     running_queue[index_running_queue].turnaround_time = sum_turnaround_time ;
                     running_queue[index_running_queue].waiting_time = running_queue[index_running_queue].turnaround_time - running_queue[index_running_queue].burst_time - running_queue[index_running_queue].arrival_time;
                     running_queue[index_running_queue++].pid = ready_queue[j].pid;
                    }
              }
            }
              else  if ( flag_running_queue == 0 and flag_ready_queue == 0 and i ==  numberofprocess-1) /* schedule in ideal case */
              {
                if ( running_queue[index_running_queue-1].pid ==0)
                 {
                   total_burst_time++;
                   sum_turnaround_time += 1;
                   running_queue[index_running_queue-1].arrival_time = time ;
                   running_queue[index_running_queue-1].burst_time = time+1;
                   running_queue[index_running_queue-1].turnaround_time = sum_turnaround_time ;
                 }
                 else
                  {
                   total_burst_time++;
                   sum_turnaround_time += 1;
                   running_queue[index_running_queue].arrival_time = time ;
                   running_queue[index_running_queue].burst_time = time+1;
                   running_queue[index_running_queue].turnaround_time = sum_turnaround_time ;
                   running_queue[index_running_queue].waiting_time = 0 ;
                   running_queue[index_running_queue++].pid = 0 ;
                 }
              }
          }
        }

   }

    for( int i = 0; i< index_running_queue; i++)
     {
        sum_waiting_time += running_queue[i].waiting_time ;
        if ( running_queue[i].pid != 0 )
        total_turnaround_time += running_queue[i].turnaround_time;
     }

    cout << "\n\n\tThe Average Waiting Time  :  " << sum_waiting_time / numberofprocess << endl;
    cout << "\tThe Average Turnaround Time : " << total_turnaround_time / numberofprocess<< endl ;
    cout << "\tSequence of processes : ";
    for ( int l = 0 ; l < index_running_queue ; l++)
        if (running_queue[l].pid == 0)
          cout << "GAP" << "  " ;
        else
          cout << "P" << running_queue[l].pid << " ";
    cout << "\n\tGantt chart \n";
    print_gantt_chart( running_queue, index_running_queue);
 }

void SJF( void )
{
    int i ,j , numberofprocess , total_burst_time = 0 , index_ready_queue = 0 , index_running_queue = 0 , flag_ready_queue = 0,flag_running_queue = 0 ;
    float  sum_turnaround_time = 0 , sum_waiting_time = 0 , total_turnaround_time = 0;
    process p [1000] , ready_queue[1000] , running_queue[1000];

    /*freopen ("SchedulingAlgorithms.txt","r",stdin);*/
    cout << "\n\tHow many processes:  " ;
    cin >> numberofprocess ;
    cout << "\n\tPlease enter  arrival time and burst time of Processes\n";
    for (  int j = 0 ; j < numberofprocess ; j++)
    {
      cout << "\tP " << j+1 << "  : ";
      cin >> p[j].arrival_time ;
      cin >> p[j].burst_time;
      p[j].pid = j+1;
      p[j].flag = 0;
      total_burst_time += p[j].burst_time;
    }

    for ( int time = 0  ; time < total_burst_time ; time++ )
     {
       if ( sum_turnaround_time == time) flag_running_queue = 0 ;

       for( i = 0 ; i <  numberofprocess ; i++)
        {
          if(p[i].arrival_time == time)
          {
             flag_ready_queue++ ;
             ready_queue[index_ready_queue].arrival_time = p[i].arrival_time;
             ready_queue[index_ready_queue].burst_time = p[i].burst_time;
             ready_queue[index_ready_queue++].pid = p[i].pid;
          }
          else
          {
            if ( flag_running_queue == 0 and flag_ready_queue > 0  )
            {

             int shortest = 100000 , shortest_index = -1 ;

             for (  j = 0 ; j < index_ready_queue ; j++)
                     if ( ready_queue[j].flag == 0 and shortest > ready_queue[j].burst_time )
                     {
                      shortest = ready_queue[j].burst_time ;
                      shortest_index = j ;
                     }

             flag_ready_queue-- ;
             flag_running_queue = 1;
             ready_queue[shortest_index].flag = 1 ;
             sum_turnaround_time += ready_queue[shortest_index].burst_time;
             running_queue[index_running_queue].arrival_time = ready_queue[shortest_index].arrival_time;
             running_queue[index_running_queue].burst_time = ready_queue[shortest_index].burst_time;
             running_queue[index_running_queue].turnaround_time = sum_turnaround_time ;
             running_queue[index_running_queue].waiting_time = running_queue[index_running_queue].turnaround_time - running_queue[index_running_queue].burst_time - running_queue[index_running_queue].arrival_time;
             running_queue[index_running_queue++].pid = ready_queue[shortest_index].pid;
            }
            else  if ( flag_running_queue == 0 and flag_ready_queue == 0 and i ==  numberofprocess-1) /* ideal case */
              {
                if ( running_queue[index_running_queue-1].pid ==0)
                 {
                   total_burst_time++;
                   sum_turnaround_time += 1;
                   running_queue[index_running_queue-1].arrival_time = time ;
                   running_queue[index_running_queue-1].burst_time = time+1;
                   running_queue[index_running_queue-1].turnaround_time = sum_turnaround_time ;
                 }
                 else
                  {
                   total_burst_time++;
                   sum_turnaround_time += 1;
                   running_queue[index_running_queue].arrival_time = time ;
                   running_queue[index_running_queue].burst_time = time+1;
                   running_queue[index_running_queue].turnaround_time = sum_turnaround_time  ;
                   running_queue[index_running_queue].waiting_time = 0 ;
                   running_queue[index_running_queue++].pid = 0 ;
                 }
              }
          }
        }

   }

    for( int i = 0; i< index_running_queue; i++)
     {
        sum_waiting_time += running_queue[i].waiting_time ;
        if ( running_queue[i].pid != 0 )
        total_turnaround_time += running_queue[i].turnaround_time;
     }

    cout << "\n\n\tThe Average Waiting Time  :  " << sum_waiting_time / numberofprocess << endl;
    cout << "\tThe Average Turnaround Time : " << total_turnaround_time / numberofprocess<< endl ;
    cout << "\tSequence of processes : ";
    for ( int l = 0 ; l < index_running_queue ; l++)
        if (running_queue[l].pid == 0)
          cout << "GAP" << "  " ;
        else
          cout << "P" << running_queue[l].pid << " ";
    cout << "\n\tGantt chart \n";
    print_gantt_chart( running_queue, index_running_queue );
 }

void SRTF( void )
{
    int j , time , finish , cur_burst = 0 , flag_ready_queue = 0 , flag_running_queue = 0 ,  total_burst_time = 0 , index_ready_queue = 0 , index_running_queue = 0 , save_burst_time [1000];
    float sum_waiting_time = 0, sum_turnaround_time = 0 ;
    process p[1000] , running_queue[1000] , ready_queue[1000] ;

   /* freopen ("SchedulingAlgorithms.txt","r",stdin);*/
    cout << "\n\tHow many processes:  " ;
    cin >> numberofprocess ;
    cout << "\n\tPlease enter  arrival time and burst time of Processes\n";
    for ( int  i = 0 ; i < numberofprocess ; i++)
    {
     cout << "\tP " << i+1 << "  : ";
     cin >> p[i].arrival_time ;
     cin >> p[i].burst_time;
     p[i].pid = i+1;
     p[i].flag = 0;
     total_burst_time += p[i].burst_time;
     save_burst_time[i] = p[i].burst_time;
    }

    finish = numberofprocess ;

    for(int time = 0  ; time <= total_burst_time; time++ )
    {
       if (cur_burst == time) flag_running_queue = 0 ;

       for(int i = 0; i < numberofprocess ; i++)
       {
          if ( p[i].arrival_time == time )
          {
             finish--;
                if ( flag_running_queue == 1 ) /* check shortest process */
                 {
                     if ( running_queue[index_running_queue-1].burst_time > p[i].burst_time )
                     {
                         cur_burst -=running_queue[index_running_queue-1].burst_time;
                         int last_cur_burst = cur_burst;
                         cur_burst += (time-last_cur_burst);

                         flag_ready_queue++;
                         ready_queue[index_ready_queue].pid = running_queue[index_running_queue-1].pid ;
                         ready_queue[index_ready_queue].arrival_time = running_queue[index_running_queue-1].arrival_time;
                         ready_queue[index_ready_queue++].burst_time = running_queue[index_running_queue-1].burst_time - (time-last_cur_burst)  ;

                         running_queue[index_running_queue-1].turnaround_time = cur_burst ;

                         cur_burst +=p[i].burst_time;
                         flag_running_queue = 1;
                         running_queue[index_running_queue].pid = p[i].pid;
                         running_queue[index_running_queue].arrival_time = p[i].arrival_time;
                         running_queue[index_running_queue].burst_time = p[i].burst_time;
                         running_queue[index_running_queue++].turnaround_time = cur_burst;
                       }
                     else
                       {
                        flag_ready_queue++;
                        ready_queue[index_ready_queue].pid = p[i].pid;
                        ready_queue[index_ready_queue].arrival_time = p[i].arrival_time ;
                        ready_queue[index_ready_queue++].burst_time = p[i].burst_time ;
                       }
                 }
                else
                 {
                    if (flag_running_queue == 0 and flag_ready_queue == 0)
                     {
                         flag_running_queue = 1;
                         cur_burst += p[i].burst_time;
                         running_queue[index_running_queue].pid = p[i].pid;
                         running_queue[index_running_queue].arrival_time = p[i].arrival_time;
                         running_queue[index_running_queue].burst_time = p[i].burst_time;
                         running_queue[index_running_queue++].turnaround_time = cur_burst;
                      }
                    else
                     {
                        flag_ready_queue++;
                        ready_queue[index_ready_queue].pid = p[i].pid;
                        ready_queue[index_ready_queue].arrival_time = p[i].arrival_time ;
                        ready_queue[index_ready_queue++].burst_time = p[i].burst_time ;

                        int shortest = 100000 , shortest_index = -1 ;

                        for (  j = 0 ; j < index_ready_queue ; j++)
                        if ( ready_queue[j].flag == 0 and shortest > ready_queue[j].burst_time )
                         {
                           shortest = ready_queue[j].burst_time ;
                           shortest_index = j ;
                         }

                        flag_ready_queue-- ;
                        flag_running_queue = 1;
                        ready_queue[shortest_index].flag = 1 ;
                        cur_burst+=ready_queue[shortest_index].burst_time;
                        running_queue[index_running_queue].pid = ready_queue[shortest_index].pid ;
                        running_queue[index_running_queue].burst_time = ready_queue[shortest_index].burst_time ;
                        running_queue[index_running_queue].turnaround_time = cur_burst ;
                        running_queue[index_running_queue++].arrival_time = ready_queue[shortest_index].arrival_time;
                     }
                }
          }
          else
          {
              if ( flag_running_queue == 0 and flag_ready_queue > 0 and i == numberofprocess-1)
              {
                   int shortest = 100000 , shortest_index = -1 ;

                   for (  j = 0 ; j < index_ready_queue ; j++)
                     if ( ready_queue[j].flag == 0 and shortest > ready_queue[j].burst_time )
                      {
                       shortest = ready_queue[j].burst_time ;
                       shortest_index = j ;
                      }

                       flag_ready_queue-- ;
                       flag_running_queue = 1;
                       ready_queue[shortest_index].flag = 1 ;
                       cur_burst+=ready_queue[shortest_index].burst_time;
                       running_queue[index_running_queue].pid = ready_queue[shortest_index].pid ;
                       running_queue[index_running_queue].burst_time = ready_queue[shortest_index].burst_time ;
                       running_queue[index_running_queue].turnaround_time = cur_burst ;
                       running_queue[index_running_queue++].arrival_time = ready_queue[shortest_index].arrival_time;

              }
              else  if ( flag_running_queue == 0 and flag_ready_queue == 0 and i ==  numberofprocess-1 and finish !=0 )
              {
                if ( running_queue[index_running_queue-1].pid ==0)
                 {
                   total_burst_time++;
                   cur_burst+= 1;
                   running_queue[index_running_queue-1].arrival_time = time ;
                   running_queue[index_running_queue-1].burst_time = time+1;
                   running_queue[index_running_queue-1].turnaround_time = cur_burst;
                 }
                 else
                  {
                   total_burst_time++;
                   cur_burst+= 1;
                   running_queue[index_running_queue].arrival_time = time ;
                   running_queue[index_running_queue].burst_time = time+1;
                   running_queue[index_running_queue].turnaround_time = cur_burst  ;
                   running_queue[index_running_queue].waiting_time = 0 ;
                   running_queue[index_running_queue++].pid = 0 ;
                 }
              }
          }
       }
    }

    for( int i = 0; i < numberofprocess; i++)
    {
         for ( int j = index_running_queue ; j >= 0 ; j--)
             if ( p[i].pid == running_queue[j].pid)
             {
                p[i].turnaround_time = running_queue[j].turnaround_time;
                p[i].waiting_time = running_queue[j].turnaround_time - save_burst_time[i] - p[i].arrival_time;
                break ;
             }
    }

    for( int i = 0; i< numberofprocess; i++)
    {
        sum_waiting_time += p[i].waiting_time;
        sum_turnaround_time += p[i].turnaround_time;
    }

    cout << "\n\n\tThe Average Waiting Time  :  " << sum_waiting_time / numberofprocess << endl;
    cout << "\tThe Average Turnaround Time : " << sum_turnaround_time / numberofprocess<< endl ;
    cout << "\tSequence of processes : ";
    for ( int l = 0 ; l < index_running_queue ; l++)
        if (running_queue[l].pid == 0)
          cout << "GAP" << "  " ;
        else
          cout << "P" << running_queue[l].pid << " ";
    cout << "\n\tGantt chart \n";
    print_gantt_chart( running_queue, index_running_queue );
}

void RR ( void)
{
    int i ,j , c  = -1 ,finish , quantum_num , numberofprocess , total_burst_time = 0 , index_ready_queue = 0 , index_running_queue = 0 , flag_ready_queue = 0,flag_running_queue = 0 , save_burst_time[1000] ;
    float  sum_turnaround_time = 0 , sum_waiting_time = 0 , cur_burst = 0;
    process p [1000] , ready_queue[1000] , running_queue[1000] , temp ;

    /*freopen ("SchedulingAlgorithms.txt","r",stdin);*/
    cout << "\n\tHow many processes:  " ;
    cin >> numberofprocess ;
    cout << "\n\tEnter the Time Quantum : ";
    cin >> quantum_num;
    cout << "\n\tPlease enter  arrival time and burst time of Processes\n";
    for (  int j = 0 ; j < numberofprocess ; j++)
    {
      cout << "\tP " << j+1 << "  : ";
      cin >> p[j].arrival_time ;
      cin >> p[j].burst_time;
      p[j].pid = j+1;
      p[j].flag = 0;
      total_burst_time += p[j].burst_time;
      save_burst_time[j] = p[j].burst_time ;
    }

    finish = numberofprocess ;
    temp.flag = 0;

    for ( int time = 0  ; time < total_burst_time ; time++ )
     {
        if (cur_burst == time) flag_running_queue = 0 ;

        if ( running_queue[index_running_queue-1].burst_time > quantum_num and cur_burst == time and temp.flag == 0 )
         {
           temp.flag = 1 ;
           temp.pid = running_queue[index_running_queue-1].pid;
           temp.arrival_time = cur_burst ;
           temp.burst_time = running_queue[index_running_queue-1].burst_time - quantum_num ;
         }


                if( temp.flag == 1)
                    {
                     temp.flag = 0 ;
                     flag_ready_queue++;
                     ready_queue[index_ready_queue].pid = temp.pid;
                     ready_queue[index_ready_queue].arrival_time = temp.arrival_time;
                     ready_queue[index_ready_queue++].burst_time = temp.burst_time;
                     }

       for( i = 0 ; i <  numberofprocess; i++)
        {
          if(p[i].arrival_time == time)
          {
             finish --;

             flag_ready_queue++ ;
             ready_queue[index_ready_queue].arrival_time = p[i].arrival_time;
             ready_queue[index_ready_queue].burst_time = p[i].burst_time;
             ready_queue[index_ready_queue++].pid = p[i].pid;


             if( flag_running_queue == 0 )
               {
                   c++;
                   if ( ready_queue[c].burst_time > quantum_num)
                   {
                    flag_running_queue = 1;
                    flag_ready_queue-- ;
                    cur_burst += quantum_num ;
                    running_queue[index_running_queue].burst_time = ready_queue[c].burst_time;
                    ready_queue[c].burst_time -= quantum_num ;
                    running_queue[index_running_queue].pid = ready_queue[c].pid ;
                    running_queue[index_running_queue++].turnaround_time = cur_burst ;
                   }
                   else
                   {
                    flag_running_queue = 1;
                    flag_ready_queue-- ;
                    cur_burst += ready_queue[c].burst_time;
                    ready_queue[c].burst_time = 0 ;
                    running_queue[index_running_queue].pid = ready_queue[c].pid ;
                    running_queue[index_running_queue++].turnaround_time= cur_burst ;
                    }
               }

          }
          else
          {

            if ( flag_running_queue == 0 and flag_ready_queue > 0  and i ==  numberofprocess-1)
            {

                   c++;
                   if ( ready_queue[c].burst_time > quantum_num)
                   {
                    flag_running_queue = 1;
                    flag_ready_queue-- ;
                    cur_burst += quantum_num ;
                    running_queue[index_running_queue].burst_time = ready_queue[c].burst_time;
                    ready_queue[c].burst_time -= quantum_num ;
                    running_queue[index_running_queue].pid = ready_queue[c].pid ;
                    running_queue[index_running_queue++].turnaround_time = cur_burst ;
                   }
                   else
                   {
                    flag_running_queue = 1;
                    flag_ready_queue-- ;
                    cur_burst += ready_queue[c].burst_time;
                    ready_queue[c].burst_time = 0 ;
                    running_queue[index_running_queue].pid = ready_queue[c].pid ;
                    running_queue[index_running_queue++].turnaround_time= cur_burst ;
                    }

             }
            else  if ( flag_running_queue == 0 and flag_ready_queue == 0 and i ==  numberofprocess-1 and finish !=0 )
            {

                if ( running_queue[index_running_queue-1].pid ==0)
                 {
                   total_burst_time++;
                   cur_burst+= 1;
                   running_queue[index_running_queue-1].arrival_time = time ;
                   running_queue[index_running_queue-1].burst_time = time+1;
                   running_queue[index_running_queue-1].turnaround_time = cur_burst;
                 }
                 else
                  {
                   total_burst_time++;
                   cur_burst+= 1;
                   running_queue[index_running_queue].arrival_time = time ;
                   running_queue[index_running_queue].burst_time = time+1;
                   running_queue[index_running_queue].turnaround_time = cur_burst  ;
                   running_queue[index_running_queue].waiting_time = 0 ;
                   running_queue[index_running_queue++].pid = 0 ;
                 }
              }
           }
        }


   }

    for( int i = 0; i < numberofprocess; i++)
    {
         for ( int j = index_running_queue ; j >= 0 ; j--)
             if ( p[i].pid == running_queue[j].pid)
             {
                p[i].turnaround_time = running_queue[j].turnaround_time;
                p[i].waiting_time = running_queue[j].turnaround_time - save_burst_time[i] - p[i].arrival_time;
                break ;
             }
    }

    for( int i = 0; i< numberofprocess; i++)
    {
        sum_waiting_time += p[i].waiting_time;
        sum_turnaround_time += p[i].turnaround_time;
    }

    cout << "\n\n\tThe Average Waiting Time  :  " << sum_waiting_time / numberofprocess << endl;
    cout << "\tThe Average Turnaround Time : " << sum_turnaround_time /numberofprocess << endl ;
    cout << "\tSequence of processes : ";
    for ( int l = 0 ; l < index_running_queue ; l++)
        if (running_queue[l].pid == 0)
          cout << "GAP" << "  " ;
        else
          cout << "P" << running_queue[l].pid << " ";
    cout << "\n\tGantt chart \n";
    print_gantt_chart( running_queue, index_running_queue);
}

void print_gantt_chart(process p[], int n)
{
    int i , j;

    cout << " " ;
    for(i = 0 ; i < n ; i++)
    {
      for( j = 0 ; j < 4; j++) cout << "-" ;
    cout << " ";
    }

    cout << "\n|";

    for(i = 0; i < n ; i++)
    {
          for( j = 0; j <  1; j++) cout <<" ";

          if ( p[i].pid == 0) cout << "  " ;
          else cout << "P" <<  p[i].pid;

          for( j = 0 ; j < 1 ; j++) cout <<" ";

          cout << "|" ;
    }
    cout << "\n ";

    for(i = 0 ; i < n ; i++)
    {
        for( j = 0 ; j <4; j++) cout << "-" ;
        cout << " ";
    }
    cout << "\n" ;

    cout << p[0].arrival_time;

    for(i= 0 ; i < n ; i++)
    {
     for( j = 0 ; j < 4 ; j++) cout << " ";
          if(p[i].turnaround_time > 9) cout <<"\b";
     cout << p[i].turnaround_time;

    }
    cout <<"\n";

}

