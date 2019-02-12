#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>
#include <windows.h>
#include <stdbool.h>

void setup (void);
void process(void);
void final (void);
void fuelStationSetup(void);
void fuelMove(void);
bool collision(void);
void respwan(void);
void carmove(void);
void obs(void);
void welcome(void);
void draw_border(void);
void setup_car(void);
void setup_back(void);
int main(void) ;
void fuelCollision(void);
void zombieCreate(void);
sprite_id setupZom(void);
void update_sprites(sprite_id sprites[], int num_sprites);
void update_sprite(sprite_id sprite);
bool ZomColl (void);
bool coll=false;
bool ZColl=false;
bool game_over = false;
bool update_screen=true;
bool FuelNeed=false;
int windist=1000;

sprite_id car;
char * car_image=   "()--()"
                    " |  | "
                    " |  | "
                    " |  | "
                    " |  | "
                    "()--()";
int carW=6;
int carH=6;

sprite_id tree;
char * tree_image=  "  V "
                    " VV "
                    " VVV"
                    "VVVV";


sprite_id tree1;
char * tree_image1= "   V   "
                    "  VV   "
                    " VVV   "
                    " VVVV  "
                    " VVVVV "
                    "VVVVVV "
                    "VVVVVVV";
sprite_id tree2;
sprite_id tree3;
sprite_id tree4;
char * tree_image2= "     V     "
                    "    VV     "
                    "   VVV     "
                    "   VVVV    "
                    "   VVVVV   "
                    "  VVVVVV   "
                    " VVVVVVVVV "
                    "VVVVVVVVVV "
                    "VVVVVVVVVVV";

sprite_id house;
char * house_image=     "  ______________"
                        " /             |"
                        "/______________|"
                        "|       __     |"
                        "|      |xx|    |"
                        "|______|xx|____|";

sprite_id house1;
char * house_image1=    "  ______________"
                        " / NNNNNNN     |"
                        "/__[VVVV]______|"
                        "|  ___  __     |"
                        "| |__| |xx|    |"
                        "|______|xx|____|";


sprite_id house2;
sprite_id barrier;
sprite_id barrier2;
char * barrier_image=   " ______________ "
                        "|XXXXXXXXXXXXX |"
                        "|XXXXXXXXXXXXX |"
                        "|______________|";

sprite_id barrier1;
char * barrier_image1=  " _________ "
                        "|XXXXXXXXX|"
                        "|_________|";

sprite_id fuelst;
char * fuel_image=      " ____________ "
                        "|xxxx|       |"
                        "|FUEL|       |"
                        "|xxxx|       |"
                        "|^_^_|       |"
                        "|^_^_|       |"
                        "|^_^_|       |"
                        "|^_^_|       |"
                        "|^_^_|       |"
                        "|^_^_|       |"
                        "|^_^_|_______|";

sprite_id finish;
char * finish_image="_________________________________________________________________________________________________"
                    "|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|"
                    "|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|"
                    "|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|_|"
                    "_________________________________________________________________________________________________";


sprite_id zombie;
char * zombie_image = "zzzzz"
					"   z "
					"  z  "
					" z   "
					"zzzzz";
int fuelw=14;
int fuelh=7;

double time;
timer_id timer;
double velocity=0;
double distance=0;
double fuel=100;
int vehicleCount=10;
#define MAX_ZOMBIES 22
sprite_id zombies[MAX_ZOMBIES];

bool ZomColl (void){
    int Zomleft[22];
    int Zomtop[22];
    int Zomright[22];
    int Zombottom[22];

    int car_top=round(sprite_y(car));
    int car_bottom=car_top+carH-1;
    int car_left=round(sprite_x(car));
    int car_right=car_left+carW-1;

    for ( int i = 0; i < 22; i++ ) {
        Zomleft[i]=round(sprite_x(zombies[i]));
        Zomtop[i]=round(sprite_y(zombies[i]));
        Zombottom[i]=Zomtop[i]+sprite_height(zombies[i]);
        Zomright[i]=Zomleft[i]+sprite_width(zombies[i]);
    }

    for(int i=0;i<22;i++){
        if (car_bottom<Zomtop[i]){
            ZColl=false;

        }
        else if(car_top>Zombottom[i]){
            ZColl=false;
        }
        else if (car_right<Zomleft[i]){
            ZColl=false;
        }
        else if(car_left>Zomright[i]){
            ZColl=false;
        }
        else ZColl=true;

        if(ZColl==true){
            break;
            velocity=0;
        }
    }
    return ZColl;
}

