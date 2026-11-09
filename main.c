#include <stdint.h>
#include <stdio.h>

char* program;
int pc = 0;

int if_depth[1024];
int else_id = 0;
int global_if_depth = 0;

#define TAPE_SIZE 80000
uint8_t tape[TAPE_SIZE];
uint8_t* tape_start = (uint8_t*)&tape;
uint8_t* tape_end = (uint8_t*)&tape + (TAPE_SIZE - 1);
uint8_t* pointer = (uint8_t*)&tape;

#define DEBUG 0

void debug_print() {
	printf(
	"\033[33m=====DEBUG=====\nPC:                %d\n->Content:         %c\nPointer(absolute): 0x%08x\nPointer(relative): 0x%08x\n=====DEBUG=====\033[0m\n", pc, program[pc], pointer, pointer - tape_start);
}

int main(int argc, char* argv[]) {
	if (argc != 2) {
		return 1;
	}

	program = argv[1];

	int next_char_is_num = 0;
	while (program[pc] != '\0') {
		if (DEBUG) debug_print();
		if (program[pc + 1] >= 48 && program[pc + 1] <= 57) {
			next_char_is_num = 1;
		} else {
			next_char_is_num = 0;
		}
		switch (program[pc]) {
			case '+':
				;
				int add_num = 1;
				if (next_char_is_num) {
					add_num = 0;
					add_num += (program[pc + 1] - '0') * 100;
					add_num += (program[pc + 2] - '0') * 10;
					add_num += (program[pc + 3] - '0');
					pc += 3;
				}
				(*pointer) += add_num;
				break;
			case '-':
				;
				int sub_num = 1;
				if (next_char_is_num) {
					sub_num = 0;
					sub_num += (program[pc + 1] - '0') * 100;
					sub_num += (program[pc + 2] - '0') * 10;
					sub_num += (program[pc + 3] - '0');
					pc += 3;
				}
				(*pointer) -= sub_num;
				break;
			case '<':
				;
				int left_mov_num = 1;
				if (next_char_is_num) {
					left_mov_num = 0;
					left_mov_num += (program[pc + 1] - '0') * 100;
					left_mov_num += (program[pc + 2] - '0') * 10;
					left_mov_num += (program[pc + 3] - '0');
					pc += 3;
				}
				if (pointer - left_mov_num >= tape_start) {
					pointer -= left_mov_num;
				}
				break;
			case '>':
				;
				int right_mov_num = 1;
				if (next_char_is_num) {
					right_mov_num = 0;
					right_mov_num += (program[pc + 1] - '0') * 100;
					right_mov_num += (program[pc + 2] - '0') * 10;
					right_mov_num += (program[pc + 3] - '0');
					pc += 3;
				}
				if (pointer + right_mov_num <= tape_end) {
					pointer += right_mov_num;
				}
				break;
			case '.':
				if (DEBUG) {
					printf("\033[36mPrint: %c\033[0m\n", (*pointer));
				} else {
					printf("%c", (*pointer));
				}
				break;
			case ',':
				(*pointer) = getchar();
				break;
			case '[':
				if ((*pointer) == 0) {
					int depth = 1;
					while (depth != 0) {
						pc++;
						if (program[pc] == '[') depth++;
						if (program[pc] == ']') depth--;
					}
				}
				break;
			case ']':
				if ((*pointer) != 0) {
					int depth = 1;
					while (depth != 0) {
						pc--;
						if (program[pc] == ']') depth++;
						if (program[pc] == '[') depth--;
					}
				}
				break;
			case '{':
				;
				int steps = 0;
				int depth = 1;
				global_if_depth++;
				int cmp_num = 0;
				cmp_num += (program[pc + 1] - '0') * 100;
				cmp_num += (program[pc + 2] - '0') * 10;
				cmp_num += (program[pc + 3] - '0');
				pc += 3;

				int found_else = 0;
				while (depth != 0 && program[pc] != '\0') {
					pc++;
					steps++;
					if (program[pc] == '{') depth++;
					if (program[pc] == ':') depth--;
				}
				pc -= steps;
				if (depth == 0) {
					found_else = 1; 
					if (DEBUG) printf("\033[36mFound Else!\033[0m\n");
				}
				depth = 1;

				if (cmp_num != (*pointer) && found_else) {
					while (depth != 0) {
						pc++;
						if (program[pc] == '{') depth++;
						if (program[pc] == ':') depth--;
					}
				} else if (cmp_num != (*pointer) && !found_else) {
					while (depth != 0) {
						pc++;
						if (program[pc] == '{') depth++;
						if (program[pc] == '}') depth--;
					}
				}
				break;
			case '}':
				global_if_depth++;
				break;
			case ':':
				;
				int local_if_depth = global_if_depth;
				while (local_if_depth != 0) {
					pc++;
					if (program[pc] == '{') local_if_depth++;
					if (program[pc] == '}') local_if_depth--;
				}
				break;
			case '#':
				;
				int jmp_num = 0;
				jmp_num += (program[pc] - '0') * 1000;
				jmp_num += (program[pc] - '0') * 100;
				jmp_num += (program[pc] - '0') * 10;
				jmp_num += (program[pc] - '0');
				pc = jmp_num - 1;
				break;
			default:
				;
				//Nothing
		}

		pc++;
	}
}
