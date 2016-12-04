/*
 * main.c
 */

#include "open_interface.h"
#include "lcd.h"
#include "botlogic.h"


int main(void) {
	oi_t *sensor_data = oi_alloc();
   oi_init(sensor_data);
	lcd_init();
	//int i = 0;

/*
	//Seven Nation Army
	int song_n = 0;
	int num_notes = 16;
	unsigned char notes [16] =    {64, 64, 67, 64, 62, 60, 59, 64, 64, 67, 64, 62, 60, 59};
	unsigned char duration [16] = {48, 16, 24, 24, 16, 64, 64 ,48, 16, 24, 24, 16, 64, 64};
	oi_loadSong(song_n, num_notes, notes, duration);
	oi_play_song(song_n);


	while(1){
		oi_update(sensor_data);
		if(sensor_data->songPlaying == 0){ break;}
	}

	int song_n2 = 2;
	unsigned char notes2 [16] =    {60, 60, 60, 60, 60, 60, 60, 60, 62, 62, 62, 62,62, 62, 62, 62};
	unsigned char duration2 [16] = {16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

	oi_loadSong(song_n2, num_notes, notes2, duration2);
	oi_play_song(song_n2);

	while(1){
		oi_update(sensor_data);
		if(sensor_data->songPlaying == 0){ break;}
	}

	int song_n5 = 0;
	unsigned char notes5 [16] =    {64, 64, 67, 64, 62, 60, 59, 64, 64, 67, 64, 62, 60, 59, 64};
	unsigned char duration5 [16] = {48, 16, 24, 24, 16, 64, 64 ,48, 16, 24, 24, 16, 64, 64, 128};
	oi_loadSong(song_n5, num_notes, notes5, duration5);
	oi_play_song(song_n5);
*/



//Final Count Down *cheers*

	int song_n = 0;
	int num_notes = 16;
	unsigned char notes [16] =    {88, 86, 88, 81,  0, 89, 88, 89, 88, 86,  0, 89, 88, 89, 81,  0};
	unsigned char duration [16] = {8,  8,  32, 32, 48,  8,  8, 16, 16, 32, 48,  8,  8, 32, 32, 48};
	oi_loadSong(song_n, num_notes, notes, duration);
	oi_play_song(song_n);

	while(1){
		oi_update(sensor_data);
		if(sensor_data->songPlaying == 0){ break;}
	}

	int song_n2 = 2;
	unsigned char notes2 [16] =    {86, 84, 86, 84, 83, 86, 84, 88, 86, 88, 81,  0, 89, 88, 89};
	unsigned char duration2 [16] = { 8,  8, 16, 16, 16, 16, 48,  8,  8, 32, 32, 48,  8,  8, 16};

	oi_loadSong(song_n2, num_notes, notes2, duration2);
	oi_play_song(song_n2);

	while(1){
	oi_update(sensor_data);
	if(sensor_data->songPlaying == 0){ break;}
	}

	int song_n3 =3;
	unsigned char notes3 [16] =    {88, 86,  0, 89, 88, 89, 81,  0, 86, 84, 86, 84, 83, 86, 84, 83};
	unsigned char duration3 [16] = {16, 32, 48,  8,  8, 16, 32, 48,  8,  8, 16, 16, 16, 16, 48,  8};

	oi_loadSong(song_n3, num_notes, notes3, duration3);
	oi_play_song(song_n3);


	while(1){
	oi_update(sensor_data);
	if(sensor_data->songPlaying == 0){ break;}
	}

	int song_n4 =1;
	unsigned char notes4 [16] =    {84, 86, 83, 84, 88, 86, 84, 83, 81, 89, 88, 88, 89, 88, 86, 100};
	unsigned char duration4 [16] = { 8, 48,  8,  8, 16, 16, 16, 16, 32, 32, 96,  8,  8,  8,  8, 128};

	oi_loadSong(song_n4, num_notes, notes4, duration4);
	oi_play_song(song_n4);



	/*
	//Iowa State TUBA fight song
		int song_n = 0;
		int num_notes = 16;

		unsigned char notes [16] =    {63, 68, 65, 70, 63, 58, 63, 58, 63, 58, 63, 58, 63, 67, 70, 69};
		unsigned char duration [16] = {24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24, 24}

		oi_loadSong(song_n, num_notes, notes, duration);
		oi_play_song(song_n)
;
		while(1){
			oi_update(sensor_data);
			if(sensor_data->songPlaying == 0){ break;


}
		int song_n2 =2;
		int num_notes2 = 16;
		 unsigned char notes2 [16] =    {70, 74, 72, 70, 69, 68, 67, 65, 70, 65, 70, 65, 70, 65, 72, 70};
		 unsigned char duration2 [16] = {12, 24, 12, 12, 12, 12, 12, 24, 24, 24, 24, 24, 24, 12, 12, 12}

		oi_loadSong(song_n2, num_notes2, notes2, duration2);
		oi_play_song(song_n2);

		while(1){
			oi_update(sensor_data);
			if(sensor_data->songPlaying == 0){ break;

		}

		int song_n3 =3;
		int num_notes3 = 16;
		unsigned char notes3 [16] =    {69, 68, 65, 70, 68, 67, 67, 67, 68, 68, 67, 65, 63, 58, 63, 58};
		unsigned char duration3 [16] = {12, 24, 24, 24, 24, 16, 24, 16, 16, 16, 16, 16, 24, 24, 24, 24}

		oi_loadSong(song_n3, num_notes3, notes3, duration3);
		oi_play_song(song_n3);

		while(1){
			oi_update(sensor_data);
			if(sensor_data->songPlaying == 0){ break;



		int song_n4 = 3;
		int num_notes4 = 16;
		unsigned char notes4 [16] =    {63, 58, 63, 58, 63, 63, 68, 67, 65, 63, 68,  0, 68, 68, 69, 69};
		unsigned char duration4 [16] = {24, 24, 24, 24, 24, 24, 12, 12, 12, 12, 12, 36, 24, 24, 24, 24};
		oi_loadSong(song_n4, num_notes4, notes4, duration4);
		oi_play_song(song_n4);

*/

		lcd_clear();
		lcd_puts("WelcoEm");



    return 0;
}

