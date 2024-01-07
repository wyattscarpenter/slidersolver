#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <limits.h>
#include <ctype.h>

#define DEBUG 0
#define dprintf(...) if(DEBUG){fprintf(stderr,__VA_ARGS__);}
#define DOBUG(...) if(DEBUG){__VA_ARGS__;}

#define HEIGHT 3
#define WIDTH 3
#define SIZE HEIGHT*WIDTH
#define RADIX 10 //we actually only need 9, since there are exactly 9 pieces and one is 0, but I've chosen 10 for clarity when printing the digits.
typedef unsigned char byte;
#define MAX_BOARDS 362880 //9 factorial
byte boards[MAX_BOARDS][SIZE];
unsigned int parents[MAX_BOARDS];
unsigned char is_already_inserted[1000000000]; // 10^9, ie RADIX^SIZE
unsigned int boards_length = 0;
unsigned int old_gen_mark = 0;

//0 is free space, other numbers go in english reading order in the sorted board and disregard free space
//we actually work backwards from desired[] in the code
//and try to find this initial[]
byte desired[] = {0,0,0, 0,0,0, 0,0,0};
byte initial[] = {0,0,0, 0,0,0, 0,0,0};

const byte neutral[] = {1,2,3,4,0,5,6,7,8};

int bz(const byte * b){//return free space of board
  for(int i = 0;i<SIZE; i++){
    if(b[i]==0){
      return i;
    }
  }
  dprintf("no zero in board\n");
  return -2;
}

int bprint(const byte * b){
  //just gonna hardcode this one lads
  return printf("-------\n|%d %d %d|\n|%d %d %d|\n|%d %d %d|\n-------\n",
		b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8]
	);
}

int bint(const byte * b){ //interprets a board as an integer, according to the RADIX
  int total = 0;
  for(int i = 0; i<SIZE;i++){
    total *= RADIX;
    total += b[i];
  }
  return total;
}

void printpath(int index){
  dprintf("I'm trying to print the path, here");
  bprint(boards[index]);
  if (index <= 0){
    exit(EXIT_SUCCESS);
  }
  printpath(parents[index]);
}

int beq(byte * r, byte * l){//test if boards equal
  for(int i = 0; i<SIZE;i++){
    if(r[i]!=l[i]){return 0;}
  }
  return 1;
}

void bcpy(byte * dst, const byte * src){
  if(!dst||!src){
    dprintf("null\n");
    return;
  }
  dprintf("copying %p to %p\n", src, dst);
  DOBUG(bprint(src);)
  dprintf("to\n");
  DOBUG(bprint(dst);)
  for(int i = 0; i<SIZE;i++){
    dst[i]=src[i];
  }
}

int bsmartinsert(byte * b, int parent){ //returns 0 (false) if no insertion was made, new length otherwise
  if(!b || is_already_inserted[bint(b)]){ // if b is null, or has already been inserted
    return 0;
  } else {
    is_already_inserted[bint(b)] = 1;
    dprintf("bsmartinsert\n");
    dprintf("inserting at board %d\n", boards_length);
    bcpy(boards[boards_length],b);
    parents[boards_length] = parent;
    if(beq(b, initial)){
      puts("You win!");
      printpath(boards_length);
    }
    return ++boards_length;
  }
}

byte * make_move(const byte * old, byte * new, int from_index, int to_index){
  bcpy(new,old);
  //0 1 2
  //3 4 5
  //6 7 8
  if (from_index < SIZE && from_index >= 0 &&
      to_index < SIZE && to_index >= 0 &&
      old[from_index] == 0 &&
      (from_index == to_index+WIDTH ||
       from_index == to_index-WIDTH ||
       ((from_index)%WIDTH !=0 && from_index == to_index+1) ||
       ((to_index)%WIDTH !=0 && to_index == from_index+1)
       )
      ){
    new[from_index] = old[to_index]; //move the non-zero back
    new[to_index] = old[from_index]; //put the zero in 
  }
  dprintf("resultant:\n");
  DOBUG(bprint(new));
  return new;
}

