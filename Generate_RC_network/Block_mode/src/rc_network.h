//########################################################################
//##			              EDA Group                               ####
//##                    Politecnico di Torino                         ####
//##		     Yukai Chen : yukai.chen@polito.it                    ####     
//## Description: Generate thermal resistor and capacitor network     #### 
//##              in block model.                                     ####
//## Input files: Floorplan fie (.flp)                                ####
//## Output files: Thermal RC networks.								  ####
//########################################################################


#include "systemc-ams.h"

#include "temperature.h"
#include "temperature_block.h"
#include "util.h"
#include "flp.h"
#include "string.h"


SCA_TDF_MODULE(rc_network)
{

SCA_CTOR(rc_network){


//using namespace std;


//Read the floorplan file indicates the layout of each component of the system
static char *flp_file = "../example.flp" ; //Change your floorplan file in here


// Floorplan
static flp_t *flp;
// Hotspot temperature model
static RC_model_t *model;
// Instantaneous temperature and power values
static double *temp, *power;
// Steady state temperature and power values
static double *overall_power, *steady_temp;


// Initialize floorplan to get adjacency matrix
flp = read_flp(flp_file,FALSE);

// Configure thermal model parameters of the default setting
thermal_config_t config = default_thermal_config();
//strcpy(config.init_file, init_file);
//strcpy(config.steady_file, steady_file);

//Allocate and initialize the RC model
model = alloc_RC_model(&config, flp);

//Generate R and C in the model
populate_R_model(model,flp);
populate_C_model(model,flp);


//Print all values about the MODEL if you want to check detailed of the model
//debug_print_model(model);


//Get the number of unites of the system 
int num_units = model->block->n_units;

//Uncomment this line if you want to see how many units in your design
//cout<<"The number of units is \t"<<num_units<<"\n";


//Get the number of total nodes of the system (equal to units' number in block mode, but different in the grid mode)
int num_nodes = model->block->n_nodes;

//Uncomment this line if you want to see how many nodes in your design
//cout<<"The total number of nodes is \t"<<num_nodes<<"\n";


//Allocate a matrix(num_nodes X num_nodes) to store the resistors between two adjacent nodes
double **r_value = dmatrix(num_nodes,num_nodes);

//Extract and store the values of resistors in the matrix derived from Hotspot
static int i,j;
for(i=0;i<num_nodes;i++){
	for(j=0;j<num_nodes;j++){
		double tmp1 = model->block->b[i][j]; // Extract the values from the matrix generate from Hotspot
		double tmp2 = 1/abs(tmp1);
			if(i==j){
				r_value[i][j]=0;	
			}else{
				if(tmp2<999999999){ // avoide the infinite super big resistors
					r_value[i][j] = tmp2;
				}else{
					r_value[i][j] = 0;
				}
			}
	}
}


//Uncomment these lines if you want to print the values of capacitors and resisots on the screen
//fprintf(stdout,"\n\n\n");
//fprintf(stdout, "Printing vector CAPACITANCE:\n");
//dump_dvector(model->block->a, model->block->n_nodes); //This function defined in Hotspot
//fprintf(stdout,"\n\n");

//fprintf(stdout, "Printing resistors between system and environment:\n");
//dump_dvector(model->block->g_amb, model->block->n_units+EXTRA);
//fprintf(stdout,"\n");

// Print number of units and nodes on the screen
cout<<"The number of units is \t"<<num_units<<"\n";
cout<<"The total number of nodes is \t"<<num_nodes<<"\n";


// Define all 12 extral nodes in the thermal model
char *extra_nodes[STR_SIZE];

//Four peripheral directional nodes in spreader layer
extra_nodes[0] = "SP_W";
extra_nodes[1] = "SP_E";
extra_nodes[2] = "SP_N";
extra_nodes[3] = "SP_S";
//Four mapped area in heat sink layer from spreader layer
extra_nodes[4] = "SINK_C_W";
extra_nodes[5] = "SINK_C_E";
extra_nodes[6] = "SINK_C_N";
extra_nodes[7] = "SINK_C_S";
//Four peripheral directional nodes in heat sink layer
extra_nodes[8] = "SINK_W";
extra_nodes[9] = "SINK_E";
extra_nodes[10] = "SINK_N";
extra_nodes[11] = "SINK_S";

//Define the character layers array to store the layer name
char *layer_name[10];

//Define the layer name
layer_name[0] = "SILICON";
layer_name[1] = "INTERFACE";
layer_name[2] = "HEATSPREAD";
layer_name[3] = "HEATSINK";


// Define the files to store the SystemC-AMS scripts for r&c network will be used in simulation
FILE *fp;  // The header file for declaring r and c network

fp  = fopen("../testbench1_rc.h","w");

//Print the title for R&C model file
fprintf(fp,"/******************************************************/\n");
fprintf(fp,"/***This file is used to simulated in systemc-ams. ****/\n");
fprintf(fp,"/***                  Yukai Chen                   ****/\n");
fprintf(fp,"/***                     EDA                       ****/\n");
fprintf(fp,"/******************************************************/\n");
fprintf(fp,"\n");fprintf(fp,"\n");fprintf(fp,"\n");

//Print the header files in the target script
fprintf(fp,"#include \"systemc-ams.h\"\n");
fprintf(fp,"\n\n\n");

//Print the MODULE name in SystemC-AMS
fprintf(fp,"SC_MODULE (thermal_network)\n"); // The name of the thermal R&C model 
fprintf(fp,"{\n"); // { for module

//---------------------------------------------------------------------------------------------
//Print inputs of module
fprintf(fp,"\t\t //Interface declaration: input\n");

for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t sca_tdf::sca_in<double> p_%s ; // Power of the %s \n",flp->units[i].name,flp->units[i].name);
}

fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------
//Print ouputs of module
fprintf(fp,"\t\t //Interface declaration: output\n");

for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t sca_tdf::sca_out<double> t_%s ; // Temperature of the %s\n",flp->units[i].name,flp->units[i].name);
}

fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------
//Print the current source of each unit in your design, they are inputs of the thermal simulator
fprintf(fp,"\t\t //Define current source of each unit\n");
for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t sca_eln::sca_tdf::sca_isource* itemp_%s;\n",flp->units[i].name);
}


fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------
//Print the voltage sink of each unit in your design, they are outputs of the thermal simulator
fprintf(fp,"\t\t //Define voltage sink of each unit\n");

for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t sca_eln::sca_tdf::sca_vsink* vtemp_%s;\n",flp->units[i].name);
}

fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------
//Print the definition of all resistors, the process of analyzing the matrix r_value[][]

//All lateral and vertical resistors in four layers, uncomment them if you do not want to see the matrix in the script
//fprintf(fp,"// Such matrix include all lateral and vertical resistors in system\n");
int counter_r; //For computing the total number of resistors in the thermal model

for(i=1; i<num_nodes; i++){
	
	fprintf(fp,"//");
	for(j=0;j<i;j++){
		fprintf(fp,"%.2f\t",r_value[i][j]); // Change the floating bits to see the more accurate values of resistors
		if(r_value[i][j]>0)
			counter_r++;
		}
	fprintf(fp,"\n");
}

fprintf(fp,"\n\n\n");

//Print the total number of resistors in the thermal model
fprintf(fp,"\n\t //The total numbe of inside resistors is : %d.\n",counter_r);

//Recheck the values generated from Hotspot. Can compare them with the values in the r_value matrix

/*fprintf(fp, "Printing resistor source value matrix from Hotspot:\n");

for(i=0;i<num_nodes;i++){
	for(j=0;j<num_nodes;j++){
	fprintf(fp,"%.10f\t",model->block->b[i][j]); // b[][] is defined in Hotspot	
	}
	fprintf(fp,"\n");		
	}
*/

//---------------------------------------------------------------------------------------------
//Print external resistors between some nodes and environment
double *external_r = dvector(num_units+EXTRA); // store the resistor between nodes and environment

//Extract these resistors' values from Hotspot
for(i=0; i< num_units+EXTRA; i++){
	double r_ambient = 1/model->block->g_amb[i];
	if(r_ambient<999999){
	external_r[i]=r_ambient;
	counter_r++;
	}else{
	external_r[i]=0;	
	}

}

//Print all resistors convect to the ambient
fprintf(fp,"\t// All convective environment resistor\n");
fprintf(fp,"\t //");

