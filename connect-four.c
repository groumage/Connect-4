#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

typedef enum {
	red,
	green,
	yellow,
	blue,
} color_t;

// change the color of which is print on the console
void change_color (color_t c) {
	switch (c) {
		case red:
		printf("\033[31m");
		break;
		case green:
		printf("\033[32m");
		break;
		case yellow:
		printf("\033[33m");
		break;
		case blue:
		printf("\033[34m");
		break;
		default:
		printf("\033[0m");
	}
}

// print the grid and every discs placed by the two players
void print_grid(int * grid, int nb_lines, int nb_cols, int index_column_full, bool wrong_input) {
	system("clear");
	printf("Let's play a Connect 4 !\n\n");
	for (int i = 0; i < nb_lines; i++) {
		for (int j = 0; j < nb_cols; j++) {
			if (grid[i * nb_cols + j] == 1) {
				change_color(red);
				printf("X");
				change_color(-1);
			}
			else if (grid[i * nb_cols + j] == 2) {
				change_color(yellow);
				printf("0");
				change_color(-1);
			}
			else
				printf("-");
			if (j != nb_cols - 1)
				printf("|");
		}
		printf("\n");
	}
	printf("\n");
	if (index_column_full != -1)
		printf("Column %d is full.\n", (index_column_full + 1));
	if(wrong_input)
		printf("The input must be a number between 1 and %d.\n", nb_cols);
}

// return true if the input has good format, false otherwise
bool check_input (char * input) {
	if (input[0] >= '0' && input[0] <= '9' && input[1] == 0)
		return true;
	else if (input[0] >= '0' && input[0] <= '9' && input[1] >= '0' && input[1] <= '9')
		return true;
	else return false;
}

// return the column where the player is puting his disc
int get_column(int player, int * grid, int nb_lines, int nb_cols) {
	char * input = malloc(sizeof(char) * 3);
	int column = 0;
	bool correct_input = false;
	printf("Player %d, which column ?\n", (player + 1));
	scanf("%2s", input);
	while (correct_input == false) {
		while (!check_input(input)) {
			print_grid(grid, nb_lines, nb_cols, -1, true);
			printf("Player %d, which column ?\n", (player + 1));
			scanf("%2s", input);
		}
		column = strtol(input, NULL, 10);
		// check if the input (at this point, it's a number) in in the correct range
		if (column >= 1 && column <= nb_cols)
			correct_input = true;
		else {
			print_grid(grid, nb_lines, nb_cols, -1, true);
			printf("Player %d, which column ?\n", (player + 1));
			scanf("%2s", input);
		}
	}
	free(input);
	return column;
}

// return the line where the disc have to be placed
// return -1 if there is no line free (ie the column is full)
int find_line(int col, int * grid, int nb_lines, int nb_cols) {
	for (int i = nb_lines - 1; i >= 0; i--)
		if (grid[nb_cols * i + col] == 0)
			return i;
	return -1;
}

// return 1 if player 1 has won
// return 2 if player 2 has won
// return -1 otherwise
int check_win(int * grid, int nb_lines, int nb_cols) {
	for (int i = 0; i < nb_lines - 3; i++) {
		for (int j = 0; j < nb_cols; j++) {
			// vertical check
			if (grid[i * nb_cols + j] != 0 &&
				grid[i * nb_cols + j] ==  grid[(i + 1) * nb_cols + j] && 
				grid[(i + 1) * nb_cols + j] == grid[(i + 2) * nb_cols + j]  && 
				grid[(i + 2) * nb_cols + j] == grid[(i + 3) * nb_cols + j])
				return grid[i * nb_cols + j];
		}
	}
	return -1;
}

// main function
int main() {
	int nb_lines = 6;
	int nb_cols = 12;
	int dim = nb_lines * nb_cols;
	int player = 0;
	bool win = false;
	int * grid = malloc(dim * sizeof(int));
	for (int i = 0; i < dim; i++)
		grid[i] = 0;
	print_grid(grid, nb_lines, nb_cols, -1, false);
	while (win == false) {
		int index_column_full = 1;
		while (index_column_full != -1) {
			index_column_full = -1;
			int col_player = get_column(player, grid, nb_lines, nb_cols);
			col_player = col_player - 1;
			int line_player = find_line(col_player, grid, nb_lines, nb_cols);
			if (line_player == -1)
				index_column_full = col_player;
			else {
				if (player == 0)
					grid[nb_cols * line_player + col_player] = 1;
				else
					grid[nb_cols * line_player + col_player] = 2;
			}
			print_grid(grid, nb_lines, nb_cols, index_column_full, false);
		}
		if (check_win(grid, nb_lines, nb_cols) == 1) {
			printf("Player 1 win the game !\n");
			win = true;
		}
		else if (check_win(grid, nb_lines, nb_cols) == 2) {
			printf("Player 2 win the game !\n");
			win = true;
		}
		else
			player = (player + 1) % 2;
	}
	free(grid);
	return EXIT_SUCCESS;
}