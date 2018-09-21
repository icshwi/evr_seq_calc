#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <string.h> // Provides memcpy prototype
#include <stdlib.h> // Provides calloc prototyp
#include <stdio.h>
#include <math.h>
//#include <create_tick_event_list.h>

int create_tick_event_list(float *freqs, int arr_len, float base_event_no, float RF_freq, float base_freq, float sys_freq, int *tick_list, int *event_list) {
        int i, j, k, total_events, num_of_events;
	float ticks_per_event, ticks_per_cycle;
	
	ticks_per_cycle = round(RF_freq/base_freq);
	k = 0;

	for(i = 0; i < arr_len; i = i + 1) {
		if (freqs[i] > 0) {
			ticks_per_event = ticks_per_cycle * base_freq / freqs[i];
			num_of_events = (int) round(freqs[i]/sys_freq);
			for(j = 0; j < num_of_events; j = j + 1) {
				event_list[k] = ((int) base_event_no) + i;
				tick_list[k] =  (int) round(j * ticks_per_event);
				k = k + 1;
			}
		}
        }
	total_events = k;
        return total_events;

}

void apply_delay(float RF_freq, float base_freq, float sys_freq, int arr_len, int *delay_list, int *tick_list, int *event_list) {
	int i, delay_index;
	float seq_len;
	seq_len = RF_freq / sys_freq;
	delay_index = 0;
	for (i = 0; i < arr_len; i = i + 1) {
		if (event_list[i] == event_list[i+1]) {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
		}
		else {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
			delay_index = delay_index + 1;
		}
		if (tick_list[i] > (int) (seq_len - 3) && tick_list[i] <= seq_len) {
			tick_list[i] = 0;
		}
		else if (tick_list[i] > seq_len) {
			tick_list[i] = tick_list[i] % (int) seq_len;
		}
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
		printf("Event: %d, Tick: %d\n", event_list[i], tick_list[i]);
	}
}

void resolve_conflicts(float RF_freq, float base_freq, float sys_freq, int arr_len, int *tick_list, int *event_list) {
	int i;
	float seq_len;
	seq_len = RF_freq / sys_freq;
	i = 1;
	printf("arr_len = %d\n", arr_len);
	while (i < arr_len) {
		if (tick_list[i-1] == tick_list[i]) {
			printf("ticks = %d  i = %d\n", tick_list[i], i);
			tick_list[i] = tick_list[i] + 1;
			if (tick_list[i] > (seq_len - 3)) {
				tick_list[i] = 0;
			}
			i = 0;
		}
		i = i + 1;
	}
	printf("i = %d\n", i);
	sort_sequence(arr_len, tick_list, event_list);
	return ;
}
static int evr_sequence_modifier(aSubRecord *precord) {
	
	float in_freqs[4], in_delays_ns[4], in_base_event_no, in_RF_freq, in_base_freq, sys_freq, lowest_base_freq;
	int delays_ticks[4], full_tick_list[512], full_event_list[512];
	int i, arr_len, total_events;
	lowest_base_freq = 12.0;
//	base_ticks = 6289424;
	printf("Start\n");

	in_freqs[0] 		= *(float *)precord->a;
	in_freqs[1] 		= *(float *)precord->b;
	in_freqs[2] 		= *(float *)precord->c;
	in_freqs[3] 		= *(float *)precord->d;	
	in_delays_ns[0] 	= *(float *)precord->e;
	in_delays_ns[1] 	= *(float *)precord->f;
	in_delays_ns[2] 	= *(float *)precord->g;
	in_delays_ns[3] 	= *(float *)precord->h;
	in_base_event_no 	= *(float *)precord->i;
	in_RF_freq 		= *(float *)precord->j;
	in_base_freq 		= *(float *)precord->k;
	

	for (i = 0; i < sizeof(in_freqs)/sizeof(in_freqs[0]); i = i +1) {
		delays_ticks[i] = (int) round(in_delays_ns[i] / 1000000000.0 * in_RF_freq);
	}	
       
	arr_len = sizeof(in_freqs) / sizeof(in_freqs[0]);

	sys_freq = in_base_freq;
	for (i = 0; i < arr_len; i = i + 1) {
		if ( sys_freq > in_freqs[i] && in_freqs[i] > 0.0) {
			sys_freq = in_base_freq/lowest_base_freq;
		}
	}

	//Create two arrays, one with all events and one with the corresponding tick timing
	total_events = create_tick_event_list(in_freqs, arr_len, in_base_event_no, in_RF_freq, in_base_freq, sys_freq, full_tick_list, full_event_list);

	//Reduce the full lists number of events needed and copy values
	int tick_list[total_events], event_list[total_events];
	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	for(i = 0; i < arr_len; i = i + 1){
		tick_list[i] = full_tick_list[i];
		event_list[i] = full_event_list[i];
	}
	
	//Apply delay for each event in the tick array
	apply_delay(in_RF_freq, in_base_freq, sys_freq, arr_len, delays_ticks, tick_list, event_list);

	//Sort the list after tick size
	sort_sequence(arr_len, tick_list, event_list);

	//Ensure no events happen on the same tick count
	resolve_conflicts(in_RF_freq, in_base_freq, sys_freq,  arr_len, tick_list, event_list);

	//Output event list
	precord->neva = arr_len;
	memcpy(precord->vala, event_list, arr_len * sizeof(event_list[0]));
	
	//Output tick list
	precord->nevb = arr_len;
	memcpy(precord->valb, tick_list, arr_len * sizeof(tick_list[0]));

	//Output commit value
	*(float *)precord->valc = 1;

return 0;
}

epicsRegisterFunction(evr_sequence_modifier);

