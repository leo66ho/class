#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <array>
#include <limits.h>
#include <map>
#include <math.h>
#include<sstream>
using namespace std;
enum SPOT_STATE {
    EMPTY = 0,
    BLACK = 1,
    WHITE = 2
};
map<int,int> boardstatus;
const int SIZE = 15;
const int INFINITE = 1000000;


class State{
    public:
    array<array<int,SIZE>,SIZE> board;
    int bvalue;
    //int depth;
    //bool ourturn;
    array<int,SIZE> hor;
    array<int,SIZE> ver;
    array<int,SIZE*2> ldig;
    array<int,SIZE*2> rdig;
   

    State(array<array<int,SIZE>,SIZE> board){
        
        for(int x=0;x<SIZE;x++){
            for(int y=0;y<SIZE;y++){
                this->board[x][y] = board[x][y];
            }
        }
        cout<<"Stated"<<endl;
        bvalue = 0; 
        setvalue();
        
    }

    State(State &rhs){
        for(int x=0;x<SIZE;x++){
            for(int y=0;y<SIZE;y++){
                this->board[x][y] = rhs.board[x][y];
            }
        }
        bvalue = rhs.bvalue;
        this->hor = rhs.hor;
        this->ver = rhs.ver;
        this->ldig = rhs.ldig;
        this->rdig = rhs.rdig;
    }

    void update(int disc,int x,int y){
        board[x][y] = disc;
        changevalue(x,y);

    }
    void setvalue();
    void changevalue(int x,int y);


};

int player;
std::array<std::array<int, SIZE>, SIZE> board;


void read_board(std::ifstream& fin) {
    fin >> player;
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            fin >> board[i][j];
            boardstatus[15*i+j] = board[i][j];
        }
    }
}

int seqvalue(int cnt,int head,int tail,int disk){
   if(disk == 0){return 0;}
   if(head == tail && disk != 0){return 0;}
   if(cnt == 5){return INFINITE;}
   if(head==tail){
     if(cnt == 4){return (player == disk)? 100:-100;}
     else if(disk == player){return cnt*2;}
     else{return -(cnt*2);}
   }
   else{
     if(disk == player){return cnt;}
     else{return -cnt;}
   }      
}

void State:: setvalue(){
    //int value = 0;
    int disk = -1,head,tail;
    int cnt;
    for(int i=0;i<SIZE;i++){
        head = -1,cnt = 1;
        disk = board[i][0];
        hor[i] = 0;
        for(int j=1;j<SIZE;j++){
            
            if(board[i][j] == disk){
                cnt++;
            }
            else if(board[i][j] != disk){
                tail = board[i][j];
                hor[i] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[i][j];
                cnt = 1;
            }
        }
        tail = -1;
        hor[i] += seqvalue(cnt,head,tail,disk);
        cout<<hor[i]<<endl;
        bvalue+=hor[i];
    }
    for(int j=0;j<SIZE;j++){
        head = -1,cnt = 1;
        ver[j] = 0;
        disk = board[0][j];
        for(int i=1;i<SIZE;i++){
            
            if(board[i][j] == disk){
                cnt++;
            }
            else if(board[i][j] != disk){
                tail = board[i][j];
                ver[j] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[i][j];
                cnt = 1;
            }
        }
        tail = -1;
        ver[j] += seqvalue(cnt,head,tail,disk);
        cout<<ver[j]<<endl;
        bvalue += ver[j];
    }
    for(int a=0;a<SIZE-1;a++){
        int b=0;
        int c=a;
        ldig[a-b] = 0;
        head = -1,cnt = 1;
        disk = board[a][b];
        while(c<SIZE && b<SIZE && c>=0 && b>=0){
             if(board[c][b] == disk){
                cnt++;
            }
            else if(board[c][b] != disk){
                tail = board[c][b];
                ldig[a-b] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[c][b];
                cnt = 1;
        }
        c++;
        b++;
        }
        tail = -1;
        ldig[a-b] += seqvalue(cnt,head,tail,disk);
        //cout<<ldig[a-b]<<endl;
        bvalue+=ldig[a-b];
    }
    for(int b=0;b<SIZE-1;b++){
        int a=0;
        int c=b;
        ldig[14-(a-b)] = 0;
        head = -1,cnt = 1;
        disk = board[a][b];
        while(a<SIZE && c<SIZE && a>=0 && b>=0){
             if(board[a][c] == disk){
                cnt++;
            }
            else if(board[a][c] != disk){
                tail = board[a][c];
                ldig[14-(a-b)] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][c];
                cnt = 1;
            a++;
            c++;
        }
        tail = -1;
        ldig[14-(a-b)] += seqvalue(cnt,head,tail,disk);
        //cout<<ldig[14-(a-b)]<<endl;
        bvalue+=ldig[14-(a-b)];
        }
    }
    for(int a=0;a<SIZE-1;a++){
        int b=0;
        int c=a;
        head = -1,cnt = 1;
        rdig[a+b] = 0;
        disk = board[a][b];
        while(c<SIZE && b<SIZE && c>=0 && b>=0){
             if(board[c][b] == disk){
                cnt++;
            }
            else if(board[c][b] != disk){
                tail = board[c][b];
                rdig[a+b] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[c][b];
                cnt = 1;
            c--;
            b++;
        }
        tail = -1;
        rdig[a+b] += seqvalue(cnt,head,tail,disk);
        //cout<<rdig[a+b]<<endl;
        bvalue+=rdig[a+b];
        }
    }
    for(int b=0;b<SIZE-1;b++){
        int a=0;
        int c=b;
        head = -1,cnt = 1;
        rdig[a+b] = 0;
        disk = board[a][b];
        while(a<SIZE && c<SIZE && a>=0 && c>=0){
            if(board[a][c] == disk){
                cnt++;
            }
            else if(board[a][c] != disk){
                tail = board[a][c];
                rdig[a+b] += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][c];
                cnt = 1;
            a--;
            c++;
        }
        tail = -1;
        rdig[a+b] += seqvalue(cnt,head,tail,disk);
        //cout<<rdig[a+b]<<endl;
        bvalue+=rdig[a+b];
        }
    }
}