void make_moves(const byte * old, byte * up, byte * down, byte * left, byte * right){//given a board, returns by pointer the result of moving the free space the indicated direction (or the same board if impossible)
  dprintf("make moves\n");
  int z = bz(old);
  make_move(old,up,z,z-WIDTH);
  make_move(old,down,z,z+WIDTH);
  make_move(old,left,z,z-1);
  make_move(old,right,z,z+1);
}

void il(){ //il stands for "iterative loop".
  byte u[SIZE];
  byte d[SIZE];
  byte l[SIZE];
  byte r[SIZE];
  
  while(old_gen_mark < boards_length){
    dprintf("%d\n",old_gen_mark);
    make_moves(boards[old_gen_mark],u,d,l,r);
    bsmartinsert(u, old_gen_mark);
    bsmartinsert(d, old_gen_mark);
    bsmartinsert(l, old_gen_mark);
    bsmartinsert(r, old_gen_mark);
    old_gen_mark++;
  }
  printf("You lose. old_gen_mark: %d, boards_length: %d\n", old_gen_mark, boards_length);
}

int assertions(){ //just a bunch of tests of the integrity of my code
  dprintf("assertions\n");
  /*DOBUG(bprint(boards[13]));
    DOBUG(bprint(boards[14]));*/
  assert(beq(initial, initial));
  assert(beq(desired, desired));
  assert(!beq(initial, desired));
  
  byte b[] = {1,2,3,4,0,5,6,7,8};
  byte c[SIZE];
  bcpy(c,b);
  assert(beq(b,c));

  assert(bz(b) == 4);
  
  byte u[SIZE];
  byte ud[] = {1,0,3,4,2,5,6,7,8};
  byte d[SIZE];
  byte dd[] = {1,2,3,4,7,5,6,0,8};
  byte l[SIZE];
  byte ld[] = {1,2,3,0,4,5,6,7,8};
  byte r[SIZE];
  byte rd[] = {1,2,3,4,5,0,6,7,8};
  make_moves(b,u,d,l,r);
  //bprint(u);
  //bprint(ud);
  assert(beq(u,ud));
  assert(beq(d,dd));
  assert(beq(l,ld));
  //bprint(r);
  //bprint(rd);
  assert(beq(r,rd));
  assert(bint(neutral) == 123405678);
  dprintf("UINT_MAX: %ud, MAX_BOARDS: %ud\n", UINT_MAX, MAX_BOARDS);
  assert(MAX_BOARDS < UINT_MAX);
 
  return 1;
}

void print_usage_message_to_stderr() {
  fprintf(stderr, "Usage: slidersolver initial_board desired_board\n\
    Boards must be 3x3. (Whitespace is allowed in the arguments, but completely ignored.)\n\
    Example (this is a valid board that will win):\n\
      ./slidersolver 185423706 123456780\n\
    Another example (also a valid board that will win) (this argument input style is at least valid in posix shell):\n\
      ./slidersolver \\\n\
                   '823\n\
                    476\n\
                    501'\\\n \
                    \\\n\
                   '123\n\
                    405\n\
                    678'\n"
  );
}

#define EXIT_INVALID_ARGUMENTS 22

int main(int argc, char ** argv){
  //read input from command line, and parse it
  if (argc != 3) {
    print_usage_message_to_stderr();
    exit(EXIT_INVALID_ARGUMENTS);
  } else {
    byte * fillable_array [] = {NULL, initial, desired};
    for (int i = 1; i < argc; i++){
      int real_j = 0;
      for (int j = 0; argv[i][j]; j++){
        char c = argv[i][j];
        if (!isspace(c)) {
          fillable_array[i][real_j] = c - '0';
          real_j++;
          dprintf("%c", c);
        }
      }
      if (real_j != 9) {
        print_usage_message_to_stderr();
        printf("Reading input failed at argument %d (1-indexed), non-ignored (non-whitepace) character %d (0-indexed) which MIGHT be %c, if you didn't use any whitespace characters before it. Initial and desired boards, as read in:\n", i, real_j, argv[i][real_j]);
        bprint(initial);
        bprint(desired);
        exit(EXIT_INVALID_ARGUMENTS);
      }
    }
  }
  
  assertions();
  bsmartinsert(desired, 0);
  old_gen_mark = 0;
  boards_length = 1;
  dprintf("loop!\n");
  il();
  return EXIT_FAILURE;
}
