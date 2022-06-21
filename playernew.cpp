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
const int NEGINF = -1000000;


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
        cout<<"Completed"<<endl;
        cout<<bvalue<<endl;
        
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
   if(head == tail &&  head != 0 && tail != 0){return 0;}
   if(cnt == 5){return (player == disk)? INFINITE:-100000;}
   if(head==tail && head==0 && tail == 0 && cnt>=1){
     if(cnt == 4){return (player == disk)? 256:-256;}
     if(cnt == 3){return (player == disk)? 32:-64;}
     if(cnt == 2){return (player == disk)? 4:-4;}
     //if(cnt == 1){return(player == disk)? 1:-1;}
   }
   else if(cnt>=3){
     if(cnt == 4){return (player == disk)? 32:-64;}
     else if(cnt == 3){return (player == disk)? 8:-8;}
     else if(cnt == 2){return (player == disk)? 2:-2;}
   }
   return 0;    
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
                head = board[i][j-1];
                disk = board[i][j];
                cnt = 1;
            }
        }
        tail = -1;
        hor[i] += seqvalue(cnt,head,tail,disk);
        //cout<<hor[i]<<"h"<<i<<endl;
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
                head = board[i-1][j];
                disk = board[i][j];
                cnt = 1;
            }
        }
        tail = -1;
        ver[j] += seqvalue(cnt,head,tail,disk);
        //cout<<ver[j]<<"v"<<j<<endl;
        bvalue += ver[j];
    }
    for(int a=1;a<SIZE-1;a++){
        int x=0;
        int y = a;
        rdig[14+a] = 0;
        head = -1,cnt = 1;
        disk = board[x][a];
        x++;
        y++;

        while(x<SIZE && y<SIZE && x>=0 && y>=0){
             if(board[x][y] == disk){
                cnt++;
            }
            else if(board[x][y] != disk){
                tail = board[x][y];
                rdig[14+a] += seqvalue(cnt,head,tail,disk);
                head = board[x-1][y-1];
                disk = board[x][y];
                cnt = 1;
        }
        x++;
        y++;
        }
        tail = -1;
        rdig[14+a] += seqvalue(cnt,head,tail,disk);
        cout<<rdig[14+a]<<"r"<<14+a<<endl;
        bvalue+=rdig[14+a];
    }
    for(int b=0;b<SIZE;b++){
        int y=0;
        int x = b;
        rdig[b] = 0;
        head = -1,cnt = 1;
        disk = board[b][y];
        x++;
        y++;

        while(x<SIZE && y<SIZE && x>=0 && y>=0){
            
             if(board[x][y] == disk){
                cnt++;
            }
            else if(board[x][y] != disk){
                tail = board[x][y];
                rdig[b] += seqvalue(cnt,head,tail,disk);
                head = board[x-1][y-1];
                disk = board[x][y];
                cnt = 1;
            }
            x++;
            y++;
            
        
        }
        tail = -1;
        rdig[b] += seqvalue(cnt,head,tail,disk);
        cout<<rdig[b]<<"r"<<b<<endl;
        bvalue+=rdig[b];
        }
    
    for(int a=0;a<SIZE;a++){
        int x=0;
        int y=a;
        head = -1,cnt = 1;
        ldig[a] = 0;
        disk = board[x][a];
        x++;
        y--;

        while(x<SIZE && y<SIZE && x>=0 && y>=0){
             if(board[x][y] == disk){
                cnt++;
            }
            else if(board[x][y] != disk){
                tail = board[x][y];
                ldig[a] += seqvalue(cnt,head,tail,disk);
                head = board[x-1][y+1];
                disk = board[x][y];
                cnt = 1;
            }
            x++;
            y--;
        
        }
        tail = -1;
        ldig[a] += seqvalue(cnt,head,tail,disk);
        cout<<ldig[a]<<"l"<<a<<endl;
        bvalue+=ldig[a];
    }
    for(int b=1;b<SIZE;b++){
        int y=14;
        int x=b;
        head = -1,cnt = 1;
        ldig[14+b] = 0;
        disk = board[x][y];
        x++;
        y--;

        while(x<SIZE && y<SIZE && x>=0 && y>=0){
            if(board[x][y] == disk){
                cnt++;
            }
            else if(board[x][y] != disk){
                tail = board[x][y];
                ldig[14+b] += seqvalue(cnt,head,tail,disk);
                head = board[x-1][y+1];
                disk = board[x][y];
                cnt = 1;
            }
            x++;
            y--;
        
        }
        tail = -1;
        ldig[14+b] += seqvalue(cnt,head,tail,disk);
        cout<<ldig[14+b]<<"l"<<14+b<<endl;
        bvalue+=ldig[14+b];
        
    }
}