void update_sprites(sprite_id sprites[], int num_sprites){
    for ( int i = 0; i < num_sprites; i++ ) {
       sprite_id current_sprite = sprites[i];
      update_sprite(current_sprite);
    }
}
void update_sprite(sprite_id sprite){
    // Get screen location of sprite.
    int x = round(sprite_x(sprite));
    int y = round(sprite_y(sprite));

    // Get the displacement vector of the sprite.
    double dx = sprite_dx(sprite);
    double dy = sprite_dy(sprite);

    // Test to see if the sprite hit the left or right border.
    if ( (x <= 0) || (x >= screen_width() - sprite_width(sprite)) ) dx = -dx;

    // Test to see if the sprite hit the top or bottom border.
    if ( (y <= screen_height()/6) || (y >= screen_height() - sprite_height(sprite)) ) dy = -dy;

    // Test to see if the sprite needs to step back and change direction.
    if ( dx != sprite_dx(sprite) || dy != sprite_dy(sprite) ) {
        sprite_back(sprite);
        sprite_turn_to(sprite, dx, dy);
    }
}

void zombieCreate(void){
//setting up the zombies at a random location on the screen
    int now=get_current_time();
    srand(now);

    for (int i=0;i<MAX_ZOMBIES;i++){
        zombies[i]=setupZom();
    }

}
sprite_id setupZom(void){
    int x_range=screen_width()-5-2;
    int y_range=screen_height()-1-7-screen_height()/6+1;
    int zombie_x=1+rand()%x_range;
    int zombie_y=9+rand()%y_range;
    zombie=sprite_create(zombie_x,zombie_y,5,5,zombie_image);

    //Set the zombie in motion
    sprite_turn_to(zombie,0.2,0.0);
    int angle = rand() % 360;
    sprite_turn(zombie, angle);
    return zombie;

}

void success(void){
    clear_screen();
    int w = screen_width();
    int h = screen_height();

    vehicleCount=3;
    draw_formatted(w/2-10,h/2, "CONGRATULATIONS YOU WIN!");
    draw_formatted(w/2-10,h/2+1, "Press 'Q' to quit");
    draw_formatted(w/2-10,h/2+2, "Press 'R' to replay");
    draw_formatted(w/2-10,h/2+3, "Distance travelled :%lf",distance);
    draw_formatted(w/2-10,h/2+4, "Time Taken:%lf",time);
    draw_formatted(w/2-10,h/2+5, "Fuel Left:%lf",fuel);

    show_screen();
    timer_reset(timer);
    velocity=0;
    distance=0;
    fuel=100;
    time=0;

    while (1) {
        int key=get_char();

        if (key=='Q'){
            game_over=true;
            return;
        }

        if (key=='R'){
            clear_screen();
            setup_screen();
            setup();
            //show_screen();
            while ( ! game_over ) {
                process();

               if ( update_screen ) {
                   show_screen();
                }
                timer_pause(10);
                if (vehicleCount==0 || fuel<=0){
                    final();
                }
                if (distance>windist){
                    success();
                }
            }
        return;
        }

    }
    clear_screen();
}

void final (void){
    clear_screen();
    int w = screen_width();
    int h = screen_height();

    vehicleCount=3;
    draw_formatted(w/2-10,h/2, "Game Over!,press 'Q' to quit");
    draw_formatted(w/2-10,h/2+1, "Press 'Q' to quit");
    draw_formatted(w/2-10,h/2+2, "Press 'R' to replay");
    draw_formatted(w/2-10,h/2+3, "Distance travelled :%lf",distance);
    draw_formatted(w/2-10,h/2+4, "Time Taken:%lf",time);
    draw_formatted(w/2-10,h/2+5, "Fuel Left:%lf",fuel);

    show_screen();
    timer_reset(timer);
    velocity=0;
    distance=0;
    fuel=100;
    time=0;

    while (1) {
        int key=get_char();

        if (key=='Q'){
            game_over=true;
            return;
        }

        if (key=='R'){
            clear_screen();
            setup_screen();
            setup();
            //show_screen();
            while ( ! game_over ) {
                process();

               if ( update_screen ) {
                   show_screen();
                }
        		timer_pause(10);
                if (vehicleCount==0 || fuel<=0){
                    final();
                }
                if (distance>windist){
                    success();
                }
            }
        return;
        }

    }
clear_screen();
}