for(i=0; i< num_units+EXTRA; i++){
	fprintf(fp,"%.8f\t",external_r[i]);
}

/*
fprintf(fp,"\n");
fprintf(fp,"\n\t //The total numbe of resistors is : %d.\n",counter_r);
fprintf(fp,"\n");
*/

//Print the total number of resistors on the screen
cout<<"The total number of resistors is \t "<<counter_r<<"\n";
cout<<"\n";


// All capacitors in the thermal network
double *c_value = dvector(num_nodes);
for (i=0;i<num_nodes;i++){
	c_value[i] = model->block->a[i];
}

//-----------------------------------------------------------------------------------------------------
//Print all capacitors in the thermal model, they are mandatory for runing transient thermal simulation
//If you only do the steady thermal simluation, you should comment them in here or in your final script
//-----------------------------------------------------------------------------------------------------

//Print all capacitors of the themral model in your script, you can comment them
fprintf(fp,"\t // All capactors in the thermal model\n");
fprintf(fp,"//");

for(i=0;i<num_nodes;i++){
	fprintf(fp,"%.8f\t",c_value[i]);
}
fprintf(fp,"\n");fprintf(fp,"\n");
fprintf(fp,"\t //The total number of capacitors is : %d.\n",num_nodes);

//Print the total number of capacitors in the thermal model
cout<<"The total number of capacitors is \t "<<num_nodes<<"\n";
cout<<"\n";

fprintf(fp,"\n\n");


//-----------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------------------------
//Print the definition resistors!!!!!!!!!!!!!!!!!!!!!!!! Critical point!
//First setp: define the environment convective resistor. Analyze g_amb
fprintf(fp,"\t\t//Define all resistor convect to the ambient!\n");

for (i=0; i<num_units+EXTRA; i++){
	if(i<num_units){
	if(model->block->g_amb[i] > 0){
		fprintf(fp,"\t\t sca_eln::sca_r* r_amb_%s;\n",flp->units[i].name);// the system floorplan mapped in sink layer resistors to environment 
	}// end >0 if
	}else{
	if(model->block->g_amb[i] > 0){
		if (i-num_units == SP_W){ // Using the index number to search the related extra_nodes
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SP_W;\n");
		}else if(i-num_units == SP_E){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SP_E;\n");		
		}else if(i-num_units == SP_N){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SP_N;\n");
		}else if(i-num_units == SP_S){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SP_S;\n");
		}else if(i-num_units == SINK_C_W){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_C_W;\n");
		}else if(i-num_units == SINK_C_E){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_C_E;\n");
		}else if(i-num_units == SINK_C_N){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_C_N;\n");
		}else if(i- num_units == SINK_C_S){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_C_S;\n");
		}else if(i- num_units == SINK_W){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_W;\n");
		}else if(i - num_units == SINK_E){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_E;\n");
		}else if(i-num_units == SINK_N){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_N;\n");
		}else if(i - num_units == SINK_S){
			fprintf(fp,"\t\t sca_eln::sca_r* r_amb_SINK_S;\n");
		}
	}// end extra_nodes >0 if
	}
//fprintf(fp,"\n");
}// end for

fprintf(fp,"\n\n");

//-----------------------------------------------------------------------------------------------------
//Second step: define the lateral and vertical resistors in the half matrix r_value

//Print the values of resistos 
/*
int counter_r;
for(i=1; i<num_nodes; i++){
fprintf(fp,"//");
for(j=0;j<i;j++){
	fprintf(fp,"%.2f\t",r_value[i][j]);
	if(r_value[i][j]>0)
		counter_r++;
	}
fprintf(fp,"\n");
}
*/


/////////////////////////////////////////////////////////////////////////////////////////////
//Critical points: Define the resistos in different layers
/////////////////////////////////////////////////////////////////////////////////////////////

//For indexing the resistos in the floorplan within the whole fours layers
int div_i = 0;
int div_j = 0;
int rem_i = 0;
int rem_j = 0;


fprintf(fp,"\t\t //All laterl and vertical resistos in thermal model inner structure.\n");

