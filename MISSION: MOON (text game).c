#include <stdio.h>
#include<math.h>
//payload name and mass(kg)
//the staellite contains basic structur of landing legs,fuel tank
char *payload_name[]= {"Camera","Solar panels","Battery","Antenna dish","X-ray spectrometer","Magnetometer"}; 
double payload_mass[]={10.0,30.0,70.0,2.0,12.0,5.0}; // mass of payload,can be edited.
int payload_count=sizeof(payload_mass)/ sizeof(payload_mass[0]);

// propellet for satelitte
char *prop_name[]={"Kerosebe/LOX","HYDROGEN/LOX","Hypergolic UDMH","Xenon ion driver"};
double prop_isp[]={300.0,450.0,320.0,3000.0};
int prop_count= sizeof(prop_isp)/sizeof(prop_isp[0]);

// material and heatshield for satellite
char *material_name[]={"Carbon phenolic","Reinforced carbon-carbon","Silica-tiles","Aluminium skin"};
double material_mass[]={13000.0,12000.0,8500.0,3000.0};
int material_count=sizeof(material_mass)/sizeof(material_mass[0]);

void stage2(double satellite_mass);
void stage3(double satellite_mass,double journey_isp);
void stage4(double satellite_mass);
void stage5(double satellite_mass, double journry_isp);
void stage6(double satellite_mass);
void stage7(double satellite_mass,double journey_isp);
int main(){
    printf("Welcome to MISSION: MOON\nThis is simple choice-answer game where your typed answer determine the mission.\n");
    int i,ch;
    double payload_total=0.0;
    double satellite_mass=0.0;
    double journey_isp=0.0;
    double material_total=0.0;
    printf("===Stage 1: Satellite & payload===\n");

    //pick payloads
    printf("Choose payloads(enter 0 when done);\n");
    for(i=0;i<payload_count;i++){printf(" %d) %-10s %.1f kg\n",i+1,payload_name[i],payload_mass[i]);}
    do{
    printf("Payload choice:");
    if(scanf("%d",&ch)!=1) break;
    if (ch>=1 && ch<= payload_count)
    {
    payload_total+=payload_mass[ch-1];
    printf(" Added %s (%.1f kg). Payload total now %.1f kg\n",payload_name[ch-1],payload_mass[ch-1],payload_total);}
    } while (ch!=0);

    //pick material
    printf("Choose material(enter 0 when done);\n");
    for(i=0;i<material_count;i++){printf(" %d) %-10s %.1f kg\n",i+1,material_name[i],material_mass[i]);}
    do{
    printf("Material choice:");
    if(scanf("%d",&ch)!=1) break;
    if (ch>=1 && ch<= material_count)
    {
    material_total+=material_mass[ch-1];
    printf(" Added %s (%.1f kg). material total now %.1f kg\n",material_name[ch-1],material_mass[ch-1],material_total);}
    } while (ch!=0);

    //choose propellent
    printf("\nChoose propellent\n");
    for(i=0;i<prop_count;i++){printf("  %d) %-16s Isp= %.0f s\n",i+1,prop_name[i],prop_isp[i]);}
    printf("Your choice: ");
    scanf("%d",&ch);
    if(ch>=1 && ch<= prop_count){journey_isp=prop_isp[ch-1];}

    satellite_mass=payload_total + material_total;  //+700 for structure other unnamed instruments. 
    
    printf("\n--Summary--\n");
    printf("Payload mass: %.1f kg\n",payload_total);
    printf("Material mass: %.1f kg\n",material_total);
    printf("Satellite mass: %.1f kg",satellite_mass); //stage 1 end
    
    stage2(satellite_mass);
    stage3(satellite_mass,journey_isp);
    stage4(satellite_mass);
    stage5(satellite_mass,journey_isp);
    stage6(satellite_mass);
    stage7(satellite_mass,journey_isp);
    return 0;
} 

#define GO  9.81
#define ORBIT_DV 9400.0 //delta v needed to reach low earth orbit,m/s 

// --vehicle name: name+number of stages
char *lv_name[]={"PSLV","GSLV MK 2","LVM3"};
int lv_stages[]={4,3,2};
int lv_count=3;

//stage data per vehicle: fuel(kg),structure(kg),isp(s)---
double lv_fuel[3][4]={
    {138000,41000,7000,2000},   //PSLV
    {160000,40000,12000,0},  //"GSLV MK 2
    {220000,130000,0,0} //LVM3
};
double lv_dry[3][4]={
    {30000,5000,1200,500},
    {3500,6000,2000,0},
    {40000,20000,0,0}
};
double lv_isp[3][4]={
    {269,280,294,308},
    {269,280,440,0},
    {269,443,0,0}
};

