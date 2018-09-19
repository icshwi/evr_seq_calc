#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <string.h> // Provides memcpy prototype
#include <stdlib.h> // Provides calloc prototyp
#include <stdio.h>
//#include <create_tick_event_list.h>

int  create_tick_event_list(int freqs[], int arr_len, int sys_freq, int total_ticks, int base_event_no, int *tick_list, int *event_list) {
        int i, j, k, total_events, ticks_per_event;
	k = 0;
        for(i = 0; i < arr_len; i = i + 1) {
		ticks_per_event = total_ticks / freqs[i];
		for(j = 0; j < freqs[i]; j = j + 1) {
			event_list[k] = base_event_no+i;
			tick_list[k] = j * ticks_per_event;
			k = k + 1;
		}
        }
	total_events = k;
        return total_events;

}

void apply_delay(int arr_len, int *delay_list, int *tick_list, int *event_list) {
	int i, delay_index;
	delay_index = 0;
	for (i = 0; i < arr_len; i = i + 1) {
		if (event_list[i] == event_list[i+1]) {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
		}
		else {
			tick_list[i] = tick_list[i]+delay_list[delay_index];
			delay_index = delay_index + 1;
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
}

static int evr_sequence_modifier(aSubRecord *precord) {
	
	int in_freqs[4], in_delays_ns[4], std_tick_list[512], std_event_list[512];
	int i, total_ticks, base_ticks, arr_len, base_event_no, total_events;
	float low_freq;

	printf("Start\n");
	in_freqs[0] = *(int *)precord->a;
	in_freqs[1] = *(int *)precord->b;
	in_freqs[2] = *(int *)precord->c;
	in_freqs[3] = *(int *)precord->d;

	in_delays_ns[0] = *(int *)precord->e;
	in_delays_ns[1] = *(int *)precord->f;
	in_delays_ns[2] = *(int *)precord->g;
	in_delays_ns[3] = *(int *)precord->h;

	base_event_no = *(int *)precord->i; //remove i from integers?

	base_ticks = 6289424;	//int *p_tick_event_list;
	total_ticks = base_ticks;
	low_freq = 14.0;
	for (i = 0; i < 4; i = i + 1) {
		if ( low_freq > in_freqs[i]) {
			low_freq = 14.0/12.0;
			total_ticks = base_ticks * 12;
		}
	}
	printf("Input freqs: %d, %d\n", sizeof(in_freqs[0]), sizeof(in_freqs));
	/*arr_len = sizeof(in_freqs) / sizeof(in_freqs[0]);
	total_events = create_tick_event_list(in_freqs, arr_len, low_freq, total_ticks, base_event_no, std_tick_list, std_event_list);

	int tick_list[total_events], event_list[total_events];
	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	apply_delay(arr_len, in_delays_ns, tick_list, event_list);

	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	sort_sequence(arr_len, tick_list, event_list);

	
	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
//	resolve_conflicts(arr_len, tick_list, event_list);
	
*/
return 0;
}

epicsRegisterFunction(evr_sequence_modifier);