for(i=1;i<num_nodes;i++){
	if(i<4*num_units && i%num_units==0){
		fprintf(fp,"\n\n"); //Seperate the resistors belong to different layers
	}
	if(i>=4*num_units && i%4==0){
		fprintf(fp,"\n\n"); //
	}

	for(j=0;j<i;j++){
	 div_i = 0;
	 div_j = 0;
	 rem_i = 0;
	 rem_j = 0;
		if(r_value[i][j]>0){
			if(i<num_units){// this resistor is in silicon layer

				rem_i = i%num_units;
				rem_j = j%num_units;
				fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_silicon_layer_horizontal;\n", flp->units[rem_i].name,flp->units[rem_j].name);
			
			}else if(i>=num_units && i<2*num_units){// interface layer

				if(j<num_units){ // These resisots are connect to the below layer, vertical resistor.
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_silicon_iface_vertical;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}else{
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_iface_layer_horizontal;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}				
				
			}else if(i>= 2*num_units && i<3*num_units){// spreader layer

				if(j<2*num_units){
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_iface_hsp_vertical;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}else{
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_hsp_layer_horizontal;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}
		
			}else if(i>=3*num_units && i<4*num_units){// sink layer

				if(j<3*num_units){
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_hsp_hsink_vertical;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}else{
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_hsink_layer_horizontal;\n", flp->units[rem_i].name,flp->units[rem_j].name);
				}

			}else if(i>=4*num_units && i<4*num_units+4){// heat spread layer four direction extra nodes
				
				if(i==4*num_units){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SP_W_%s_hsp_border_horizontal;\n", flp->units[rem_j].name);
				}else if(i == 4*num_units+1){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SP_E_%s_hsp_border_horizontal;\n", flp->units[rem_j].name);
				}else if(i == 4*num_units+2){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SP_N_%s_hsp_border_horizontal;\n", flp->units[rem_j].name);
				}else if(i == 4*num_units+3){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SP_S_%s_hsp_border_horizontal;\n", flp->units[rem_j].name);
				}
					
			}else if(i>=4*num_units+4 && i<4*num_units+8){// heat spread boder mapped into heat sink first level border
				
				if(i==4*num_units+4){
					if(j<NL*num_units // Lateral resistors mapped by design floorplan in sink layer connect to the first level outer extra nodes
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_W_%s_sink_horizontal;\n", flp->units[rem_j].name);
					}else{
					rem_j = j-NL*num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_W_%s_vertical;\n", extra_nodes[rem_j]);	
					}
				}else if(i==4*num_units+5) {
					if(j<NL*num_units){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_E_%s_sink_horizontal;\n", flp->units[rem_j].name);
					}else{
					rem_j = j-NL*num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_E_%s_vertical;\n", extra_nodes[rem_j]);		
					}		
				}else if(i==4*num_units+6) {
					if(j<NL*num_units){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_N_%s_sink_horizontal;\n", flp->units[rem_j].name);
					}else{
					rem_j = j-NL*num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_N_%s_vertical;\n", extra_nodes[rem_j]);		
					}		
				}else if(i==4*num_units+7) {
					if(j<NL*num_units){
					rem_j = j%num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_S_%s_sink_horizontal;\n", flp->units[rem_j].name);
					}else{
					rem_j = j-NL*num_units;
					fprintf(fp,"\t\t sca_eln::sca_r* r_SINK_C_S_%s_vertical;\n", extra_nodes[rem_j]);		
					}
				}
			}else if (i>=4*num_units+8 && i<4*num_units+12){//
				rem_i = i - NL*num_units;
				rem_j = j - NL*num_units;
				fprintf(fp,"\t\t sca_eln::sca_r* r_%s_%s_horizontal;\n", extra_nodes[rem_i],extra_nodes[rem_j]);
			}
		}// end for if >0
	}// end for j

}// end for i

//-----------------------------------------------------------------------------------------------------------
//Print the definition of all capacitors in thermal model

fprintf(fp,"\n\n");
fprintf(fp,"\t\t //Define all capacitors in thermal netnork.\n");

for(i=0;i<num_nodes;i++){
	if(i<NL*num_units){
		if(i%num_units == 0){
			fprintf(fp,"\n");
		}
	div_i = i/num_units;
	rem_i = i%num_units;
	fprintf(fp,"\t\t sca_eln::sca_c* c_%s_%s;\n",flp->units[rem_i].name,layer_name[div_i]);
	}else{
		if(i%4 == 0){
			fprintf(fp,"\n");
		}
	rem_i = i - NL*num_units;
	fprintf(fp,"\t\t sca_eln::sca_c* c_%s;\n",extra_nodes[rem_i]);
	}
	
}


