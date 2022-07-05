#include <ncurses.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include "constants.h"

static void handle_input(void);
static void tick(int signum);
static void game_over(void);

static char *playfield;
static int score, dying, ply_anim;
extern char *player_l;
extern char *player_r;
extern char *player_m;

int main(int argc, char *argv[])
{
	srand(time(NULL));
	/*
	player_x = 0;
	player_y = SCREENH / 2;
	bomb_x = -1;
	bomb_y = -1;
	bomb_animating = 100;
	bomb_frame = 0;
	bomb_delay = BOMB_DELAY_RESET;
	*/
	score = 0;
	dying = 0;
	ply_anim = 1;
	initscr();
	if (has_colors() == FALSE) {
		endwin();
		fprintf(stderr, "Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	nonl();
	cbreak();
	noecho();
	keypad(stdscr, true);
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	init_pair(2, COLOR_BLACK, COLOR_YELLOW);
	clear();
	curs_set(0);

	playfield = calloc(SCREENW + 1, sizeof(char));
	if (playfield == NULL) {
		perror("Failed to allocate playfield");
		exit(1);
	}

	memset(playfield, ' ', SCREENW);
	refresh();

	struct sigaction action;
	action.sa_handler = &tick;
	action.sa_flags = SA_RESTART;
	sigaction(SIGALRM, &action, NULL);

	struct itimerval timer;
	timer.it_value.tv_sec = 0;
	timer.it_value.tv_usec = 1000000 / FPS;
	timer.it_interval.tv_sec = 0;
	timer.it_interval.tv_usec = 1000000 / FPS;
	setitimer(ITIMER_REAL, &timer, NULL);

	for EVER {
		if (dying) {
			continue;
		}
		handle_input();
	}

	game_over();
}

static void handle_input(void)
{
	int c = getch();
	if (c == KEY_LEFT && ply_anim > 0) {
		ply_anim--;
	} else if (c == KEY_RIGHT && ply_anim < 2) {
		ply_anim++;
	}
}

static void tick(int signum)
{
	/*
	if (dying > 0) {
		if (dying > 200) {
			game_over();
			return;
		}

		dying++;
	} else {
		animate_bomb();
	}
	*/

	attron(COLOR_PAIR(1));
	for (int y = 0; y < SCREENH; y++) {
		mvprintw(y, 0, "%s", playfield);
	}
	mvprintw(0, 0, "Score: %d", score);
	attroff(COLOR_PAIR(1));

	attron(COLOR_PAIR(2));
	int y = SCREENH - PLAYER_H;
	int offset = (SCREENW - PLAYER_W) / 2;
	char *player = ply_anim == 0 ? player_l : (ply_anim == 1 ? player_m : player_r);
	for (int i = 0; i < PLAYER_H; i++) {
		for (int j = offset; j < PLAYER_W + offset; j++) {
			if (*player == '*') {
				mvprintw(y, j, "%s", " ");
			}
			player++;
		}
		y++;
	}
	attroff(COLOR_PAIR(2));

	/*
	if (dying > 0) {
		int d = dying / 20;
		char background = *(playfield + (player_y * (SCREENW + 1)) + player_x);
		mvaddch(player_y, player_x, d % 2 ? background : 'V');
	} else {
		mvaddch(player_y, player_x, 'V');
	}

	*/
	refresh();
}

static void game_over(void)
{
	endwin();
	printf("You scored: %d\n\n", score);
	exit(0);
}