void fuelCollision(void){
    int car_top=round(sprite_y(car));
    int car_bottom=car_top+carH-1;
    int car_left=round(sprite_x(car));
    int car_right=car_left+carW-1;

    int fuelst_top=round(sprite_y(fuelst));
    int fuelst_bottom=fuelst_top+sprite_height(fuelst)-1;
    int fuelst_left=round(sprite_x(fuelst));
    int fuelst_right=fuelst_left+sprite_width(fuelst)-1;

    if (car_bottom<fuelst_bottom && car_top<fuelst_top && car_left>fuelst_left && car_right<fuelst_right){
        if (fuelst_top>car_top){
            fuel=100;
        }
    }
}

bool collision(void){

    int car_top=round(sprite_y(car));
    int car_bottom=car_top+carH-1;
    int car_left=round(sprite_x(car));
    int car_right=car_left+carW-1;
    sprite_id obs[11]={tree,tree1,tree2,tree3,tree4,barrier,barrier1,barrier2,house,house1,house2};
    int obsleft[11];
    int obstop[11];
    int obsright[11];
    int obsbottom[11];
    int obsH[11]={4,7,9,4,4,4,3,4,6,6,6};
    int obsW[11]={4,7,11,4,4,16,11,16,16,16,16};

    for(int i=0;i<11;i++){
        obsleft[i]=round(sprite_x(obs[i]));
        obstop[i]=round(sprite_y(obs[i]));
        obsbottom[i]=obstop[i]+obsH[i];
        obsright[i]=obsleft[i]+obsW[i];
    }

    for(int i=0;i<11;i++){
        if (car_bottom<obstop[i]){
            coll=false;
        }
        else if(car_top>obsbottom[i]){
            coll=false;
        }
        else if (car_right<obsleft[i]){
            coll=false;
        }
        else if(car_left>obsright[i]){
            coll=false;
        }
        else coll=true;
        if(coll==true){
            break;
            velocity=0;
        }
    }
    return coll;
}