//--------------------------------------------------------------------------------------
//Print the definition of all nodes in the thermal model, the node is same as capacitors because each node has one related capacitor connect to ground
fprintf(fp,"\n\n");
fprintf(fp,"\t\t //Define all nodes in thermal netnork.\n");

for(i=0;i<num_nodes;i++){
	if(i<NL*num_units){
		if(i%num_units == 0){
			fprintf(fp,"\n");
		}
	div_i = i/num_units;
	rem_i = i%num_units;
	fprintf(fp,"\t\t sca_eln::sca_node node_%s_%s;\n",flp->units[rem_i].name,layer_name[div_i]);
	}else{
		if(i%4 == 0){
			fprintf(fp,"\n");
		}
	rem_i = i - NL*num_units;
	fprintf(fp,"\t\t sca_eln::sca_node node_%s;\n",extra_nodes[rem_i]);
	}
	
}

//---------------------------------------------------------------------------------------------------------------------
//Print the definition of the ground 
fprintf(fp,"\n\n");
fprintf(fp,"\t\t //Define GND in thermal netnork.\n");

fprintf(fp,"\t\t sca_eln::sca_node_ref\t gnd;\n");

//Start to print SC_CTOR
fprintf(fp,"\n\n");fprintf(fp,"\n\n");
fprintf(fp,"\t\t //SC_CTOR: instantiate all objects, connect resistors and capacitors\n");

fprintf(fp,"\t\t SC_CTOR(thermal_network)\n");
fprintf(fp,"\t\t {\n"); fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------------------------------
//Instatiate all current source of each unit
fprintf(fp,"\t\t // Instatiate all current source of each unit! The input value of the model!!!\n");


for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t // Instatiate the %s's current source! INPUTS!\n",flp->units[i].name);
	fprintf(fp,"\t\t itemp_%s = new sca_eln::sca_tdf::sca_isource(\"itemp_%s\");\n",flp->units[i].name,flp->units[i].name);
	fprintf(fp,"\t\t itemp_%s->p(gnd);\n",flp->units[i].name);
	fprintf(fp,"\t\t itemp_%s->n(node_%s_%s);\n",flp->units[i].name,flp->units[i].name,layer_name[0]);
	fprintf(fp,"\t\t itemp_%s->inp(p_%s);\n",flp->units[i].name,flp->units[i].name);
	fprintf(fp,"\n\n");
	
}
fprintf(fp,"\n\n");

//---------------------------------------------------------------------------------------------------------------------
//Instatiate all voltage sink of each unit
fprintf(fp,"\t\t // Instatiate all voltage sink of each unit! The output value of the model!!!\n");

for(i=0;i<num_units;i++){
	fprintf(fp,"\t\t // instatiate the %s's voltage sink! OUTPUTS!\n",flp->units[i].name);
	fprintf(fp,"\t\t vtemp_%s = new sca_eln::sca_tdf::sca_vsink(\"vtemp_%s\");\n",flp->units[i].name,flp->units[i].name);
	fprintf(fp,"\t\t vtemp_%s->p(node_%s_%s);\n",flp->units[i].name,flp->units[i].name,layer_name[0]);
	fprintf(fp,"\t\t vtemp_%s->n(gnd);\n",flp->units[i].name);
	fprintf(fp,"\t\t vtemp_%s->outp(t_%s);\n",flp->units[i].name,flp->units[i].name);
	fprintf(fp,"\n\n");
	
}

fprintf(fp,"\n");
fprintf(fp,"\t\t ///////////////////////////////////////////////////////////////////\n ");
fprintf(fp,"\t\t //Instatiate all resistors!\n");

//---------------------------------------------------------------------------------------------------------------------
//Instatiate all resistors convect to ambient 
fprintf(fp,"\t\t // Instatiate all external resistors between some nodes and environment!\n");

