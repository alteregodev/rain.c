#include <stdio.h> 
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ioctl.h>

void print_matrix(char** matrix, int width, int height)
{
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			printf("%c", matrix[i][j]);
		}
	}
}

void rain_loop(char** matrix, int width, int height, int drop_length, int delay)
{
	const char drop = '|';
	while (1) {
		const int random_pos = rand() % width; // Random x pos for the drop
		for (int i = 0; i <= height - drop_length; i++) {
			for (int g = 0; g < drop_length; g++) {
				if (g >= height)
					break;
				matrix[i+g][random_pos] = drop; // print the drop into the matrix
			}
			usleep(delay); // sleep but with microseconds instead of seconds
			system("clear");
			print_matrix(matrix, width, height);

			for (int g = 0; g < drop_length; g++) {
				if (g >= height)
					break;
				matrix[i+g][random_pos] = ' '; // clear the drop
			}
		}
	}
}

char** get_matrix(int width, int height)
{
	char** matrix = malloc(height * sizeof(char*)); // allocate memory for rows
	if (matrix == NULL) {
		perror("Malloc failed");
		exit(EXIT_FAILURE);
	}

	for (int n = 0; n < height; n++) {
		matrix[n] = malloc((width + 1) * sizeof(char)); // allocate memory for each character in rows
		if (matrix[n] == NULL) { // check for succesful malloc
			perror("Failed to allocate memory for rows");
			for (int k = 0; k < n; k++) {
				free(matrix[k]);
			}
			free(matrix);
			exit(EXIT_FAILURE);
		}
	}

	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			matrix[i][j] = ' ';
		}
		matrix[i][width] = '\0'; // set the null terminator
	}

	return matrix;
}

int main(void)
{
	srand(time(NULL));
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1) {
		perror("ioctl");
		exit(EXIT_FAILURE);
	}

	int width = w.ws_col; // row length 
	int height = w.ws_row; // matrix height

	int drop_length; // length of the drop to be rendered
	int delay; // delay of drop movement

	printf("Enter drop length\n> ");
	if (scanf("%d", &drop_length) != 1) {
		printf("Invalid input\n");
		return 1;
	} else if (drop_length > 12) {
		printf("Too much length\n");
		return 1;
	}

	printf("Enter delay in microseconds (1000 microseconds = 1 milisecond)\n> ");
	if (scanf("%d", &delay) != 1) {
		printf("Invalid input\n");
		return 1;
	}

	char** matrix = get_matrix(width, height); // matrix

	rain_loop(matrix, width, height, drop_length, delay); // run loop

	for (int i = 0; i < height; i++) { // free memory when loop ended
		free(matrix[i]);
	}
	free(matrix);

	return 0;
}
