#include<stdio.h>
#include<time.h>
#include<termios.h>
#include<stdlib.h>
//Global variables
int h,px,py,m=0,s=0,ex,ey,lvl=1,r=11,c=11;
char **maze = NULL; //double pointer for storing the dynamically allocated 2D grid maze
		    //Dynamic Memory Allocation
void mem(){
	maze = (char **)malloc(r*sizeof(char *));
	for (int i=0;i<r;i++){
		maze[i] = (char *)malloc(c*sizeof(char));
	}
}
//Cleaning Allocated Memory
void fm(){
	if (maze != NULL){
		for(int i=0;i<r;i++){
			free(maze[i]);
		}
		free(maze);
		maze = NULL;
	}
}
//Maze Generator (Randomised DFS + Backtracking )
void gmp(int cx, int cy) {
	//Vector Tracking Offset Pairs Mapping
	int dx[] = {0, 0, -2, 2};
	int dy[] = {-2, 2, 0, 0};
	int dirs[] = {0, 1, 2,3}; //Index directions key maps
				  //Fisher-Yates Shuffle Algo
	for (int i = 3; i > 0; i--) {
		int j = rand() % (i + 1);
		int t = dirs[i];
		dirs[i] = dirs[j];
		dirs[j] = t;;
	}
	for (int i = 0; i < 4; i++) {
		int nx = cx + dx[dirs[i]];
		int ny = cy + dy[dirs[i]];
		if (nx > 0 && nx < c - 1 && ny > 0 && ny < r - 1) {
			if (maze[ny][nx] == '#') {
				maze[cy + dy[dirs[i]] / 2][cx + dx[dirs[i]] / 2] = ' '; //carving exit path
				maze[ny][nx] = ' ';
				gmp(nx, ny);
			}
		}
	}
}
//Backtracking due no more avilable options
//level progression
void slvl() {
	fm();
	c = 11 + (lvl - 1) * 4;
	r = 11 + (lvl - 1) * 4;
	if (c > 99)
		c = 99;
	if (r > 99) 
		r = 99;
	mem();//allocating fresh memory
	for (int y = 0; y < r; y++) {
		for (int x = 0; x < c; x++) {
			maze[y][x] = '#';
		}
	}
	maze[1][1] = ' ';//starting channel
	gmp(1, 1);//maze mapping
	py = 1; 
	px = 1;
	ex = c - 2; 
	ey = r - 2;
	maze[py][px] = '$';//player spawn 
	maze[ey][ex] = ')';//target spawn
}
//Display
void pm(){
	system("clear");//linux display clear
	printf("=================================\n");
	printf("        MAZE RUNNER - LEVEL %d    \n", lvl);
	printf("        Grid Dimensions: %dx%d   \n", c, r);
	printf("        Moves Taken: %d   \n", m);
	printf("        Strikes: %d / 3 (%s)   \n",s,(s==0)?"~~~":(s==1)?"~~*":(s==2)?"*~*":"***");
	printf("=================================\n");
	printf("Controls: W/A/S/D | Press Q to Quit\n\n");
	//warnings
	if (h==1)
		puts("Can't Phase Through Walls\n ");
	else if (h==2)
		puts("Invalid Command\n");
	else
		printf("\n\n");
	int i,j;
	for (i=0;i<r;i++)
	{	
		for (j=0;j<c;j++)
		{
			printf(" %c",maze[i][j]);
		}
		printf("\n");
	}
}
//Movement system 
int move(int nx,int ny){
	//keeping player in bounds
	if (nx>=0 && nx<c && ny>=0 && ny<r){
		if (maze[ny][nx] != '#'){
			//clearing trails , changing player position
			maze[py][px] = ' ';
			px=nx;
			py=ny;	
			maze[py][px] = '$';
			m++;
			return 1; //successful movement 
		}
	}
	return 0; //in case of collision 
}
int main(){	
	srand((unsigned int)time(NULL)); //pseudo-random engine via time value
	slvl(); //generation of level 1 map parameters
	char in;
	while (1) {
		pm(); //displaying maze
		      //strike system for collision and wrong input
		if (s>=3){
			printf("\nGAME OVER! Too many mistakes. Restarting from Level 1...\n");
			lvl = 1;
			s = 0;
			m = 0;
			h = 0;
			printf("Press Enter to Try Again...\n");
			while(getchar() != '\n');
			getchar();
			slvl();
			continue;
		}
		//Winning level verification
		if(px==ex && py==ey){
			printf("\n level %d cleared! Advancing Forward",lvl);
			lvl++;
			h=0;
			s=0;
			printf("\nPress Enter to Continue");
			while(getchar() != '\n');
			getchar();
			slvl();
			continue;	
		}
		printf("Enter Action\n");
		int r=scanf(" %c", &in);
		while(getchar() !='\n');
		if (r!=1)
			continue;
		if (in == 'q' || in == 'Q') {
			printf("Thanks for playing!\n");
			break;
		}
		//Navigation systems and strike  systems
		switch (in) {
			case 'w': case 'W': 
				h=!move(px, py - 1); 
				if(h)
					s++;
				break;
			case 's': case 'S': 
				h=!move(px, py + 1); 
				if(h)
					s++;
				break;
			case 'a': case 'A': 
				h=!move(px - 1, py); 
				if(h)
					s++;
				break;
			case 'd': case 'D': 
				h=!move(px + 1, py); 
				if(h)
					s++;
				break;
			default : //for wrong inputs 
				h=2;
				s++;
				break;
		}
	}
	fm(); //final heap data block collections, sweeps before exit
}
