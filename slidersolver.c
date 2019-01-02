#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define DEBUG 0
#define dprintf(...) if(DEBUG){fprintf(stderr,__VA_ARGS__);}
#define DOBUG(...) if(DEBUG){__VA_ARGS__;}

#define HEIGHT 3
#define WIDTH 3
#define SIZE HEIGHT*WIDTH
typedef unsigned char byte;
#define MAX_BOARDS 362880 //9 factorial
byte boards[MAX_BOARDS][SIZE];
unsigned int parents[MAX_BOARDS];
unsigned int boards_length = 0;
unsigned int old_gen_mark = 0;

//0 is free space, index is eventual order disregarding space
byte desired[] = {1,2,3,4,0,5,6,7,8}; //we actually work backwards from this in the code
byte initial[] = {1,2,3,4,7,6,5,0,8}; //and try to find this

int bprint(const byte * b){
  //just gonna hardcode this one lads
  return printf("-------\n|%d %d %d|\n|%d %d %d|\n|%d %d %d|\n-------\n",
		b[0],b[1],b[2],b[3],b[4],b[5],b[6],b[7],b[8]
		);
}

void printpath(int index){
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

int bz(const byte * b){//return free space of board
  if(!b){
    return -1;
  }
  for(int i = 0;i<SIZE; i++){
    if(b[i]==0){
      return i;
    }
  }
  dprintf("no zero in board\n");
  return -2;
}

void bcpy(byte * dst, const byte * src){
  if(!dst||!src){
    dprintf("null\n");
    return;
  }
  dprintf("copying %lu to %lu\n", (unsigned long)src, (unsigned long)dst);
  DOBUG(bprint(src);)
  dprintf("to\n");
  DOBUG(bprint(dst);)
  for(int i = 0; i<SIZE;i++){
    dst[i]=src[i];
  }
}

int bsmartinsert(byte * b, int parent){
  //returns 0 (false) if no insertion was made, new length otherwise
  if(!b){
    return 0;
  } else {
    dprintf("bsmartinsert\n");
    int i = 0;
    while(i<boards_length){
      if(beq(boards[i],b)){return 0;}
      i++;
    }
    if(boards_length>=MAX_BOARDS){
      dprintf("max boards exceeded");
      dprintf("(theoretically impossible, check code correctness)");
      exit(EXIT_FAILURE);
    }
    dprintf("inserting at board %d\n", i);
    bcpy(boards[i],b);
    parents[i] = parent;
    if(beq(b, initial)){
      printpath(i);
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
}

int assertions(){
  dprintf("assertions\n");
  DOBUG(bprint(boards[13]));
  DOBUG(bprint(boards[14]));
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
 
  return 1;
}

int main(int argc, char ** argv){
  assertions();
  bsmartinsert(desired, -1);
  old_gen_mark = 0;
  boards_length = 1;
  dprintf("loop!\n");
  il();
  return EXIT_SUCCESS;
}