//module seperation altitudes(km)
double sep_alt[]={60.0,130.0,180.0,200.0};
int v=0;
void stage2 (double satellite_mass){
    int i,ch,nstages;
    double mass_above,m0,mf,dv,dv_total=0.0;
    double dv_stage[4],m0_stage[4],mf_stage[4];

    printf("==STAGE 2: Rocket build & Launch==");
    printf("Satellite mass from stage 1: %.1f kg\n",satellite_mass);
    printf("\n Choose launch vehucle:\n");
    for(i=0;i<lv_count;i++){printf(" %d) %s (%d stages)\n",i+1,lv_name[i],lv_stages[i]);}
    printf("Your Choice: ");
    scanf("%d",&ch);
    if(ch< 1 || ch> lv_count){
        printf("Invalid choice\n");
        return;
    }
    v= ch-1;
    nstages= lv_stages[v];
    printf("\n---LIFT OFF: %s ---\n",lv_name[v]);
    /* work from top stage down,tracking thw mass sitting above the stage
    being burned(payload + all higer stages */
    mass_above=satellite_mass;
    for (i=nstages-1; i>=0; i--){
        m0=mass_above+ lv_fuel[v][i] + lv_dry[v][i];  //before burn
        mf= mass_above+ lv_dry[v][i];  //after burn
        dv=lv_isp[v][i] *GO * log(m0/mf);
        dv_stage[i]=dv;
        m0_stage[i]=m0;
        mf_stage[i]=mf;
        dv_total+=dv;
        mass_above=m0;
    }
    //printing in launch order to enjoy stage
    for(i=0;i<nstages;i++){
        printf("Stages %d: %.0f kg -> %.0f kg, +%.0f m/s [sep %.0f km]\n",i+1,m0_stage[i],mf_stage[i],dv_stage[i],sep_alt[i]);
        printf("    >>Stage %d seperated,mass now %.0f kg\n",mf_stage[i]);
    }
    printf("\nTotal delta-v available: %.0f m/s\n",dv_total);
    printf("Needed for orbit         :%.0f m/s\n",ORBIT_DV);

    if (dv_total>= ORBIT_DV){printf("\nLIFT OFF NORMAL-- %s reaches orbit!\n",lv_name[v]);
        printf("Sarellite detached and released into orbit.\n");}
    else{printf("\nTOO HEAVY -- %s can't lift this satellit. Try bigger vehicle",lv_name[v]);}   
} //stage2 end

#define EARTH_MOON 384400000.0 //earth-moon distance in meters
#define CRUISE_FAC 0.6 //game factor:cruise speed=0.6*exhaust velocity 

void stage3(double satellite_mass,double journey_isp){
    int i,ch;
    double ve,speed,time,hours,days;

    printf("\n=== STAGE 3: Journey to Moon==\n");
    printf("Satellite mass: %.1f kg\n",satellite_mass);
    printf("Propellent isp: %.0f s\n",journey_isp);

    ve= journey_isp*GO; //exhaust velocity m/s
    speed=CRUISE_FAC*ve;  // cruise speed m/s
    time=EARTH_MOON/speed;
    hours=time/3600.0;
    days=hours/24.0;

    printf("\n---Journey---\n");
    printf("Exhaust velocity   : %.0f m/s\n",ve);
    printf("Cruie speed        : %.0f m/s (%.0f km/h)\n",speed,speed*3.6);
    printf("Earth-Moon Distance: %.0f km\n",EARTH_MOON/1000.0);
    printf("Travel time        : %.1f hours (%.2f days)\n",hours,days);

    if (days<=2.0){printf("A fast crossing! The crew is happy.\n");}
    else{printf("A long,slow cruise. Bring extra snacks\n");}  
} //stage 3 complete

#define MOON_G 1.62
#define START_ALT 2000.0
#define START_VEL 100.
#define THRUST_A 5.0 // breaking acceleration m/s
#define DT 0.01  //time stop
#define FUEL_KG 300.0   //kg/s while braking
#define BURN_RATE 2.0

void stage4(double satellite_mass){
    double ignite_alt,alt=START_ALT, v=START_VEL,fuel=FUEL_KG;
    int burning=0;

    printf("\n===STAGE 4: MOON LANDING===\n");
    printf("Lander mass: %.1f kg\n",satellite_mass);
    printf("You are at %.0f m,falling at %.0f m/s.\n",START_ALT,START_VEL);
    printf("Moon gravity is %.2f m/s^2.\n",MOON_G);
    printf("Thrust force a = %.0f",satellite_mass*THRUST_A);
    printf("\nAt what altitude(m)should the braking engine fire?\n");
    printf("Formula to calculate--> ((velocity^2)+ 2*g(moon)*H)/2*thrust force\n");
    if(scanf("%lf",&ignite_alt)!=1) return;
    
    //stumalting decent
    while (alt>0.0 && fuel>0.0)
    {
        double a=MOON_G;
        if(alt<=ignite_alt){a-=THRUST_A;
            fuel-= BURN_RATE*DT;
            burning=1;
        }
        v+=(a*DT);
        alt-=(v*DT);
    }
    if(alt>0.0){   //fuel ran out mid air
        v=sqrt(v*v+ 2.0* MOON_G*alt);
        printf("Fuel ran out above the surface\n");
    }
    printf("\n---TOUCHDOWN---\n");
    printf("Descent speed : %.1f m/s\n",v);
    printf("Fuel left     : %.1f kg\n",fuel);
    
    if(v<=3.0 && v> -1.0){printf("\nSOFT LANDING! The staellite is safely on the moon.\n");}
    else{printf("\nCRASH! You hit %.1f m/s. Try igniting at a different altitude\n",v);}
    return;
}  //stage 4 complete

