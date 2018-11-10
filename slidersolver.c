#include <stdlib.h>
#include <stdio.h>

#define HEIGHT 3
#define WIDTH 3
typedef unsigned char byte;
typedef byte board[HEIGHT*WIDTH]; //this will decay to a pointer, mind!
#define MAX_BOARDS 362880 //9 factorial
board boards[MAX_BOARDS]; 
int boards_length = 0;

//0 is free space, index is eventual order disregarding space
board initial = {1,2,3,0,4,5,6,7,8};
board desired = {0,2,3,1,4,5,6,7,8};

int beq(board r, board l){//test if boards equal
  for(int i = 0; i<sizeof(board);i++){
    if(r[i]!=l[i]){return 0;}
  }
  return 1;
}

int bz(board b){//return free space of board
  int i = 0;
  while(b[i] && i<sizeof(board)){
    i++;
  }
  return i;
}

void bcpy(board dst, board src){ //since boards always pass by reference, this fn is necessary.
  for(int i = 0; i<sizeof(board);i++){
    dst[i]=src[i];
  }
}

int bsmartinsert(board b){
  //returns 0 (false) if no insertion was made, new length (!=0, true) otherwise
  int i = 0;
  for(; i<boards_length; i++){
    if(beq(boards[i],b)){return 0;}
  }
  if(boards_length>MAX_BOARDS){
    puts("boards length exceeded (this is theoretically impossible, check code correctness)");
    exit(1);
  }
  bcpy(boards[i],b);
  return ++boards_length;
}

board * make_move(board old, board * new, int from_index, int to_index){
  if (from_index < sizeof(board) && from_index >= 0 &&
      to_index < sizeof(board) && to_index >= 0 &&
      old[from_index] == 0 &&
      (from_index == to_index+WIDTH ||
       from_index == to_index-WIDTH ||
       (from_index/WIDTH !=0 && from_index == to_index+1) ||
       (from_index/WIDTH !=1 && from_index == to_index-1)
       )
      ){
    bcpy(*new,old);
    *new[to_index] = old[from_index];
    *new[from_index] = 0;
    return new;
  }
  //error condition
  return NULL;
}

void make_moves(board old, board * up, board * down, board * left, board * right){//given a board, returns by pointer the result of moving the free space the indicated direction (or null if impossible)
  int z = bz(old);
  make_move(old,up,z,z-WIDTH);
  make_move(old,down,z,z+WIDTH);
  make_move(old,left,z,z-1);
  make_move(old,right,z,z+1);
}

int rl(board b){ //rl stands for "recursive loop". or maybe for "rl", I forget which
  board u;
  board d;
  board l;
  board r;
  if(!b){
    return 0;
  }
  if(beq(b, desired)){
    return 1;
  }
  if(bsmartinsert(b)){
    make_moves(b,&u,&d,&l,&r);
  }
  return (rl(u) || rl(d) || rl(l) || rl(r));
}

int main(int argc, char ** argv){
  return rl(initial);
}
