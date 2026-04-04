#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#define N 40
#define DT 0.1
#define C 1.0
#define DAMP 0.99
#define FRAMES 200

double h_curr[N][N];
double h_prev[N][N];
double h_next[N][N];

double get_time() {
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return t.tv_sec + t.tv_nsec * 1e-9;
}

void init_surface() {

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++) {
            h_curr[i][j]=0;
            h_prev[i][j]=0;
            h_next[i][j]=0;
        }
}

void apply_impulse(int cx,int cy) {

    int radius=2;
    double strength=2.0;

    for(int i=0;i<N;i++)
        for(int j=0;j<N;j++) {

            int dx=i-cx;
            int dy=j-cy;

            if(dx*dx+dy*dy<=radius*radius) {
                h_curr[i][j]+=strength;
                h_prev[i][j]=h_curr[i][j];
            }
        }
}

void update_physics() {

    for(int i=1;i<N-1;i++)
        for(int j=1;j<N-1;j++) {

            double u=h_curr[i][j];

            double lap =
                h_curr[i-1][j] +
                h_curr[i+1][j] +
                h_curr[i][j-1] +
                h_curr[i][j+1] -
                4*u;

            h_next[i][j]=(2*u-h_prev[i][j]+C*C*DT*DT*lap)*DAMP;
        }

    for(int i=1;i<N-1;i++)
        for(int j=1;j<N-1;j++) {

            h_prev[i][j]=h_curr[i][j];
            h_curr[i][j]=h_next[i][j];
        }
}

void render(double fps) {

    printf("\033[H\033[J");

    for(int i=0;i<N;i++) {

        for(int j=0;j<N;j++) {

            double val=h_curr[i][j];

            if(val>1.0) printf("@");
            else if(val>0.5) printf("O");
            else if(val>0.1) printf("o");
            else if(val>-0.1) printf(".");
            else if(val>-0.5) printf("-");
            else printf("~");
        }

        printf("\n");
    }

    /* move cursor to right side and print FPS */
    printf("\033[2;60H");
    printf("FPS: %.2f",fps);
}

int main() {

    init_surface();
    apply_impulse(N/2,N/2);

    double start=get_time();

    for(int f=0;f<FRAMES;f++) {

        double frame_start=get_time();

        update_physics();

        double frame_end=get_time();
        double fps=1.0/(frame_end-frame_start);

        render(fps);

        usleep(50000);
    }

    return 0;
}