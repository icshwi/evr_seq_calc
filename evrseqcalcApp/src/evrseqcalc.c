#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <string.h> // Provides memcpy prototype
#include <stdlib.h> // Provides calloc prototype
#include <stdio.h>
#include <math.h>
//#include <create_tick_event_list.h>

int create_tick_event_list(float *freqs, int arr_len, float base_event_no, float RF_freq, float sys_freq, float sequence_freq, int *tick_list, int *event_list) {
  int i, j, k, total_events, num_of_events;
	float ticks_per_event, ticks_per_cycle;
	ticks_per_cycle = round(RF_freq / sys_freq* 1000000);
//	printf("%f", ticks_per_cycle);
	k = 0;

	for(i = 0; i < arr_len; i = i + 1) {
		if (freqs[i] > 0) {
			ticks_per_event = ticks_per_cycle * sys_freq / freqs[i];
//			printf("Calc num of events: %f, ticks_per_event %f, ticks_per_cycle %f\n", freqs[0]/sequence_freq, ticks_per_event, ticks_per_cycle);
			num_of_events = (int) round(freqs[i]/sequence_freq);
//			printf("No of events: %d\n", num_of_events);
			for(j = 0; j < num_of_events; j = j + 1) {
				event_list[k] = ((int) base_event_no) + i;
				tick_list[k] =  (int) round(j * ticks_per_event);
//				printf("Event %d: %d Tick: %d\n", k,  event_list[k], tick_list[k]);
				k = k + 1;
			}
		}
        }
  total_events = k;
//printf("Total events: %d\n", total_events);
  return total_events;

}

void apply_delay(float RF_freq, float sys_freq, float sequence_freq, int arr_len, int *delay_list, int *tick_list, int *event_list, float in_end_event_ticks) {
	int i, delay_index;
	float seq_len;
	seq_len = round(RF_freq / sequence_freq * 1000000);
	delay_index = 0;
	for (i = 0; i < arr_len; i = i + 1) {
//		printf("Apply delay: %f, %d\n", seq_len, delay_list[i]);
		if (event_list[i] == event_list[i+1]) {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
		}
		else {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
			delay_index = delay_index + 1;
		}
		if (tick_list[i] >= (int) (seq_len - in_end_event_ticks) && tick_list[i] <= (int)seq_len) {
			tick_list[i] = 0;
		}
		else if (tick_list[i] > seq_len) {
			tick_list[i] = tick_list[i] % (int) seq_len;
		}
//		printf("tick_list: %d max_ticks%d\n", tick_list[i], (int) (RF_freq/sequence_freq - in_end_event_ticks) );
	}	
	return ;
}

void sort_sequence(int arr_len, int *tick_list, int *event_list) {
	int i, j, tmp_tick, tmp_event;
	//Bubble sort	
	for (i = 0; i < arr_len - 1; i = i +1) {
		for (j = 0; j < arr_len - i - 1; j = j +1) {
			if (tick_list[j]> tick_list[j+1]) {
				tmp_tick = tick_list[j];
				tmp_event = event_list[j];
				tick_list[j] = tick_list[j+1];
				event_list[j] = event_list[j+1];
				tick_list[j+1] = tmp_tick;
				event_list[j+1] = tmp_event;
			}
		}
	}
	for (i = 0; i < arr_len; i = i + 1) {
//		printf("Event: %d, Tick: %d\n", event_list[i], tick_list[i]);
	}
}

