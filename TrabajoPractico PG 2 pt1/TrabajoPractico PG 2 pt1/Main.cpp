#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\color.h"
#include "allegro5\allegro_primitives.h"
#include "allegro5\allegro_audio.h"
#include "allegro5\allegro_acodec.h"
#include <iostream>
#define BVEL 5
#define DECBR -100
#define DECBL 700
#define EDEFR 700
#define EDEFL -150
#define EVEL 2
#define ALTO 480
#define ANCHO 640
using namespace std;

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP  *player = NULL;
	ALLEGRO_BITMAP  *enemyR = NULL;
	ALLEGRO_BITMAP  *enemyL = NULL;
	ALLEGRO_BITMAP  *bulletR = NULL;
	ALLEGRO_BITMAP  *bulletL = NULL;
	ALLEGRO_BITMAP  *menu = NULL;
	ALLEGRO_BITMAP  *back = NULL;
	ALLEGRO_BITMAP  *life3 = NULL;
	ALLEGRO_BITMAP  *life2 = NULL;
	ALLEGRO_BITMAP  *life1 = NULL;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	ALLEGRO_SAMPLE *hit;
	ALLEGRO_SAMPLE *shoot;
	ALLEGRO_SAMPLE *music;
	/* Inits */
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();
	if (!al_install_audio()) {
		fprintf(stderr, "failed to initialize audio!\n");
		return -1;
	}

	if (!al_init_acodec_addon()) {
		fprintf(stderr, "failed to initialize audio codecs!\n");
		return -1;
	}

	/* create */
	display = al_create_display(ANCHO, ALTO);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60.0);
	player = al_load_bitmap("player.png");
	enemyR = al_load_bitmap("enemyR.png");
	enemyL = al_load_bitmap("enemyL.png");
	bulletR = al_load_bitmap("BulletR.png");
	bulletL = al_load_bitmap("BulletL.png");
	menu = al_load_bitmap("Menu.png");
	back = al_load_bitmap("Background.png");
	life3 = al_load_bitmap("lifebar.png");
	life2 = al_load_bitmap("lifebar2.png");
	life1 = al_load_bitmap("lifebar3.png");
	hit = al_load_sample("hurt.wav");
	shoot = al_load_sample("shoot.wav");
	music = al_load_sample("music.wav");
	al_reserve_samples(3);

	ALLEGRO_SAMPLE_INSTANCE *song = al_create_sample_instance(music);
	al_set_sample_instance_playmode(song, ALLEGRO_PLAYMODE_LOOP);
	al_attach_sample_instance_to_mixer(song, al_get_default_mixer());

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));

	al_play_sample_instance(song);
	/* Start */
	bool gameOver = false;
	al_flip_display();
	int width = al_get_display_width(display);
	int height = al_get_display_height(display);
	float x = 150;
	float eRX = 550;
	float eLX = -100;
	bool dirr = true;
	bool gameStart = false;
	int bRX = -100;
	int bLX = 700;
	bool activeR = false;
	bool activeL = false;
	int life = 3;

	/* GameLoop */
	al_start_timer(timer);
	while (!gameOver) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);
		//----MENU---------------------------
		if (!gameStart) {
			al_draw_bitmap(menu, 0, 0, 0);
		}
		if (al_key_down(&keyState, ALLEGRO_KEY_ENTER)) {
			gameStart = true;
		}
		//----SALIR--------------------------
		if (al_key_down(&keyState, ALLEGRO_KEY_ESCAPE)) {
			gameOver = true;
		}
		//----GAMELOOP-----------------------
		if (gameStart == true) {
			if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
				gameOver = true;
			}
			//----DRAW----
			if (event.type == ALLEGRO_EVENT_TIMER) {
				al_draw_bitmap(back, 0, 0, 0);
				al_draw_bitmap(enemyR, eRX, 250, 0);
				al_draw_bitmap(enemyL, eLX, 250, 0);
				al_draw_bitmap(bulletR, bRX, 360, 0);
				al_draw_bitmap(bulletL, bLX, 360, 0);
				al_draw_bitmap(player, x, 300, 0);
				switch (life) {
				case 1:
					al_draw_bitmap(life1, 5, 5, 0);
					break;
				case 2:
					al_draw_bitmap(life2, 5, 5, 0);
					break;
				case 3:
					al_draw_bitmap(life3, 5, 5, 0);
					break;
				}
			}
			//----KEYBOARD----
			if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
				x += 2;
				dirr = true;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
				x -= 2;
				dirr = false;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_SPACE)) {
				if (dirr) {
					if (!activeR) {
						bRX = x;
						activeR = true;
						al_play_sample(shoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}
				else {
					if (!activeL) {
						bLX = x;
						activeL = true;
						al_play_sample(shoot, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
					}
				}
			}
			//----BORDERCOLLISION----
			if (x > width - al_get_bitmap_width(player)) {
				x = width - al_get_bitmap_width(player);
			}
			else if (x < 0) {
				x = 0;
			}
			//----ENEMYCOLLISION----
			if (x + al_get_bitmap_width(player) > eRX+10) {
				life--;
				eRX = EDEFR;
				al_play_sample(hit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			if (x < eLX + al_get_bitmap_width(enemyL)-10) {
				life--;
				eLX = EDEFL;
				al_play_sample(hit, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, 0);
			}
			//----SHOOT----
			if (activeR) {
				bRX += BVEL;
				if (bRX > ANCHO + 10) {
					bRX = DECBR;
					activeR = false;
				}
			}
			if (activeL) {
				bLX -= BVEL;
				if (bLX < -100) {
					bLX = DECBL;
					activeL = false;
				}
			}
			if (bRX + al_get_bitmap_width(bulletR) >= eRX) {
				eRX = EDEFR;
				bRX = DECBR;
				activeR = false;
			}
			if (bLX <= eLX + al_get_bitmap_width(enemyL)) {
				eLX = EDEFL;
				bLX = DECBL;
				activeL = false;
			}
			//----ENEMY MOVEMENT----
			eRX -= EVEL;
			eLX += EVEL;
			//----ENDGAME----
			if (life <= 0) {
				gameOver = true;
			}
		}
		al_flip_display();
	}

	/* destroy */
	al_destroy_display(display);
	al_destroy_bitmap(player);
	al_destroy_bitmap(enemyR);
	al_destroy_bitmap(enemyL);
	al_destroy_bitmap(bulletR);
	al_destroy_bitmap(bulletL);
	al_destroy_bitmap(menu);
	al_destroy_bitmap(back);
	al_destroy_bitmap(life1);
	al_destroy_bitmap(life2);
	al_destroy_bitmap(life3);
	al_destroy_sample(hit);
	al_destroy_sample(shoot);
	al_destroy_sample(music);
	al_destroy_sample_instance(song);

	al_destroy_event_queue(queue);
	return 0;
}