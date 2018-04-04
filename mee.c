//Molly Seither
//OS
//Memory simulation: this uses four different algorithms for memory allocation.
//Reads in a file with each of the programs necessitating memory and allocates/
//deallocates accordingly, with respect to the given algorithm. Loads file into
//poperations array of program structures (type, value, and number). Manipulates
//said array in order to properly allocate memory, simultaneously counting the
//holes checked before actual alllocation/deallocation. Each algorithm returns
//the amount of holes checked in order to properly execute given a data file,
//data.txt.

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "prog.h"

//global
int blocks;
struct prog poperations[2000];
int toperations;

//function constructors
int firstFit();
int nextFit();
int bestFit();
int worstFit();

  //Parameters: none
  //Returns: number of holes
  //FirstFit assigns the first available space in memory. For each iteration, the
  //program restarts at the beginning of the memory, looking again for the first
  //available locaiton
  int firstFit(){
    //initialize variables
    int shifter;
    int store[blocks];
    int holes = 0;
    for (int i=0;i<blocks;i++){
      store[i]=-1;}
    //go through each program
    for (int j=0;j<toperations;j++){
      //allocated
      if (strcmp(poperations[j].type,"a")==0){
	int s = 0;
	for (int l=0;l<blocks;l++){
	  if (store[l] == -1){
	    s = s + 1;
	    if (s ==poperations[j].value){ //see if big enough
	      holes = holes + 1; //add to holes
	      for (int m=0;m<poperations[j].value;m++){
		shifter = l-m;
		store[shifter]=poperations[j].number;}
	      break;} //place in memory is found
	  }
	  else{ //end
	    if (s!=0){
	      holes = holes + 1;}
	    s = 0;} //resets
	}}
      //deallocated
      else if (strcmp(poperations[j].type,"d")==0){
	for (int k=0;k<blocks;k++){
	  if (store[k]==poperations[j].value){
	    store[k] = -1;}}
      }
    }
    return holes;}

  //Parameters: none
  //Returns: number of holes
  //NextFit functions similarly to FirstFit, but instead of returning to the beginning
  //of memory for each iteration, it continues from the last allocated memory spot.
  int nextFit(){
    //initialize variables
    int shifter;
    int store[blocks];
    int holes;
    int zero = 0;
    for (int i=0;i<blocks;i++){
      store[i]=-1;}
    //go through each program
    for (int j=0;j<toperations;j++){
      //deallocated
      if (strcmp(poperations[j].type,"d")==0){
	for (int k=0;k<blocks;k++){
	  if (store[k]==poperations[j].value){
	    store[k]=-1;}}} //removes from mem
      //allocated
      else if (strcmp(poperations[j].type,"a")==0){
	int s=0; //segment
	for (int l=0;l<blocks;l++){
	  //memory is not large enough
	  if (blocks == zero){
	    holes = holes + 1; //increment
	    zero = 0;
	    s = 0;}
	  if (store[zero] == -1){
	    s = s + 1; //increment
	    if (s=poperations[j].value){
	      holes = holes + 1; //checked hole, increment
	      for (int m=0;m<poperations[j].value;m++){
		shifter = zero - m;
		store[shifter] = poperations[j].number;}
	      break;} //memory found, kill
	    else{ //end of array
	      if (s!=0){
		holes = holes+1;}//checked hole, increment
	      s = 0;}
	    zero = zero + 1;}
	}
      }
      return holes;}
  }
  
  //Parameters: none
  //Returns: number of holes
  //BestFit works by trying to match the amount of memory necessary exactly with a
  //segment of that size. If there is no identical match, best fit acts accordingly,
  //finding the "best" fit, or that of the size closest to what is necessary. This
  //algorithm works by creating a list of all segments, and acting accordingly.
  int bestFit(){
    //initialize variables
    int shifter;
    int store[blocks];
    int holes = 0;
    int zero = 0; 
    for (int i=0;i<blocks;i++){ //allocates memory
      store[i]=-1;}
    //saves spots for tracking purposes
    int sindex[blocks+1];
    int slist[blocks+1];
    int ssize = 0;
    int sbest = -1;
    //go through each program
    for (int j=0;j<toperations;j++){
      //deallocated
      if (strcmp(poperations[j].type, "d")==0){
	for (int k=0;k<blocks;k++){
	  if (store[k]==poperations[j].value){
	    store[k] = -1;}}
      }
      //allocated
      else if (strcmp(poperations[j].type, "a") == 0){
	int s = 0; //segment
	for (int l=0;l<blocks;l++){
	  if (store[l] == -1){
	    ssize = ssize + 1;
	    int ending = blocks - 1;
	    if (l==ending){ //stores segment
	      sindex[zero]=ssize;
	      slist[zero] = l;
	      zero = zero+1;}}
	  else{ //end of a segment
	    int curr = l-1;
	    if (ssize > 0){ //stores segment
	      sindex[zero]=ssize;
	      slist[zero] = curr;
	      zero = zero+1;}
	    ssize = 0;}} //reset
	for (int m=0;m<zero;m++){
	  holes = holes + 1; //hole checked, increment
	  if (sbest == -1){ //find next hole
	    if (sindex[m]>=poperations[j].value){
	      sbest = m;
	      if (sindex[sbest]!=poperations[j].value){ //perfect fit
	      }
	      else{
		break;} //memory found, kill
	    }}
	  else{
	    if (sindex[m]>=poperations[j].value){
	      sbest = m;
	      if (sindex[sbest]!=poperations[j].value){ //perfect fit
	      }
	      else{
		break;}} //memory found, kill
	  }}
      }
      //assigns memoryy to best/optimal
      for (int x=0;x<poperations[j].value;x++){ 
	store[slist[sbest]-x]=poperations[j].number;}
    }
    return holes;}

