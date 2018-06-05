#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\color.h"
#include "allegro5\allegro_primitives.h"
#include <iostream>
#define BVEL 5
#define DECB -100
#define ALTO 480
#define ANCHO 640
using namespace std;

int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP  *player = NULL;
	ALLEGRO_BITMAP  *enemy = NULL;
	ALLEGRO_BITMAP  *bullet = NULL;
	ALLEGRO_BITMAP *menu = NULL;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	/* Inits */
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();
	/* create */
	display = al_create_display(ANCHO, ALTO);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60.0);
	player = al_load_bitmap("player.png");
	enemy = al_load_bitmap("enemy.png");
	bullet = al_load_bitmap("Bullet.png");
	menu = al_load_bitmap("Menu.png");

	al_register_event_source(queue, al_get_keyboard_event_source());
	al_register_event_source(queue, al_get_display_event_source(display));
	al_register_event_source(queue, al_get_timer_event_source(timer));
	/* Start */
	bool gameOver = false;
	al_flip_display();
	int width = al_get_display_width(display);
	int height = al_get_display_height(display);
	float x = 0;
	float enemyX = 400;
	bool dirr = true;
	bool gameStart = false;
	int bX=-100;
	bool activeBullet = false;

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
				al_clear_to_color(al_map_rgb(255, 255, 255));
				al_draw_bitmap(enemy, enemyX, 50, 0);
				al_draw_bitmap(bullet, bX, 100, 0);
				al_draw_bitmap(player, x, 50, 0);
			}
			//----KEYBOARD----
			if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
				x += 2;
				dirr = false;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
				x -= 2;
				dirr = true;
			}
			if (al_key_down(&keyState, ALLEGRO_KEY_SPACE)) {
				if (!activeBullet) {
					bX = x;
					activeBullet = true;
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
			if (x + al_get_bitmap_width(player) > enemyX) {
				gameOver = true;
			}
			//----SHOOT----
			if (activeBullet) {
				bX += BVEL;
				if (bX > ANCHO+10|| bX < -10) {
					bX = DECB;
					activeBullet = false;
				}
			}
		}
		al_flip_display();
	}

	/* destroy */
	al_destroy_display(display);
	al_destroy_bitmap(player);
	al_destroy_bitmap(enemy);
	al_destroy_event_queue(queue);
	al_uninstall_keyboard();
	return 0;
}