#define MOON_GM 4.9048e12
#define MOON_R  1737400.0
#define ASCENT_DV 2700.0
#define ASCENT_FUEL 900.0
void stage5(double satellite_mass, double journry_isp){
    double ascent_mass,fuel_needed, v_esc,speed,time,hours,days;
    printf("\n=== SATEG 5: Retrun to earth===\n");
    //1. lift off from moon
    ascent_mass=satellite_mass; //ascent moduel
    v_esc= sqrt(2.0*MOON_GM/MOON_R);    // lunar escape speed
    fuel_needed= ascent_mass* (exp(ASCENT_DV/(journry_isp*GO))-1.0);

    printf("Lunar escape speed : %.0f m/s\n",v_esc);
    printf("Ascent-delta       : %.0f m/s\n",ASCENT_DV);
    printf("Ascent fuel neede  : %.0f kg (have %.0f kg)\n",fuel_needed,ASCENT_FUEL);
    if(fuel_needed>ASCENT_FUEL){printf("NOT ENOUGH FUEL!\n");
        return;}
    printf("LIFT OFF from MOON! Heading back home\n");   
    
    //cruise back
    speed=CRUISE_FAC*journry_isp *GO;
    time=EARTH_MOON/speed;
    hours=time/3600.0;
    days=hours/24.0;
    printf("\nReturn cruise speed : %.0f m/s\n",speed);
    printf("Return travel time    : %.1f hours (%.2f days)\n",hours,days);
} // stage 5 complete


#define ENTRY_SPEED 11000.0 //
void stage6(double satlliet_mass){
    int i,ch;
    double angle;
    printf("\n===Stage 6: EARTH RE-ENTRY ===\n");
    printf("Satellite mass : %.1f kg\n",satlliet_mass);
    printf("Entry speed : %.0f m/s\n",ENTRY_SPEED);

    if(ENTRY_SPEED> *material_mass){
        printf("\nThe shield fails -- the satelliet burns up on the re-entry.\n");
        return;}
    printf("Shield holds. You survived the heats.\n");    
    printf("\nChoose teh entry angel in degrees(try 3-12): ");
    if(scanf("%lf",&angle)!=1) return;
    if(angle<4.0){printf("Too shallow--you skip off the atmosphere and lost in space\n");}
    else if (angle>10.0){printf("Too steep-- g-force and heat crush the satelliet.\n");}
    else{printf("\nSPLAHDOWN! The satelliet hits the Bay of Bengal at ~15N,87E\nCrew home safe. MISSION COMPLETE\n");}
}//Stage6 complete

/*===mission log: each stage writes its results here===*/
int g_stage=1;
int g_vejicle_used=0;
double g_dv_total=0.0;
double g_out_hours=0.0;
double g_hours_return=0.0;
int g_materials_used=0;

void stage7(double satellite_mass,double journey_isp){
    int score;
    g_vejicle_used=v+1;
    printf("\nYour payload has arrived and its online.\nHere is the record of your mission.\n");
    printf("Staellite mass : %.1f kg\nPropellent isp : %.0f s\n",satellite_mass,journey_isp);
    if(g_vejicle_used>0){printf("Launch vehicle : %s (%.0f m/s delta-v)\n",lv_name[g_vejicle_used-1],g_dv_total);}
    printf("Outbound journey : %.1f hours\n",g_out_hours);
    printf("Moon landing : %s\n",(g_stage>=4)? "SOFT LANDING":"not completed");

    if(g_hours_return>0){printf("Return journey : %.1f hours",g_hours_return);}
    if(g_materials_used>0){printf("Shield used : %s\n",material_name[g_materials_used-1]);}
    printf("Stage completed : %d of 6\n",g_stage);

    /*score:15 per stage,bonus for coming all the way home*/
    score=g_stage*15;
    if(g_stage==6){score+=10;}
    printf("\n MISSION SCORE  : %d / 100\n",score);

    if(g_stage==6){printf("Rank : MISSION COMMANDER -a flawless round trip!\n");}
    else if (g_stage>=4){printf("Rank : LUNAR EXPLORER - you reached the moon\n");}
    else if (g_stage>=2){printf("Rank : ORBITAL CADET - you reached space\n");}
    else{printf("Rank : Grounf crew - the mission ended early\n");}

    printf("\n=================================================\n");
    printf("    Thank you for playing MISSION: MOON\n");
    printf("=================================================\n");
}//stage 7 complete