void State:: changevalue(int x,int y){
    int disk,head,tail;
    int cnt = 0;
    int value = 0;
    head = -1,cnt = 1;
    disk = board[x][0];
    for(int k=1;k<SIZE;k++){
            
            if(board[x][k] == disk){
                cnt++;
            }
            else if(board[x][k] != disk){
                tail = board[x][k];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[x][k];
                cnt = 1;
            }
        }
    tail = -1;
    value += seqvalue(cnt,head,tail,disk);
    bvalue = bvalue-hor[x]+value;
    hor[x] = value;


    for(int k=1;k<SIZE;k++){
            
            if(board[k][y] == disk){
                cnt++;
            }
            else if(board[k][y] != disk){
                tail = board[k][y];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[k][y];
                cnt = 1;
            }
        }
    tail = -1;
    value += seqvalue(cnt,head,tail,disk);
    bvalue = bvalue-ver[y]+value;
    ver[y] = value;

    int i = x-y;
    if(i<=0){
        int a = 0;
        int value = 0;
        int b = -(x-y);
        while(a<SIZE && b<SIZE){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][b];
                cnt = 1;
            a++;
            b++;
        }
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-ldig[14-i]+value;
        ldig[14-i] = value;
    }
    else{
        int a = x-y;
        int b = 0;
        while(a<SIZE && b<SIZE){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][b];
                cnt = 1;
            a++;
            b++;
        }
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-ldig[i]+value;
        ldig[i] = value;
    }
    


    int j = x+y;
    if(j<=SIZE-1){
        int a = 0;
        int b = x+y;
        while(a<SIZE && b<SIZE && a>=0 && b>=0){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][b];
                cnt = 1;
            a--;
            b++;
        }
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-rdig[j]+value;
        rdig[j] = value;
    }
    else{
        int a = x-y;
        int b = 0;
        while(a<SIZE && b<SIZE){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = disk;
                disk = board[a][b];
                cnt = 1;
            a++;
            b++;
        }
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-rdig[j]+value;
        rdig[j] = value;
    }
}

int minimax(State s,int depth,bool ourturn){
    if(depth == 0 || s.bvalue == INFINITE || s.bvalue == -(INFINITE)){
        return s.bvalue;
    }
    else if(ourturn == true){
        for(int x=0;x<SIZE;x++){
        for(int y=0;y<SIZE;y++){
            if(s.board[x][y] == EMPTY){
                int bvalue = -1;
                State n(s);
                n.update(player,x,y);
                bvalue = max(bvalue,minimax(n,depth-1,false));
                return bvalue;
            }
        }
        }
    }
    else if(ourturn == false){
        for(int x=0;x<SIZE;x++){
        for(int y=0;y<SIZE;y++){
            if(s.board[x][y] == EMPTY){
                int bvalue = INFINITE;
                State n(s);
                n.update(3-player,x,y);
                bvalue = min(bvalue,minimax(n,depth-1,true));
                return bvalue;
            }
            }
        }
    }
    
    
}


void write_valid_spot(std::ofstream& fout,State original) {
    srand(time(NULL));
    int x, y;
    int bvalue = -1;
    if(board[7][7] == EMPTY){
        x = 7,y = 7;
        fout<<x<<" "<<y<<endl;
        fout.flush();
        return;
    }
    while(true){
    for(x=0;x<SIZE;x++){
    for(y=0;y<SIZE;y++){
        if(original.board[x][y] == EMPTY){
             State s(original);
             s.update(player,x,y);
             if(bvalue<minimax(s,3,false)){
                
                bvalue = s.bvalue;
                fout<<x<<" "<<y<<endl;
                fout.flush();
             }
        }
    }
    }
    }
}



int main(int, char** argv) {
    std::ifstream fin(argv[1]);
    std::ofstream fout(argv[2]);
    cout<<"Started"<<endl;
    read_board(fin);
    cout<<"Readed"<<endl;
    State original(board);
    cout<<"Stated"<<endl;
    write_valid_spot(fout,original);
    fin.close();
    fout.close();
    return 0;
}
