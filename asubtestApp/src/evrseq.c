#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <string.h> // Provides memcpy prototype
#include <stdlib.h> // Provides calloc prototyp
#include <stdio.h>
//#include <create_tick_event_list.h>

int create_tick_event_list(int *freqs, int arr_len, int base_event_no, int *tick_list, int *event_list) {
        int i, j, k, total_events, ticks_per_event, total_ticks, base_ticks, num_of_events, temp;
	float sys_freq, float_freqs[arr_len];
	temp = freqs[0];
	sys_freq = (float) temp;
	printf("create tick list input freq: %d arr_len: %d\n ", freqs[0], temp);
	printf("input float temp:  %f, input float freq:  %f\n", sys_freq, float_freqs[0]);
	for (i = 0; i < arr_len; i = i + 1) {
		float_freqs[i] = (float) freqs[i];
//		sys_freq = (float) freqs[0];
	}
	k = 0;
	printf("Tick list input freqs: %d, %d, %d, %d\n", freqs[0], freqs[1], freqs[2], freqs[3]);
	
	base_ticks = 6289424;
	total_ticks = base_ticks;
	printf("input float temp:  %d, input float freq:  %d\n", sys_freq, float_freqs[0]);
	sys_freq = 14.0;
	for (i = 0; i < 4; i = i + 1) {
		if ( sys_freq > freqs[i]) {
			sys_freq = 14.0/12.0;
			total_ticks = base_ticks * 12;
			}
	}
        
	for(i = 0; i < arr_len; i = i + 1) {
		if (freqs[i] > 0) {
			ticks_per_event = base_ticks / freqs[i];
			num_of_events = (int) round(float_freqs[i]/sys_freq);
			for(j = 0; j < num_of_events; j = j + 1) {
				event_list[k] = base_event_no+i;
				tick_list[k] = j * ticks_per_event;
				k = k + 1;
				printf("Event: %d Tick: %d\n", event_list[k], tick_list[k]);
			}
		}
        }

	k = 999;
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
	
	float in_freqs[4], in_delays_ns[4], sys_freq, in_base_event_no;
	int freqs[4], delays_ns[4], std_tick_list[512], std_event_list[512];
	int i, total_ticks, base_ticks, arr_len, base_event_no, total_events, tmp;

	printf("Start\n");
	tmp = *(int *)precord->a; 
	in_freqs[0] = *(float *)precord->a;
	in_freqs[1] = *(float *)precord->b;
	in_freqs[2] = *(float *)precord->c;
	in_freqs[3] = *(float *)precord->d;	
	in_delays_ns[0] = *(float *)precord->e;
	in_delays_ns[1] = *(float *)precord->f;
	in_delays_ns[2] = *(float *)precord->g;
	in_delays_ns[3] = *(float *)precord->h;


	for (i = 0; i < 4; i = i +1) {
		freqs[i] = (int)in_freqs[i];
		delays_ns[i] = (int)in_delays_ns[i];
	}	

	in_base_event_no = *(float *)precord->i; //remove i from integers?
	base_event_no = (int) in_base_event_no;
	
	arr_len = sizeof(freqs) / sizeof(freqs[0]);

	total_events = create_tick_event_list(freqs, arr_len, base_event_no, std_tick_list, std_event_list);
	printf("Total events: %d\n");
/*
	int tick_list[total_events], event_list[total_events];
	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	apply_delay(arr_len, delays_ns, tick_list, event_list);

	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
	sort_sequence(arr_len, tick_list, event_list);

	
	arr_len = sizeof(tick_list) / sizeof(tick_list[0]);
//	resolve_conflicts(arr_len, tick_list, event_list);
	
*/
return 0;
}

epicsRegisterFunction(evr_sequence_modifier);

