#include "allegro5\allegro.h"
#include "allegro5\allegro_image.h"
#include "allegro5\color.h"
#include "allegro5\allegro_primitives.h"
#include <iostream>

using namespace std;
int main(int argc, char **argv) {
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_BITMAP  *image = NULL;
	ALLEGRO_BITMAP  *image2 = NULL;
	ALLEGRO_EVENT_QUEUE *queue;
	ALLEGRO_TIMER *timer;
	/* Inits */
	al_init();
	al_init_image_addon();
	al_install_keyboard();
	al_init_primitives_addon();
	/* create */
	display = al_create_display(640, 480);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 60.0);
	image = al_load_bitmap("player.png");
	image2 = al_load_bitmap("enemy.png");
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
	/* GameLoop */
	al_start_timer(timer);
	while (!gameOver) {
		ALLEGRO_EVENT event;
		al_wait_for_event(queue, &event);
		if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			gameOver = true;
		}
		if (event.type == ALLEGRO_EVENT_TIMER) {
			al_clear_to_color(al_map_rgb(255, 255, 255));
			al_draw_bitmap(image,x,50,0);
			al_draw_bitmap(image2, enemyX, 0, 0);
			al_flip_display();
		}
		ALLEGRO_KEYBOARD_STATE keyState;
		al_get_keyboard_state(&keyState);

		if (al_key_down(&keyState, ALLEGRO_KEY_RIGHT)) {
			x += 2;
		}
		if (al_key_down(&keyState, ALLEGRO_KEY_LEFT)) {
			x -= 2;
		}
		if (x > width - al_get_bitmap_width(image)) {
			x = width - al_get_bitmap_width(image);
		}
		else if (x < 0) {
			x = 0;
		}

		if (x + al_get_bitmap_width(image) > enemyX) {
			gameOver = true;
		}
	}

	/* destroy */
	al_destroy_display(display);
	al_destroy_bitmap(image);
	al_destroy_bitmap(image2);
	al_destroy_event_queue(queue);
	al_uninstall_keyboard();
	return 0;
}