for(i=0;i<num_units+EXTRA;i++){
if(external_r[i]>0){
	if(i<num_units){
	fprintf(fp,"\t\t // resistor between node %s and ambient!\n",flp->units[i].name);
	fprintf(fp,"\t\t r_amb_%s = new sca_eln::sca_r(\"r_amb_%s\");\n",flp->units[i].name,flp->units[i].name);
	fprintf(fp,"\t\t r_amb_%s->p(node_%s_%s);\n",flp->units[i].name,flp->units[i].name,layer_name[3]);
	fprintf(fp,"\t\t r_amb_%s->n(gnd);\n",flp->units[i].name);
	fprintf(fp,"\t\t r_amb_%s->value = %.6f;\n",flp->units[i].name,external_r[i]);
	}else{
	fprintf(fp,"\t\t // resistor between node %s and ambient!\n",extra_nodes[i-num_units]);
	fprintf(fp,"\t\t r_amb_%s = new sca_eln::sca_r(\"r_amb_%s\");\n",extra_nodes[i-num_units],extra_nodes[i-num_units]);
	fprintf(fp,"\t\t r_amb_%s->p(node_%s);\n",extra_nodes[i-num_units],extra_nodes[i-num_units]);
	fprintf(fp,"\t\t r_amb_%s->n(gnd);\n",extra_nodes[i-num_units]);
	fprintf(fp,"\t\t r_amb_%s->value = %.6f;\n",extra_nodes[i-num_units],external_r[i]);
	}
	fprintf(fp,"\n");
}
}

//---------------------------------------------------------------------------------------------------------------------
//Instatiate all vertical and horizontal resistors
fprintf(fp,"\n\n");
fprintf(fp,"\t\t // Instatiate all vertical and horzontal resisors!\n");