void respwan(void){
    vehicleCount=vehicleCount-1;
    fuel=100;
    velocity=0;
    if(collision() || ZomColl()){
        int w=screen_width();
        int h=screen_height();
        sprite_move_to(car,w*0.5,h*0.84);
        while (collision() || ZomColl()){
            int c=(0.75*w-16+1-0.25*w);
            int spawnx=(rand()%c+0.25*w);
            sprite_move_to(car,spawnx,h*0.84);
            collision();
            ZomColl();
        }
    }
}
void carmove(void){

    int w = screen_width();
    int h = screen_height();
    int key=get_char();
    int carx=round(sprite_x(car));
    //Detecting if car is on the road or off the road
    bool roadcheck=true;

    if (screen_width()*0.25>carx){
        roadcheck=false;
    }

    if (screen_width()*0.75-6<carx){
        roadcheck=false;
    }
    //Mechanism relating to off-raod max speed.
    if (roadcheck==false){
        if (velocity>=0.3){
            velocity=0.3;
        }
    }
    //Mechanism to avoid speed exceed limit
    if (roadcheck==true){
        if (velocity>=1){
            velocity=1;
        }
    }

    if (roadcheck==true){
        if (velocity<0){
            velocity=0;
        }
    }

    if (roadcheck==true){
        if (velocity<=0){
            velocity=0;
        }
    }
    //Accelerating the car off the road
    if (key=='w'){
        if(velocity<0.3){
            if (roadcheck==false){
                velocity+=0.1;
            }
        }
    }

    //Accelerating the car on the road
    if (key=='w'){
        if(velocity<1){
            if(roadcheck==true){
                velocity+=0.1;
            }
        }
    }


    if ( key == 'a' ) {
        if ( sprite_x( car ) > 1 && velocity!=0) {
            sprite_move( car, -1, 0 );
        }
    }
    else if ( key == 'd' ) {
        if ( sprite_x( car ) < screen_width() - 6-1 &&  velocity!=0 ) {
            sprite_move( car, +1, 0 );
        }
    }

    //Decelerating the car
    if (key=='s'){
        if(velocity>0){
            if(roadcheck==true){
                velocity-=0.1;
            }
            else if(roadcheck==false){
                velocity-=0.1;
            }
        }
    }

    if ( key == 'q' ) {
        game_over=true;
    }
// Code to set up fuel station randomly

    int selector=rand()%(2+1-1)+1;
    int fx;
    if (selector==1){
         fx=0.75*w;
    }
    else{
         fx=0.25*w-13;
    }
    int FuelCoef=rand()%(4+1-20)+1;
    int fy=(screen_height()/6)-5;
    if( round(sprite_y(fuelst))==h*FuelCoef){
        fuelst=sprite_create(fx,fy,14,11,fuel_image);
        sprite_draw(fuelst);
    }
    sprite_turn_to(fuelst,0,velocity);
    //sprite_step(fuelst);


    //int q=0.25*w-16-14+1-0;
    int q=0.25*w-16-14+1-0;
    int housex=(rand()%q);
    int housey=(screen_height()/6)+1;
    if( round(sprite_y(house))==screen_height()){
        house=sprite_create(housex,housey,16,6,house_image);
        sprite_draw(house);
    }
    sprite_turn_to(house,0,velocity);
    sprite_step(house);
// Code to set up scenary randomly
    int r=(w-16+1-(w*0.75+14));
    int housex1=round(rand()%r+(w*0.75)+14);
    int housey1=(h/6)+1;
    if( round(sprite_y(house1))==screen_height()){
        house1=sprite_create(housex1,housey1,16,6,house_image);
        sprite_draw(house1);
    }
// Code to set up scenary randomly
    int s=(w-4+1-(w*0.75+14));
    int treex=round(rand()%s+(w*0.75)+14);
    int treey=(h/6)+1;
    if ( round(sprite_y(tree))==screen_height()){
        tree=sprite_create(treex,treey,4,4,tree_image);
        sprite_draw(tree);
    }
// Code to set up scenary randomly
    int a=(w-7+1-(w*0.75+14));
    int treex1=round(rand()%a+(w*0.75)+14);
    int treey1=(h/6)+1;
    if (round(sprite_y(tree1))==screen_height()){
        tree1=sprite_create(treex1,treey1,7,7,tree_image1);
        sprite_draw(tree1);
    }
// Code to set up scenary randomly
    int p=0.25*w-16-14+1-0;
    int housex2=(rand()%p);
    int housey2=(screen_height()/6)+1;
    if( round(sprite_y(house2))==screen_height()){
        house2=sprite_create(housex2,housey2,16,6,house_image1);
        sprite_draw(house2);
    }

// Code to set up scenary randomly
    int k=(w-11+1-(w*0.75+14));
    int treex2=(rand()%k+(w*0.75)+14);
    int treey2=(screen_height()/6)+1;
    if( round(sprite_y(tree2))==screen_height()){
        tree2=sprite_create(treex2,treey2,11,9,tree_image2);
        sprite_draw(tree2);
    }
// Code to set up scenary randomly
    int l=0.25*w-4-14+1-0;
    int treex3=(rand()%l);
    int treey3=(screen_height()/6)+1;
    if( round(sprite_y(tree3))==screen_height()){
        tree3=sprite_create(treex3,treey3,4,4,tree_image);
        sprite_draw(tree3);
    }
// Code to set up scenary randomly
    int cc=(w-4+1-(w*0.75+14));
    int treex4=(rand()%cc+(w*0.75)+14);
    int treey4=(screen_height()/6)+1;
    if( round(sprite_y(tree4))==screen_height()){
        tree4=sprite_create(treex4,treey4,4,4,tree_image);
        sprite_draw(tree4);
    }


    velocity=sprite_dy(fuelst);
    sprite_turn_to(fuelst,0,velocity);
    sprite_step(fuelst);

    velocity=sprite_dy(house);
    sprite_turn_to(house,0,velocity);
    velocity=sprite_dy(house);

    sprite_turn_to(tree,0,velocity);
    sprite_step(tree);
    velocity=sprite_dy(tree);

    sprite_turn_to(tree1,0,velocity);
    sprite_step(tree1);
    velocity=sprite_dy(tree1);

    sprite_turn_to(tree2,0,velocity);
    sprite_step(tree2);
    velocity=sprite_dy(tree2);

    sprite_turn_to(tree3,0,velocity);
    sprite_step(tree3);
    velocity=sprite_dy(tree3);

    sprite_turn_to(tree4,0,velocity);
    sprite_step(tree4);
    velocity=sprite_dy(tree4);

    sprite_turn_to(house1,0,velocity);
    sprite_step(house1);
    velocity=sprite_dy(house1);

    sprite_turn_to(house2,0,velocity);
    sprite_step(house2);
    velocity=sprite_dy(house2);

    clear_screen();
    sprite_draw(car);

}
void obs(void){
    int w = screen_width();
    int h = screen_height();
    int c=(0.75*w-16+1-0.25*w);
    int barrierx=(rand()%c+0.25*w);
    int barriery=(h/6)+1;
    if( round(sprite_y(barrier))==h*2.5){
        barrier=sprite_create(barrierx,barriery,16,4,barrier_image);
        sprite_draw(barrier);
    }

    sprite_turn_to(barrier,0,velocity);
    sprite_step(barrier);
    velocity=sprite_dy(barrier);

    int barrierx1=(rand()%c+0.25*w);
    int barriery1=(h/6)+1;
    if( round(sprite_y(barrier1))==h){
        barrier1=sprite_create(barrierx1,barriery1,11,3,barrier_image1);
        sprite_draw(barrier1);
    }

    sprite_turn_to(barrier1,0,velocity);
    sprite_step(barrier1);
    velocity=sprite_dy(barrier1);

    int barrierx2=(rand()%c+0.25*w);
    int barriery2=(h/6)+1;
    if( round(sprite_y(barrier2))==h){
        barrier2=sprite_create(barrierx2,barriery2,16,4,barrier_image);
        sprite_draw(barrier2);
    }

    sprite_turn_to(barrier2,0,velocity);
    sprite_step(barrier2);
    velocity=sprite_dy(barrier2);

}
void welcome(void){
//Creating Console screen
    setup_screen();
    int w=screen_width();
    int h=screen_height();
    draw_string( w/2-10, h/2, "Welcome to Race to Zombie Mountain" );
    draw_string( w/2-10, h/2+1, "Created by P.Sachin.D.Fernando n9879129" );
    draw_string( w/2-10, h/2+2, "Use these controls to play the game, " );
    draw_string( w/2-10, h/2+3, "a-To move left,d-to move right,w- to accelerate, s-decelerate" );
    draw_string( w/2-10, h/2+4, "To pause the game press Ctrl+S" );
    draw_string( w/2-10, h/2+5, "To quit game press q " );
    draw_string( w/2-10, h/2+6, "Press any key to continue" );
    show_screen();
    wait_char();
    clear_screen();
}