//Parameters: none
//Returns: number of holes
//WorstFit creates a list of segments then uses said list to determine the "worst" fit
//for each, meaning a small segment goes in a large memory frame.
int worstFit(){
  //initialize variables
  int shifter;
  int store[blocks];
  int holes = 0;
  int zero = 0;
  for (int i=0;i<blocks;i++){ //allocates memory
    store[i]=-1;}
  //saves spots
  int sindex[blocks+1];
  int slist[blocks+1];
  int ssize = 0;
  int sbest = -1;
  //go through each program
  for (int j=0;j<toperations;j++){
    //deallocated
    if (strcmp(poperations[j].type, "d")==0){
      for (int k=0;k<blocks;k++){
	if (store[k]==poperations[j].value){
	  store[k] = -1;}}
    }
    //allocated
    else if (strcmp(poperations[j].type, "a") == 0){
      int s = 0; //segment
      for (int l=0;l<blocks;l++){
	if (store[l] == -1){
	  ssize = ssize + 1;
	  int ending = blocks - 1; //memory filled, end segment
	  if (l==ending){
	    sindex[zero]=ssize; //store in mem
	    slist[zero] = l;
	    zero = zero+1;}}
	else{ //end of segment
	  int curr = l-1;
	  if (ssize > 0){
	    sindex[zero]=ssize; //store in mem
	    slist[zero] = curr;
	    zero = zero+1;}
	  ssize = 0;}}
      for (int m=0;m<zero;m++){
	holes = holes + 1; //hole checked, increment
	if (sbest == -1){
	  if (sindex[m]>=poperations[j].value){ //next hole
	    sbest = m;}}
	else{
	  if (sindex[m]>=poperations[j].value){ //compares to current
	    if (sindex[m]> sindex[sbest]){
	      sbest = m;}}}
      }
      //assigns memory
      for (int x=0;x<poperations[j].value;x++){
	store[slist[sbest]-x]=poperations[j].number;}}}
  return holes;}


int main(void){
  //reading in file
  FILE * file;
  file = fopen("./data.txt","r");
  int one, counter, programs, changed; //initialize necessary variables
  one = 1; 
  counter = 0;
  programs = 0;
  changed = -1;
  char type[1];
  struct prog pro; //program struct
  fscanf(file, "%d", &blocks);
  while(fscanf(file, "%s %d", type, &changed) !=EOF){
    //for allocation
    if (strcmp(type,"a")==0){
      type[strcspn(type, "\n")]=0;
      strcpy(pro.type, type);
      pro.value = changed;
      pro.number = counter;
      counter++;}
    //for deallocation
    else if(strcmp(type,"d")==0){
      type[strcspn(type,"\n")]=0;
      strcpy(pro.type,type);
      pro.value = changed;
      pro.number= -1;}
    //add program to poperations array
    poperations[programs]= pro;
    programs++;} //increment
  toperations = programs;
  fclose(file);
  
  //run each algorithm and print result
  int ff, nf, bf, wf;
  ff = firstFit();
  printf("First Fit: %d\n", ff);
  nf = nextFit();
  printf("Next Fit: %d\n", nf);
  bf = bestFit();
  printf("Best Fit: %d\n", bf);
  wf = worstFit();
  printf("Worst Fit: %d\n", wf);                                        

}