for(i=1;i<num_nodes;i++){
	fprintf(fp,"\n");
	for(j=0;j<i;j++){
		div_i = 0;
		div_j = 0;
		rem_i = 0;
		rem_j = 0;
			if(r_value[i][j]>0){
				if(i<num_units){// silicon layer
					rem_i = i%num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t // Resistor between %s and %s in silicon layer!\n",flp->units[rem_i].name,flp->units[rem_j].name);
					fprintf(fp,"\t\t r_%s_%s_silicon_layer_horizontal = new sca_eln::sca_r(\"r_%s_%s_silicon_layer_horizontal\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
					fprintf(fp,"\t\t r_%s_%s_silicon_layer_horizontal->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[0]);
					fprintf(fp,"\t\t r_%s_%s_silicon_layer_horizontal->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[0]);
					fprintf(fp,"\t\t r_%s_%s_silicon_layer_horizontal->value = %.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
					fprintf(fp,"\n");
				}else if(i>=num_units && i<2*num_units){// interface layer
					if(j<num_units){
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t // Resistor of %s between silicon layer and interface layer!\n ",flp->units[rem_i].name);
						fprintf(fp,"\t\t r_%s_%s_silicon_iface_vertical = new sca_eln::sca_r(\"r_%s_%s_silicon_iface_vertical\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_silicon_iface_vertical->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[0]);
						fprintf(fp,"\t\t r_%s_%s_silicon_iface_vertical->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[1]);
						fprintf(fp,"\t\t r_%s_%s_silicon_iface_vertical->value = %.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");
					}else{
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t //Resistor between %s and %s in interface layer!\n ",flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_iface_layer_horizontal = new sca_eln::sca_r(\"r_%s_%s_iface_layer_horizontal\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_iface_layer_horizontal->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[1]);
						fprintf(fp,"\t\t r_%s_%s_iface_layer_horizontal->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[1]);
						fprintf(fp,"\t\t r_%s_%s_iface_layer_horizontal->value = %.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");
					}

				}else if(i>= 2*num_units && i<3*num_units){// spreader layer
					if(j<2*num_units){
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t // Resistor between %s in interface layer and spreader layer!\n",flp->units[rem_i].name);
						fprintf(fp,"\t\t r_%s_%s_iface_hsp_vertical = new sca_eln::sca_r(\"r_%s_%s_iface_hsp_vertical\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);	
						fprintf(fp,"\t\t r_%s_%s_iface_hsp_vertical->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[1]);
						fprintf(fp,"\t\t r_%s_%s_iface_hsp_vertical->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[2]);
						fprintf(fp,"\t\t r_%s_%s_iface_hsp_vertical->value=%.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");				
					}else{
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t //Resistor between %s and %s in spreader layer!\n ",flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_hsp_layer_horizontal = new sca_eln::sca_r(\"r_%s_%s_hsp_layer_horizontal\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_hsp_layer_horizontal->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[2]);
						fprintf(fp,"\t\t r_%s_%s_hsp_layer_horizontal->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[2]);
						fprintf(fp,"\t\t r_%s_%s_hsp_layer_horizontal->value=%.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");
					}

				}else if(i>=3*num_units && i<4*num_units){// sink layer
					if(j<3*num_units){
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t // Resistor between %s in spreader layer and sink layer!\n",flp->units[rem_i].name);
						fprintf(fp,"\t\t r_%s_%s_hsp_hsink_vertical = new sca_eln::sca_r(\"r_%s_%s_hsp_hsink_vertical\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_hsp_hsink_vertical->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[2]);
						fprintf(fp,"\t\t r_%s_%s_hsp_hsink_vertical->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[3]);
						fprintf(fp,"\t\t r_%s_%s_hsp_hsink_vertical->value=%.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");
					}else{
						rem_i = i%num_units;
						rem_j = j%num_units;
						fprintf(fp,"\t\t //Resistor between %s and %s in heat sink layer!\n ",flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_hsink_layer_horizontal = new sca_eln::sca_r(\"r_%s_%s_hsink_layer_horizontal\");\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,flp->units[rem_j].name);
						fprintf(fp,"\t\t r_%s_%s_hsink_layer_horizontal->p(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_i].name,layer_name[3]);
						fprintf(fp,"\t\t r_%s_%s_hsink_layer_horizontal->n(node_%s_%s);\n",flp->units[rem_i].name,flp->units[rem_j].name,flp->units[rem_j].name,layer_name[3]);
						fprintf(fp,"\t\t r_%s_%s_hsink_layer_horizontal->value=%.8f;\n",flp->units[rem_i].name,flp->units[rem_j].name,r_value[i][j]);
						fprintf(fp,"\n");
					}
				}else if(i>=4*num_units && i<4*num_units+4){// heat spread four direction
					// beautiful loop for extral nodes in spread layer
					rem_i = i-4*num_units;
					div_j = j/num_units;
					rem_j = j%num_units;
					fprintf(fp,"\t\t // Resistors between unit mapped in spread and spread border four direction->%s!\n",extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_hsp_border_horizontal = new sca_eln::sca_r(\"r_%s_%s_hsp_border_horizontal\");\n",extra_nodes[rem_i],flp->units[rem_j].name,extra_nodes[rem_i],flp->units[rem_j].name);
					fprintf(fp,"\t\t r_%s_%s_hsp_border_horizontal->p(node_%s_%s);\n",extra_nodes[rem_i],flp->units[rem_j].name,flp->units[rem_j].name,layer_name[2]);
					fprintf(fp,"\t\t r_%s_%s_hsp_border_horizontal->n(node_%s);\n",extra_nodes[rem_i],flp->units[rem_j].name,extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_hsp_border_horizontal->value=%.8f;\n",extra_nodes[rem_i],flp->units[rem_j].name,r_value[i][j]);
					fprintf(fp,"\n");
					
				}else if(i>=4*num_units+4 && i<4*num_units+8){// heat spread boder mapped into heat sink first level border
					rem_i = i-4*num_units;
					div_j = j/num_units;
					//rem_j = j%num_units;
					if(j<NL*num_units){
					rem_j = j%num_units;
					fprintf(fp,"\t\t // Resistor between %s in heat sink layer and heat sink 1st level border!\n ",flp->units[rem_j].name);
					fprintf(fp,"\t\t r_%s_%s_sink_horizontal =new sca_eln::sca_r(\"r_%s_%s_sink_horizontal\"); \n",extra_nodes[rem_i],flp->units[rem_j].name,extra_nodes[rem_i],flp->units[rem_j].name);
					fprintf(fp,"\t\t r_%s_%s_sink_horizontal->p(node_%s_%s);\n",extra_nodes[rem_i],flp->units[rem_j].name,flp->units[rem_j].name,layer_name[div_j]);
					fprintf(fp,"\t\t r_%s_%s_sink_horizontal->n(node_%s);\n",extra_nodes[rem_i],flp->units[rem_j].name,extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_sink_horizontal->value=%.8f;\n",extra_nodes[rem_i],flp->units[rem_j].name,r_value[i][j]);
					fprintf(fp,"\n");
					}else{
					rem_j = j-NL*num_units;
					fprintf(fp,"\t\t // Resistor between %s in preader border and heat sink 1st level border!\n ",extra_nodes[rem_j]);
					fprintf(fp,"\t\t r_%s_%s_vertical= new sca_eln::sca_r(\"r_%s_%s_vertical\");\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_i],extra_nodes[rem_j]);
					fprintf(fp,"\t\t r_%s_%s_vertical->p(node_%s);\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_j]);
					fprintf(fp,"\t\t r_%s_%s_vertical->n(node_%s);\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_vertical->value=%.8f;\n",extra_nodes[rem_i],extra_nodes[rem_j],r_value[i][j]);
					fprintf(fp,"\n");
					}

				}else if(i>=4*num_units+8 && i<4*num_units+12){
					rem_i = i - NL*num_units;
					rem_j = j - NL*num_units;
					fprintf(fp,"\t\t // Resistor between 1st and 2nd level border in sink layer of direction %s!!\n",extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_horizontal = new sca_eln::sca_r(\"r_%s_%s_horizontal\");\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_i],extra_nodes[rem_j]);
					fprintf(fp,"\t\t r_%s_%s_horizontal->p(node_%s);\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_j]);
					fprintf(fp,"\t\t r_%s_%s_horizontal->n(node_%s);\n",extra_nodes[rem_i],extra_nodes[rem_j],extra_nodes[rem_i]);
					fprintf(fp,"\t\t r_%s_%s_horizontal->value=%.8f;\n",extra_nodes[rem_i],extra_nodes[rem_j],r_value[i][j]);
					fprintf(fp,"\n");

				}
			}// end for if>0
	}// end for j
}// end for i;
fprintf(fp,"\n\n\n");

//---------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------
//Print the definition of all capacitors!!!!!!
fprintf(fp,"\t\t // Internal capacitor instantiation!!!\n");

for(i=0;i<num_nodes;i++){
	if(i<NL*num_units){
	rem_i = i%num_units;
	div_i = i/num_units;
	fprintf(fp,"\t\t // Capacitor of %s_%s in the thermal model!\n",flp->units[rem_i].name,layer_name[div_i]);
	fprintf(fp,"\t\t c_%s_%s = new sca_eln::sca_c(\"c_%s_%s\");\n",flp->units[rem_i].name,layer_name[div_i],flp->units[rem_i].name,layer_name[div_i]);
	fprintf(fp,"\t\t c_%s_%s->p(node_%s_%s);\n",flp->units[rem_i].name,layer_name[div_i],flp->units[rem_i].name,layer_name[div_i]);
	fprintf(fp,"\t\t c_%s_%s->n(gnd);\n",flp->units[rem_i].name,layer_name[div_i]);
	fprintf(fp,"\t\t c_%s_%s->value=%.8f;\n",flp->units[rem_i].name,layer_name[div_i],c_value[i]);
	fprintf(fp,"\n");
	}else{
	rem_i = i - NL*num_units;
	fprintf(fp,"\t\t // Capacitor of %s in the thermal model!\n",extra_nodes[rem_i]);
	fprintf(fp,"\t\t c_%s = new sca_eln::sca_c(\"c_%s\");\n",extra_nodes[rem_i],extra_nodes[rem_i]);
	fprintf(fp,"\t\t c_%s->p(node_%s);\n",extra_nodes[rem_i],extra_nodes[rem_i]);
	fprintf(fp,"\t\t c_%s->n(gnd);\n",extra_nodes[rem_i]);
	fprintf(fp,"\t\t c_%s->value=%.8f;\n",extra_nodes[rem_i],c_value[i]);
	fprintf(fp,"\n");
	}

}

//Do not forget these!
fprintf(fp,"\n\n");
fprintf(fp,"\t\t }// end CTOR\n"); // for ctor
fprintf(fp,"\n};// end module\n\n\n"); // } for module




}//CTOR	

};//MODULE