void State:: changevalue(int x,int y){
    int disk,head,tail;
    int cnt = 0;
    
    head = -1,cnt = 1;
    disk = board[x][0];
    int value = 0;
    for(int k=1;k<SIZE;k++){
            
            if(board[x][k] == disk){
                cnt++;
            }
            else if(board[x][k] != disk){
                tail = board[x][k];
                value += seqvalue(cnt,head,tail,disk);
                head = board[x][k-1];
                disk = board[x][k];
                cnt = 1;
            }
        }
    tail = -1;
    value += seqvalue(cnt,head,tail,disk);
    bvalue = bvalue-hor[x]+value;
    hor[x] = value;

    value = 0;
    head = -1,cnt = 1;
    disk = board[0][y];
    for(int k=1;k<SIZE;k++){
            
            if(board[k][y] == disk){
                cnt++;
            }
            else if(board[k][y] != disk){
                tail = board[k][y];
                value += seqvalue(cnt,head,tail,disk);
                head = board[k-1][y];
                disk = board[k][y];
                cnt = 1;
            }
        }
    tail = -1;
    value += seqvalue(cnt,head,tail,disk);
    bvalue = bvalue-ver[y]+value;
    ver[y] = value;

    int i = x-y;
    
    if(i>=0){
        int a = 0;
        int value = 0;
        int b = x-y;
        head = -1,cnt = 1;
        disk = board[a][b];
        a++;
        b++;
        while(a<SIZE && b<SIZE && a>=0 && b>=0){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = board[a-1][b-1];
                disk = board[a][b];
                cnt = 1;
            }
            a++;
            b++;
        
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-rdig[x-y]+value;
        rdig[x-y] = value;
    }
    else{
        int a = y-x;
        int b = 0;
        int value = 0;
        head = -1,cnt = 1;
        disk = board[a][b];
        a++;
        b++;
        while(a<SIZE && b<SIZE && a>=0 && b>=0){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = board[a-1][b-1];
                disk = board[a][b];
                cnt = 1;
            }
            a++;
            b++;
        
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-rdig[14+y-x]+value;
        rdig[14+y-x] = value;
    }
    


    int j = x+y;
    if(j<SIZE){
        int a = 0;
        int b = x+y;
        int value = 0;
        head = -1,cnt = 1;
        disk = board[a][b];
        a++;
        b--;
        while(a<SIZE && b<SIZE && a>=0 && b>=0){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = board[a-1][b+1];
                disk = board[a][b];
                cnt = 1;
            }
            a++;
            b--;
        
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-ldig[x+y]+value;
        ldig[x+y] = value;
    }
    else{
        int a = x+y-14;
        int b = SIZE-1;
        int value = 0;
        head = -1,cnt = 1;
        disk = board[a][b];
        a++;
        b--;
        while(a<SIZE && b<SIZE && a>=0 && b>=0){
             if(board[a][b] == disk){
                cnt++;
            }
            else if(board[a][b] != disk){
                tail = board[a][b];
                value += seqvalue(cnt,head,tail,disk);
                head = board[a-1][b+1];
                disk = board[a][b];
                cnt = 1;
            }
            a++;
            b--;
        
        }
        tail = -1;
        value += seqvalue(cnt,head,tail,disk);
        bvalue = bvalue-ldig[x+y]+value;
        ldig[x+y] = value;
    }
}

int minimax(State s,int depth,bool ourturn,int i,int j){
    if(depth == 0 || abs(s.bvalue)>=970000 || abs(s.bvalue)<=1030000){
        return s.bvalue;
    }
    else if(ourturn == true){
        for(int x=i-4;x<i+4;x++){
        for(int y=j-4;y<j+4;y++){
            if(x<SIZE && x>=0 && y<SIZE && y<=0){
            if(s.board[x][y] == EMPTY){
                int bvalue = -1000000;
                State n(s);
                n.update(player,x,y);
                bvalue = max(bvalue,minimax(n,depth-1,false,x,y));
                return bvalue;
            }
            }
        }
        }
    }
    else if(ourturn == false){
        for(int x=i-4;x<i+4;x++){
        for(int y=j-4;y<j+4;y++){
            if(x<SIZE && x>=0 && y<SIZE && y>=0){
            if(s.board[x][y] == EMPTY){
                int bvalue = INFINITE;
                State n(s);
                n.update(3-player,x,y);
                bvalue = min(bvalue,minimax(n,depth-1,true,x,y));
                return bvalue;
            }
            }
            }
        }
    }
}


void write_valid_spot(std::ofstream& fout,State original) {
    srand(time(NULL));
    int x, y;
    int bvalue = -1000000;
    int hvalue = -1000000;
    if(board[7][7] == EMPTY){
        fout<<7<<" "<<7<<endl;
        fout.flush();
        return;
    }
    
        for(x=0;x<SIZE;x++){
        for(y=0;y<SIZE;y++){
        if(original.board[x][y] == EMPTY){
             State s(original);
             s.update(player,x,y);
             if(bvalue<minimax(s,6,false,x,y)){
                
                bvalue = s.bvalue;
                if(hvalue<=bvalue){
                hvalue = bvalue;
                cout<<hvalue<<endl;
                cout<<x<<" "<<y<<endl;
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
    //cout<<"Stated"<<endl;
    write_valid_spot(fout,original);
    fin.close();
    fout.close();
    return 0;
}
