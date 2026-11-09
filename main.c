#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

uint8_t tape[80000];
uint8_t* tape_begin = (uint8_t*)&tape;
uint8_t* tape_end = (uint8_t*)&tape + 79999;
uint8_t* pointer = (uint8_t*)&tape;


int pc = 0;

int depth;

char* program;
char* file_buf;

#define debug 0

char* messages[] = {
"Divine language",
"No errors btw",
"Brainfuck but better",
"Really fast language!",
"Called BF+ btw",
"Want to main this language?",
"Works as intended",
"Skill issue",
"Have you tried not making mistakes?",
"Undefined? Never heard of it",
"Your code is your problem",
"Garbage In, Garbage Out",
"Trust the process",
"Certified spaghetti",
"Speed is a feature",
"Memory is temporary",
"Everything is a feature",
"Not a bug, just behavior",
"Segmentation? Sounds like a you problem",
"One char at a time",
"Less talking, more executing",
"Zero complaints",
"Probably works",
"Don't question the interpreter",
"Syntax is a social construct",
"Your CPU is sweating",
"Born to interpret",
"Brainfuck? More like Brainfast",
"Error 404: Errors not found",
"Optimized by doing less",
"Written in C, obviously",
"Works on my machine",
"Please don't inspect the source",
"Your warranty is void",
"Expected nothing, got something",
"Compiler? Never needed one",
"Made with questionable decisions",
"Runtime? More like fun time",
"Please lower your expectations",
"Logic is optional",
"Works until it doesn't",
"Powered by questionable logic",
"Your RAM belongs to us",
"Don't worry about the details",
"Nobody knows why this works",
"Stack overflow is a lifestyle",
"Professional nonsense",
"Trust me, I'm interpreted",
"Made possible by bad decisions",
"Performance over sanity",
"Why fix what isn't broken?",
"One language, zero standards",
"Everything is under control",
"Made by a person with divine intellect",
"The creator of this is a Terry Davis fan",
"The syntax was born in hell",
"Want to see 256 cool symbols? [.+]",
"The creator of this uses NixOS btw"
};

long get_file_size(const char *filename) {
    FILE *fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        return -1;
    }

    fseek(fptr, 0, SEEK_END);
    long size = ftell(fptr);

    fclose(fptr);
    return size;
}

int main(int argc, char* argv[]) {
	printf("===Just wanna warn you: this language has no errors===\n");
	srand(time(NULL) ^ getpid());
	printf("Your random message: %s\n", messages[rand() % 58]);
	if (argc != 2) {
		return 1;
	}

	FILE* fptr = fopen(argv[1], "r");

	if (fptr == NULL) {
		program = argv[1];
	} else {
		long size = get_file_size(argv[1]);
		file_buf = malloc(size + 1);
		if (fgets(file_buf, size, fptr) == NULL) {
			printf("Couldn't copy file content to buffer!\n");
			fclose(fptr);
			free(file_buf);
			return 1;
		} else {
			program = file_buf;
		}
	}
	
	while (program[pc] != '\0') {
		if (debug) {
			printf("\n------------\nPC: %d\nInstruction: %c\nPointer Position: %d\n------------\n\n", pc, program[pc], (int)(pointer - tape_begin));
		}
		switch (program[pc]) {
			case '+':
				if (program[pc + 1] >= 48 && program[pc + 1] <= 57) {
					int add_num = 0;
					add_num += (program[pc + 1] - '0') * 100;
					add_num += (program[pc + 2] - '0') * 10;
					add_num += (program[pc + 3] - '0');
					pc += 3;
					(*pointer) += add_num;
				} else {
					(*pointer)++;
				}
				break;
			case '-':
				if (program[pc + 1] >= 48 && program[pc + 1] <= 57) {
					int sub_num = 0;
					sub_num += (program[pc + 1] - '0') * 100;
					sub_num += (program[pc + 2] - '0') * 10;
					sub_num += (program[pc + 3] - '0');
					pc += 3;
					(*pointer) -= sub_num;
				} else {
					(*pointer)--;
				}
				break;
			case '>':
				if (program[pc + 1] >= 48 && program[pc + 1] <= 57) {
					int jmp_num = 0;
					jmp_num += (program[pc + 1] - '0') * 100;
					jmp_num += (program[pc + 2] - '0') * 10;
					jmp_num += (program[pc + 3] - '0');
					pc += 3;
					if (pointer + jmp_num <= tape_end) {
						pointer += jmp_num;
					}
				} else {
					if (pointer < tape_end) {
						pointer++;
					}
				}
				break;
			case '<':
				if (program[pc + 1] >= 48 && program[pc + 1] <= 57) {
					int jmp_num = 0;
					jmp_num += (program[pc + 1] - '0') * 100;
					jmp_num += (program[pc + 2] - '0') * 10;
					jmp_num += (program[pc + 3] - '0');
					pc += 3;
					if (pointer - jmp_num >= tape_end) {
						pointer -= jmp_num;
					}
				} else {
					if (pointer > tape_end) {
						pointer--;
					}
				}
				break;
			case '[':
				if ((*pointer) == 0) {
					int depth = 1;
					while (depth != 0) {
						pc++;
						if (program[pc] == '[') {depth++; }
						if (program[pc] == ']') {depth--; }
					}
				}
				break;
			case ']':
				if ((*pointer) != 0) {
					int depth = 1;
					while (depth != 0) {
						pc--;
						if (program[pc] == ']') {depth++; }
						if (program[pc] == '[') {depth--; }
					}
				}
				break;
			case '{':
				depth = 1;
				int has_else = 0;
				int else_pos = -1;

				//find else if existing
				int steps = 0;
				while (program[pc] != '}') {
					steps++;
					pc++;
					if (program[pc] == ':') {
						if (debug) printf("Has Else!\n");
						has_else = 1;
						else_pos = pc;
						break;
					}
				}
				pc -= steps;

				steps = 0;
				while (program[pc] != '}') {
					pc++;
					steps++;
					if (program[pc] == '{') {
						depth++;
					}
				}
				pc -= steps;

				if (debug) printf("PC: %d\n", pc);

				int cmp_num = 0;
				cmp_num += (program[pc + 1] - '0') * 100;
				cmp_num += (program[pc + 2] - '0') * 10;
				cmp_num += (program[pc + 3] - '0');

				if (debug) printf("Cmp Num: %d\n", cmp_num);

				if ((*pointer) == cmp_num) {
					pc += 3;
				} else {
					if (debug) printf("Pointer %d is not %d\n", (*pointer), cmp_num);
					if (has_else) {
						pc = else_pos;
					} else {
						while (depth != 0) {
							pc++;
							if (program[pc] == '}') {
								depth--;
							}
						}		
					}
				}
				break;
			case ':':
				if (debug) printf("Loop depth: %d\n", depth);
				while (depth != 0) {
					pc++;
					if (program[pc] == '}') {depth--; }
				}
				if (debug) printf("Reached End! PC: %d\n", pc);
				break;
			case '.':
				printf("%c", (*pointer));
				break;
			case ',':
				(*pointer) = getchar();
				break;
			case '#':
				int jmp_num = 0;
				jmp_num += (program[pc + 1] - '0') * 1000;
				jmp_num += (program[pc + 2] - '0') * 100;
				jmp_num += (program[pc + 3] - '0') * 10;
				jmp_num += (program[pc + 4] - '0');
				pc = jmp_num - 1;
				break;
			default: 
				//Nothing
				break;
		} 
		pc++;
	}

	if (fptr != NULL) {
		fclose(fptr);
		free(file_buf);
	}
}
