#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main() {
  srand(time(0));
  //shapes declaration
  int random,map_size=2,maxbottom,minbottom,row_check,col_check,resume=1,score=0,maxscore=0,gameover=0,counter=0,counter2,right,left,dropcolumn,stop,rotation,j,k;
  int map[50][50],shapes[2][4][4],rows2delete[4],origin[2],bottom[4];
  char ans[2]; //answer string
  while (map_size<5) {
    printf("Enter the board length (greater than 4):");scanf("%d",&map_size);
  }
  printf("Welcome to TETRIS\n-Shapes rotate clockwise\n-Column is defined according to the star (*)\n");
  while (resume==1) {
  for (size_t i = 0; i < map_size; i++) {
    for (size_t k = 0; k < map_size; k++) {
      map[i][k]=0;
    }
  }
  while (gameover==0) {
    //reset deleted rows and shape
    for (size_t i = 0; i < 4; i++) {
      rows2delete[i]=-1;
    }
    for (size_t i = 0; i < 4; i++) {
      for (size_t k = 0; k < 4; k++) {
        shapes[0][i][k]=0;
      }
    }
    //"create" a new random shape
    j=1;k=0;
    shapes[0][j][k]=2;
    for (size_t i = 1; i < 4; i++) {
      random=rand()%3;
      if (random==2) {
        //right
        k++;
        shapes[0][j][k]=1;
      } else if (random==1) {
        //up
        j--;
        if (j<0) {
          j++;
          k++;
          shapes[0][0][k]=1;
        } else if (shapes[0][j][k]!=0) {
          k++;
          shapes[0][j][k]=1;
        } else {
          shapes[0][j][k]=1;
        }
      } else {
        //down
        j++;
        if (j>3) {
          j--;
          k++;
          shapes[0][3][k]=1;
        } else if (shapes[0][j][k]!=0) {
          k++;
          shapes[0][j][k]=1;
        } else {
          shapes[0][j][k]=1;
        }
      }
    }
    printf("This is the shape dropping:\n");
    for (size_t i = 0; i < 4; i++) {
      for (size_t k = 0; k < 4; k++) {
        if (shapes[0][i][k]==1) {
          printf("+ ");
        } else if (shapes[0][i][k]==2) {
          printf("* ");
        } else {
          printf("  ");
        }
      }
      printf("\n");
    }
    //rotate the shape using the [1]th layer temporarly
    rotation=1;
    while (rotation==1) {
      printf("Gonna rotate that? (y/n): ");scanf("%s",&ans[0]);
      if (ans[0]=='y') {
        for (size_t i = 0; i < 4; i++) {
          for (size_t k = 0; k < 4; k++) {
            shapes[1][i][k]=shapes[0][3-k][i];
          }
        }
        for (size_t i = 0; i < 4; i++) {
          for (size_t k = 0; k < 4; k++) {
            shapes[0][i][k]=shapes[1][i][k];
            if (shapes[0][i][k]==1) {
              printf("+ ");
            } else if (shapes[0][i][k]==2) {
              printf("* ");
            } else {
              printf("  ");
            }
          }
          printf("\n");
        }
      } else {
        rotation=0;
      }
    }
    //find the origin coordinates
    counter=counter2=0;origin[0]=origin[1]=-1;
    while (origin[0]==-1) {
      while (origin[0]==-1 && counter2<4) {
        if(shapes[0][counter][counter2]==2) {
          origin[0]=counter;origin[1]=counter2;
        } else {
          counter2++;
        }
      }
      if (origin[0]==-1) {
        counter2=0;counter++;
      }
    }
    //define an array with the bottom coordinates
    for (size_t i = 0; i < 4; i++) {
      bottom[i]=-1;
    }
    for (size_t i = 0; i < 4; i++) {
      counter=0;
      while (counter<4 && bottom[i]==-1) {
        if (shapes[0][3-counter][i]!=0) {
          bottom[i]=3-counter;
        }
        counter++;
      }
    }
    maxbottom=0;minbottom=0; //get the max bottom
    for (size_t i = 1; i < 4; i++) {
      if (bottom[i]>bottom[maxbottom]) {
        maxbottom=i;
      }
      if (bottom[i]>-1 && (bottom[i]<bottom[minbottom] || bottom[minbottom]==-1)) {
        minbottom=i;
      }
    }
    //get the length to the right and to the left
    right=0;left=0;counter=origin[1]+1;
    while (bottom[counter]!=-1 && counter<4) {
      right++;
      counter++;
    }
    counter=origin[1]-1;
    while (bottom[counter]!=-1 && counter>=0) {
      left++;
      counter--;
    }
    //print the map
    printf("\n**T.E.T.R.I.S**\n");
    for (size_t i = 0; i < 2; i++) {
      if (i==0) {
        for (size_t k = 0; k < map_size; k++) {
          printf("%ld ",k+1);
        }
      } else {
        for (size_t k = 0; k < map_size; k++) {
          printf("_ ");
        }
      }
      printf("\n");
    }
    for (size_t i = 1; i < map_size; i++) {
      for (size_t k = 0; k < map_size; k++) {
        if (map[i][k]==0) {
          printf("  ");
        } else {
          printf("+ ");
        }
      }
      printf("\n");
    }
    for (size_t i = 0; i < 2; i++) {
      if (i==0) {
        for (size_t k = 0; k < map_size; k++) {
          printf("_ ");
        }
      } else {
        for (size_t k = 0; k < map_size; k++) {
          printf("%ld ",k+1);
        }
      }
      printf("\n");
    }
    printf("\n");
    //print the final shape
    for (size_t i = 0; i < 4; i++) {
      for (size_t k = 0; k < 4; k++) {
        if (shapes[0][i][k]==1) {
          printf("+ ");
        } else if (shapes[0][i][k]==2) {
          printf("* ");
        } else {
          printf("  ");
        }
      }
      printf("\n");
    }
    //ask for dropcolumn
    counter=0;
    while (counter==0) {
      printf("Column no. to drop the shape from (1-%d): ",map_size);scanf("%d",&dropcolumn);
      if (map_size-dropcolumn>=right && dropcolumn>left) {
        counter=1;
      } else {
        printf("\nE: The shape exceeds the map if dropped from there.\nAre you sure that you checked the map?\n\n");
      }
    }
    //check the map for stop column
    counter=0;counter2=0;
    while (bottom[counter]==-1) {
      counter++;
    }
    stop=0;
    while (stop==0 && counter2<map_size) {
      for (size_t i = counter; i < right+origin[1]+1; i++) {
        /*counter2: row number, counter: shape starting column in shapes matrix
        i: columns to check */
        row_check=counter2+bottom[i]-bottom[maxbottom];
        col_check=dropcolumn-1-left+i-counter;
        if (map[row_check][col_check]!=0 && row_check>-1) {
          stop=1;counter2--;
        } else if (row_check<0 && counter2+1-bottom[maxbottom]+bottom[minbottom]>0) {
          gameover=1;stop=1;counter--;
        }
      }
      counter2++;
    }
    //defining new map with the shape dropped
    for (size_t i = 0; i <bottom[maxbottom]+1; i++) {
      for (size_t k = dropcolumn-left; k < dropcolumn+right+1; k++) {
        if (shapes[0][bottom[maxbottom]-i][counter+k-dropcolumn+left]!=0) {
          map[counter2-1-i][k-1]=1;
        }
      }
      printf("\n");
    }
    //check if game is over
    for (size_t i = 0; i < map_size; i++) {
      if(map[0][i]!=0) {
        gameover=1;
      }
    }
    //delete filled rows
    if (gameover==0) {
      counter2=-1;
      for (size_t i = 1; i < map_size; i++) {
        counter=0;
        while (map[map_size-i][counter]!=0 && counter<map_size) {
          counter++;
        }
        if (counter==map_size) {
          rows2delete[counter2+1]=map_size-i;
          counter2++;
        }
      }
      while (rows2delete[counter2]!=-1 && counter2>-1) {
        score=score+100;
        for (size_t i = rows2delete[counter2]; i > 0; i--) {
          for (size_t k = 0; k < map_size; k++) {
            map[i][k]=map[i-1][k];
          }
        }
        counter2--;
      }
    }
    //print the map
    printf("\n**T.E.T.R.I.S**\n");
    for (size_t i = 0; i < 2; i++) {
      if (i==0) {
        for (size_t k = 0; k < map_size; k++) {
          printf("%ld ",k+1);
        }
      } else {
        for (size_t k = 0; k < map_size; k++) {
          printf("_ ");
        }
      }
      printf("\n");
    }
    for (size_t i = 1; i < map_size; i++) {
      for (size_t k = 0; k < map_size; k++) {
        if (map[i][k]==0) {
          printf("  ");
        } else {
          printf("+ ");
        }
      }
      printf("\n");
    }
    for (size_t i = 0; i < 2; i++) {
      if (i==0) {
        for (size_t k = 0; k < map_size; k++) {
          printf("_ ");
        }
      } else {
        for (size_t k = 0; k < map_size; k++) {
          printf("%ld ",k+1);
        }
      }
      printf("\n");
    }
    printf("=========\nscore:%d\n=========\n",score);
  }
  printf("GAME OVER\n");
  if (score>=maxscore) {
    maxscore=score;
    printf("Score: %d. You hold the highest score. Congrats!\n",score);
  } else if (score>600) {
    printf("Highest score: %d\nYour score: %d. Keep it up :)\n",maxscore,score);
  } else {
    printf("Highest score: %d\nYour score: %d. Shame on you.\n",maxscore,score);
  }
  printf("Play again? (y/n):");scanf("%s",&ans[0]);
  if (ans[0]!='y') {
    resume=0;
  } else {
    gameover=0;
    score=0;
  }
  }
  return 0;
}