void draw_border( void ) {
// Creating main border

    int left=0;
    int top=0;
    int right=screen_width()-1;
    int bottom=screen_height()-1;
    draw_line( left , top, right, top, '%' );
    draw_line( left, bottom, right, bottom, '%');
    draw_line( right, top, right, bottom, '%' );
    draw_line( left, top, left, bottom, '%' );

//Creating dashboard which is 1/4 of the screen
    int dleft=0;
    int dtop=0;
    int dright=screen_width()-1;
    int dbottom=(screen_height()/6)-1;
    draw_line( dleft , dtop, dright, dtop, '%' );
    draw_line( dleft, dbottom, dright, dbottom, '%');
    draw_line( dright, dtop, dright, dbottom, '%' );
    draw_line( dleft, dtop, dleft, dbottom, '%' );
//Setting up the the in game borders
    int rleft=screen_width()*0.25;
    int rtop=dbottom+1;
    int rright=screen_width()*0.75;
    int rbottom=screen_height()-2;
    draw_line( rleft, rtop, rleft, rbottom, '|' );
    draw_line( rright, rtop, rright, rbottom, '|' );

    if (vehicleCount==0){
        game_over=true;;
    }

    draw_formatted(dleft + 1, dtop + 1, "Time:%lf", time);
    draw_formatted(dleft + 1, dtop + 2, "Condition of the car(Hp Left):%i",vehicleCount); //Good,Bad,Terrible
    draw_formatted(dleft + 1, dtop + 3, "Speed of the car:%lf",velocity*10);
    draw_formatted(dleft + 1, dtop + 4, "Amount of fuel left:%lf",fuel);
    draw_formatted(dleft + 1, dtop + 5, "Total distance travelled:%lf",distance);
}