void resolve_conflicts(float RF_freq, float sys_freq, float sequence_freq, int arr_len, int *tick_list, int *event_list, float in_end_event_ticks) {
	int i;
	float seq_len;
	seq_len =round(RF_freq / sequence_freq * 1000000);
	i = arr_len;
//	printf("arr_len = %d\n", arr_len);
	//Search backwards to have the events being arranged in an event number increasing order
	while (i > 0) {
		if (tick_list[i-1] == tick_list[i]) {
//			printf("event[i-1] = %d ticks[i-1] = %d event[i] = %d ticks[i] = %d i = %d\n", event_list[i-1], tick_list[i-1], event_list[i],  tick_list[i], i);
			tick_list[i] = tick_list[i] + 1;
			if (tick_list[i] > (int) (seq_len - in_end_event_ticks)) {
				tick_list[i] = 0;
			}
			sort_sequence(arr_len, tick_list, event_list);
			i = arr_len ;
		}
		i = i - 1;
	}
//	printf("Sorting output:\n");
	return ;
}
static int evr_sequence_calc(aSubRecord *precord) {
	
	float in_freqs[4], in_delays_ns[4], in_base_event_no, in_RF_freq, in_sys_freq, sequence_freq, lower_seq_freq, end_event_number, in_end_event_ticks;
	int delays_ticks[4], full_tick_list[2048], full_event_list[2048];
	int i, arr_len, total_events;
	lower_seq_freq = 12.0;
	end_event_number = 127.0;

	in_freqs[0] 		= *(float *)precord->a;
	in_freqs[1] 		= *(float *)precord->b;
	in_freqs[2] 		= *(float *)precord->c;
	in_freqs[3] 		= *(float *)precord->d;	
	in_delays_ns[0] 	= *(float *)precord->e;
	in_delays_ns[1] 	= *(float *)precord->f;
	in_delays_ns[2] 	= *(float *)precord->g;
	in_delays_ns[3] 	= *(float *)precord->h;
	in_base_event_no 	= *(float *)precord->i;
	in_RF_freq 		= *(float *)precord->j; //MHz
	in_sys_freq 		= *(float *)precord->k;
	in_end_event_ticks	= *(float *)precord->l + 1; //Add 1, arrays starts at 0
	
	for (i = 0; i < sizeof(in_freqs)/sizeof(in_freqs[0]); i = i +1) {
		//Convert ns to us by dividing by 1000, then multiply with MHz
		delays_ticks[i] = (int) round(in_delays_ns[i] / 1000.0 * in_RF_freq);
	}	
       
	arr_len = sizeof(in_freqs) / sizeof(in_freqs[0]);

	sequence_freq = in_sys_freq;
	for (i = 0; i < arr_len; i = i + 1) {
		if ( sequence_freq > in_freqs[i] && in_freqs[i] > 0.0) {
			sequence_freq = in_sys_freq/lower_seq_freq;
		}
	}

	//Create two arrays, one with all events and one with the corresponding tick timing
	total_events = create_tick_event_list(in_freqs, arr_len, in_base_event_no, in_RF_freq, in_sys_freq, sequence_freq, full_tick_list, full_event_list);

	//Reduce the array sizes and copy values
	int tick_list[total_events], event_list[total_events];
	//arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	for(i = 0; i < total_events; i = i + 1){
		tick_list[i] = full_tick_list[i];
		event_list[i] = full_event_list[i];
	}
//	printf("total_events: %d, arr_len: %d\n", total_events, arr_len);
	//Apply delay for each event in the tick array
	apply_delay(in_RF_freq, in_sys_freq, sequence_freq, total_events, delays_ticks, tick_list, event_list, in_end_event_ticks);

	//Sort the list after tick size
	sort_sequence(total_events, tick_list, event_list);

	//Ensure no events happen on the same tick count
	resolve_conflicts(in_RF_freq, in_sys_freq, sequence_freq,  total_events, tick_list, event_list, in_end_event_ticks);

//	for (i = 0; i<total_events; i = i + 1) {
//		printf("Ticknumber: %d\n", tick_list[i]);
//	}

	//Add one more event to allow for sequence end event
	int out_events[total_events+1];
	int  out_ticks[total_events+1];
	for (i = 0; i < total_events; i = i + 1) {
		out_events[i] = (int) event_list[i];
		out_ticks[i] = (int) tick_list[i];
	}

	//Add sequence end event, a few ticks before the end to allow the event to be processed
	arr_len = sizeof(out_ticks)/sizeof(out_ticks[0]);

	out_ticks[total_events] = round(in_RF_freq / sequence_freq * 1000000) - in_end_event_ticks;
	out_events[total_events] = end_event_number;

	int digits_no = 0;
	int digits_event = 0;
    int digits_ticks = 0;
    int digits_ns = 0;
	printf("\n No    |  Event no  |   Tick number   |  Time (ns)      \n_______|____________|_________________|_______________________\n");
        for (i = 0; i <= total_events; i = i + 1) {
		digits_no = 2 - floor(log10(abs(i+1))) + 1;
		digits_event = 2 - floor(log10(abs(out_events[i]+1))) + 1;
        digits_ticks = 8 - floor(log10(abs(out_ticks[i]+1))) + 1;
        digits_ns =  9 - floor(log10(abs((out_ticks[i] / in_RF_freq * 1000)+1))) + 1;
	printf(" %*c", digits_no,' ');
	printf("%d  ", i + 1);
	printf("|");
	printf("    %*c", digits_event,' ');
	printf("%d    ", (int) out_events[i]);
        printf("|");
        printf(" %*c", digits_ticks,' ');
        printf("%d      ", (int) out_ticks[i]);      
        printf("|");
        printf(" %*c", digits_ns,' ');
        printf("%d\n", (int) (out_ticks[i] / in_RF_freq * 1000));
        }
	
	//Output event list
	precord->neva = arr_len;	
	memcpy(precord->vala, out_events, arr_len * sizeof(out_events[0]));
	
	//Output tick list
	precord->nevb = arr_len;
	memcpy(precord->valb, out_ticks, arr_len * sizeof(out_ticks[0]));
	
	//Output commit value0
	*(int *)precord->valc = 1;

return 0;
}

epicsRegisterFunction(evr_sequence_calc);