void setup_car(void){
    setup_screen();
    int w = screen_width();
	int h = screen_height();
    car=sprite_create(w/2,h*0.84,6,6,car_image);
}

void setup_back(void){
    int w = screen_width();
    int h = screen_height();
    tree=sprite_create(w/9,h*0.9,4,4,tree_image);
    tree1=sprite_create(w/7,h*0.29,7,7,tree_image1);
    tree2=sprite_create(w*0.8,(h/4)+15,11,9,tree_image2);
    tree3=sprite_create(w/7,h/2+5,4,4,tree_image);
    tree4=sprite_create(w*14/15,h*0.2,4,4,tree_image);
    barrier=sprite_create(w*0.5,h*0.4,16,4,barrier_image);
    barrier1=sprite_create(w*0.4,h*0.2,11,3,barrier_image1);
    barrier2=sprite_create(w*0.6,h*0.7,16,4,barrier_image);
    house=sprite_create(w*0.8,h*0.35,16,6,house_image);
    house1=sprite_create(w*0.8,h*0.7,16,6,house_image);
    house2=sprite_create(w*0.15,h*0.82,16,6,house_image1);
    fuelst=sprite_create(w*0.25-14,h/6-40,14,11,fuel_image);
    finish=sprite_create(w*0.25,h/6-5,97,5,finish_image);
    fuelCollision();
    zombieCreate();

}

void process(void){
    fuelCollision();
    update_sprites(zombies,MAX_ZOMBIES);
    if(timer_expired(timer)){
        time+=0.1;
        distance+=time*velocity;
        fuel-=velocity*time*0.5;
        if (fuel<=0){
            fuel=0;
            velocity=0;
        }
    }

    carmove();

    if (round(distance)>round(windist-30)){
        sprite_draw(finish);
        sprite_turn_to(finish,0,velocity);
        sprite_step(finish);
    }

    if (round(fuel)<=95){
        sprite_draw(fuelst);
        sprite_turn_to(fuelst,0,velocity);
        sprite_step(fuelst);

    }


    sprite_step(finish);
    sprite_step(tree);
    sprite_draw(tree);
    collision();
    sprite_step(tree1);
    sprite_step(tree2);
    sprite_step(tree3);
    sprite_step(tree4);
    sprite_step(house);
    sprite_step(house1);
    sprite_step(house2);
    sprite_draw(tree);
    sprite_draw(tree1);
    sprite_draw(tree2);
    sprite_draw(tree3);
    sprite_draw(tree4);
    sprite_draw(barrier);
    sprite_draw(barrier1);
    sprite_draw(barrier2);
    sprite_draw(house);
    sprite_draw(house1);
    sprite_draw(house2);

    obs();
    sprite_step(barrier);
    sprite_step(barrier1);
    sprite_step(barrier2);

    for (int i = 0; i < MAX_ZOMBIES; i++) {
        sprite_step(zombies[i]);
    }
    for (int i = 0; i < MAX_ZOMBIES; i++) {
        sprite_draw(zombies[i]);
    }

    draw_border();
    sprite_draw(car);
    show_screen();

    if ( collision() || ZomColl()){
        respwan();
    }

}


void setup (void){
    welcome();
    timer=create_timer(100);
    draw_border();
    setup_car();
    setup_back();
}
int main(void) {
    setup_screen();

    setup();
    show_screen();
    while ( ! game_over ) {
        process();

       if ( update_screen ) {
           show_screen();
       }
		timer_pause(10);
        if (vehicleCount==0 || fuel<=0 ){
            final();
        }
        if (distance>windist){
            success();
        }
    }

return 0;
}
