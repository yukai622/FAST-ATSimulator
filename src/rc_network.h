/******************************************************/
/***This file is used to simulated in systemc-ams. ****/
/***                  Yukai Chen                   ****/
/***                     EDA                       ****/
/******************************************************/



#include "systemc-ams.h"



SC_MODULE (thermal_network)
{
		 //Interface declaration: input
		 sca_tdf::sca_in<double> p_CORE ; // Power of the CORE 
		 sca_tdf::sca_in<double> p_MEM ; // Power of the MEM 
		 sca_tdf::sca_in<double> p_RFTR ; // Power of the RFTR 
		 sca_tdf::sca_in<double> p_UART ; // Power of the UART 


		 //Interface declaration: output
		 sca_tdf::sca_out<double> t_CORE ; // Temperature of the CORE
		 sca_tdf::sca_out<double> t_MEM ; // Temperature of the MEM
		 sca_tdf::sca_out<double> t_RFTR ; // Temperature of the RFTR
		 sca_tdf::sca_out<double> t_UART ; // Temperature of the UART


		 //Define current source of each unit
		 sca_eln::sca_tdf::sca_isource* itemp_CORE;
		 sca_eln::sca_tdf::sca_isource* itemp_MEM;
		 sca_eln::sca_tdf::sca_isource* itemp_RFTR;
		 sca_eln::sca_tdf::sca_isource* itemp_UART;

		 //Define voltage sink of each unit
		 sca_eln::sca_tdf::sca_vsink* vtemp_CORE;
		 sca_eln::sca_tdf::sca_vsink* vtemp_MEM;
		 sca_eln::sca_tdf::sca_vsink* vtemp_RFTR;
		 sca_eln::sca_tdf::sca_vsink* vtemp_UART;






	 //The total numbe of inside resistors is : 52.
	// All convective environment resistor
	 //377.25000127	31.43750007	94.30838712	85.50657324	0.00000000	0.00000000	0.00000000	0.00000000	1.61633660	1.61633660	1.83227443	1.83227443	0.55888889	0.55888889	0.55888889	0.55888889	

	 //The total numbe of resistors is : 64.

	 // All capactors in the thermal model
//0.00008741	0.00104895	0.00034967	0.00038566	0.00002664	0.00031968	0.00010656	0.00011753	0.00118215	0.01418580	0.00472881	0.00521558	0.02114383	0.25372602	0.08457903	0.09328536	0.27591164	0.27591164	0.24339481	0.24339481	4.93493234	4.93493234	4.35333901	4.35333901	14.27208863	14.27208863	14.27208863	14.27208863	

	 //The total number of capacitors is : 28.


		//Define all resistor convect to the ambient!
		 sca_eln::sca_r* r_amb_CORE;
		 sca_eln::sca_r* r_amb_MEM;
		 sca_eln::sca_r* r_amb_RFTR;
		 sca_eln::sca_r* r_amb_UART;
		 sca_eln::sca_r* r_amb_SINK_C_W;
		 sca_eln::sca_r* r_amb_SINK_C_E;
		 sca_eln::sca_r* r_amb_SINK_C_N;
		 sca_eln::sca_r* r_amb_SINK_C_S;
		 sca_eln::sca_r* r_amb_SINK_W;
		 sca_eln::sca_r* r_amb_SINK_E;
		 sca_eln::sca_r* r_amb_SINK_N;
		 sca_eln::sca_r* r_amb_SINK_S;


		 //All laterl and vertical resistos in thermal model inner structure.
		 sca_eln::sca_r* r_MEM_CORE_silicon_layer_horizontal;
		 sca_eln::sca_r* r_UART_CORE_silicon_layer_horizontal;
		 sca_eln::sca_r* r_UART_MEM_silicon_layer_horizontal;
		 sca_eln::sca_r* r_UART_RFTR_silicon_layer_horizontal;


		 sca_eln::sca_r* r_CORE_CORE_silicon_iface_vertical;
		 sca_eln::sca_r* r_MEM_MEM_silicon_iface_vertical;
		 sca_eln::sca_r* r_MEM_CORE_iface_layer_horizontal;
		 sca_eln::sca_r* r_RFTR_RFTR_silicon_iface_vertical;
		 sca_eln::sca_r* r_UART_UART_silicon_iface_vertical;
		 sca_eln::sca_r* r_UART_CORE_iface_layer_horizontal;
		 sca_eln::sca_r* r_UART_MEM_iface_layer_horizontal;
		 sca_eln::sca_r* r_UART_RFTR_iface_layer_horizontal;


		 sca_eln::sca_r* r_CORE_CORE_iface_hsp_vertical;
		 sca_eln::sca_r* r_MEM_MEM_iface_hsp_vertical;
		 sca_eln::sca_r* r_MEM_CORE_hsp_layer_horizontal;
		 sca_eln::sca_r* r_RFTR_RFTR_iface_hsp_vertical;
		 sca_eln::sca_r* r_UART_UART_iface_hsp_vertical;
		 sca_eln::sca_r* r_UART_CORE_hsp_layer_horizontal;
		 sca_eln::sca_r* r_UART_MEM_hsp_layer_horizontal;
		 sca_eln::sca_r* r_UART_RFTR_hsp_layer_horizontal;


		 sca_eln::sca_r* r_CORE_CORE_hsp_hsink_vertical;
		 sca_eln::sca_r* r_MEM_MEM_hsp_hsink_vertical;
		 sca_eln::sca_r* r_MEM_CORE_hsink_layer_horizontal;
		 sca_eln::sca_r* r_RFTR_RFTR_hsp_hsink_vertical;
		 sca_eln::sca_r* r_UART_UART_hsp_hsink_vertical;
		 sca_eln::sca_r* r_UART_CORE_hsink_layer_horizontal;
		 sca_eln::sca_r* r_UART_MEM_hsink_layer_horizontal;
		 sca_eln::sca_r* r_UART_RFTR_hsink_layer_horizontal;


		 sca_eln::sca_r* r_SP_W_CORE_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_W_MEM_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_W_RFTR_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_E_MEM_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_E_RFTR_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_E_UART_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_N_RFTR_hsp_border_horizontal;
		 sca_eln::sca_r* r_SP_S_MEM_hsp_border_horizontal;


		 sca_eln::sca_r* r_SINK_C_W_CORE_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_W_MEM_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_W_RFTR_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_W_SP_W_vertical;
		 sca_eln::sca_r* r_SINK_C_E_MEM_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_E_RFTR_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_E_UART_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_E_SP_E_vertical;
		 sca_eln::sca_r* r_SINK_C_N_RFTR_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_N_SP_N_vertical;
		 sca_eln::sca_r* r_SINK_C_S_MEM_sink_horizontal;
		 sca_eln::sca_r* r_SINK_C_S_SP_S_vertical;


		 sca_eln::sca_r* r_SINK_W_SINK_C_W_horizontal;
		 sca_eln::sca_r* r_SINK_E_SINK_C_E_horizontal;
		 sca_eln::sca_r* r_SINK_N_SINK_C_N_horizontal;
		 sca_eln::sca_r* r_SINK_S_SINK_C_S_horizontal;


		 //Define all capacitors in thermal netnork.
		/*

		 sca_eln::sca_c* c_CORE_SILICON;
		 sca_eln::sca_c* c_MEM_SILICON;
		 sca_eln::sca_c* c_RFTR_SILICON;
		 sca_eln::sca_c* c_UART_SILICON;

		 sca_eln::sca_c* c_CORE_INTERFACE;
		 sca_eln::sca_c* c_MEM_INTERFACE;
		 sca_eln::sca_c* c_RFTR_INTERFACE;
		 sca_eln::sca_c* c_UART_INTERFACE;

		 sca_eln::sca_c* c_CORE_HEATSPREAD;
		 sca_eln::sca_c* c_MEM_HEATSPREAD;
		 sca_eln::sca_c* c_RFTR_HEATSPREAD;
		 sca_eln::sca_c* c_UART_HEATSPREAD;

		 sca_eln::sca_c* c_CORE_HEATSINK;
		 sca_eln::sca_c* c_MEM_HEATSINK;
		 sca_eln::sca_c* c_RFTR_HEATSINK;
		 sca_eln::sca_c* c_UART_HEATSINK;

		 sca_eln::sca_c* c_SP_W;
		 sca_eln::sca_c* c_SP_E;
		 sca_eln::sca_c* c_SP_N;
		 sca_eln::sca_c* c_SP_S;

		 sca_eln::sca_c* c_SINK_C_W;
		 sca_eln::sca_c* c_SINK_C_E;
		 sca_eln::sca_c* c_SINK_C_N;
		 sca_eln::sca_c* c_SINK_C_S;

		 sca_eln::sca_c* c_SINK_W;
		 sca_eln::sca_c* c_SINK_E;
		 sca_eln::sca_c* c_SINK_N;
		 sca_eln::sca_c* c_SINK_S;
		*/

		 //Define all nodes in thermal netnork.

		 sca_eln::sca_node node_CORE_SILICON;
		 sca_eln::sca_node node_MEM_SILICON;
		 sca_eln::sca_node node_RFTR_SILICON;
		 sca_eln::sca_node node_UART_SILICON;

		 sca_eln::sca_node node_CORE_INTERFACE;
		 sca_eln::sca_node node_MEM_INTERFACE;
		 sca_eln::sca_node node_RFTR_INTERFACE;
		 sca_eln::sca_node node_UART_INTERFACE;

		 sca_eln::sca_node node_CORE_HEATSPREAD;
		 sca_eln::sca_node node_MEM_HEATSPREAD;
		 sca_eln::sca_node node_RFTR_HEATSPREAD;
		 sca_eln::sca_node node_UART_HEATSPREAD;

		 sca_eln::sca_node node_CORE_HEATSINK;
		 sca_eln::sca_node node_MEM_HEATSINK;
		 sca_eln::sca_node node_RFTR_HEATSINK;
		 sca_eln::sca_node node_UART_HEATSINK;

		 sca_eln::sca_node node_SP_W;
		 sca_eln::sca_node node_SP_E;
		 sca_eln::sca_node node_SP_N;
		 sca_eln::sca_node node_SP_S;

		 sca_eln::sca_node node_SINK_C_W;
		 sca_eln::sca_node node_SINK_C_E;
		 sca_eln::sca_node node_SINK_C_N;
		 sca_eln::sca_node node_SINK_C_S;

		 sca_eln::sca_node node_SINK_W;
		 sca_eln::sca_node node_SINK_E;
		 sca_eln::sca_node node_SINK_N;
		 sca_eln::sca_node node_SINK_S;


		 //Define GND in thermal netnork.
		 sca_eln::sca_node_ref	 gnd;




		 //SC_CTOR: instantiate all objects, connect resistors and capacitors
		 SC_CTOR(thermal_network)
		 {


		 // Instatiate all current source of each unit! The input value of the model!!!
		 // Instatiate the CORE's current source! INPUTS!
		 itemp_CORE = new sca_eln::sca_tdf::sca_isource("itemp_CORE");
		 itemp_CORE->p(gnd);
		 itemp_CORE->n(node_CORE_SILICON);
		 itemp_CORE->inp(p_CORE);


		 // Instatiate the MEM's current source! INPUTS!
		 itemp_MEM = new sca_eln::sca_tdf::sca_isource("itemp_MEM");
		 itemp_MEM->p(gnd);
		 itemp_MEM->n(node_MEM_SILICON);
		 itemp_MEM->inp(p_MEM);


		 // Instatiate the RFTR's current source! INPUTS!
		 itemp_RFTR = new sca_eln::sca_tdf::sca_isource("itemp_RFTR");
		 itemp_RFTR->p(gnd);
		 itemp_RFTR->n(node_RFTR_SILICON);
		 itemp_RFTR->inp(p_RFTR);


		 // Instatiate the UART's current source! INPUTS!
		 itemp_UART = new sca_eln::sca_tdf::sca_isource("itemp_UART");
		 itemp_UART->p(gnd);
		 itemp_UART->n(node_UART_SILICON);
		 itemp_UART->inp(p_UART);




		 // Instatiate all voltage sink of each unit! The output value of the model!!!
		 // instatiate the CORE's voltage sink! OUTPUTS!
		 vtemp_CORE = new sca_eln::sca_tdf::sca_vsink("vtemp_CORE");
		 vtemp_CORE->p(node_CORE_SILICON);
		 vtemp_CORE->n(gnd);
		 vtemp_CORE->outp(t_CORE);


		 // instatiate the MEM's voltage sink! OUTPUTS!
		 vtemp_MEM = new sca_eln::sca_tdf::sca_vsink("vtemp_MEM");
		 vtemp_MEM->p(node_MEM_SILICON);
		 vtemp_MEM->n(gnd);
		 vtemp_MEM->outp(t_MEM);


		 // instatiate the RFTR's voltage sink! OUTPUTS!
		 vtemp_RFTR = new sca_eln::sca_tdf::sca_vsink("vtemp_RFTR");
		 vtemp_RFTR->p(node_RFTR_SILICON);
		 vtemp_RFTR->n(gnd);
		 vtemp_RFTR->outp(t_RFTR);


		 // instatiate the UART's voltage sink! OUTPUTS!
		 vtemp_UART = new sca_eln::sca_tdf::sca_vsink("vtemp_UART");
		 vtemp_UART->p(node_UART_SILICON);
		 vtemp_UART->n(gnd);
		 vtemp_UART->outp(t_UART);



		 ///////////////////////////////////////////////////////////////////
 		 //Instatiate all resistors!
		 // Instatiate all external resistors between some nodes and environment!
		 // resistor between node CORE and ambient!
		 r_amb_CORE = new sca_eln::sca_r("r_amb_CORE");
		 r_amb_CORE->p(node_CORE_HEATSINK);
		 r_amb_CORE->n(gnd);
		 r_amb_CORE->value = 377.250001;

		 // resistor between node MEM and ambient!
		 r_amb_MEM = new sca_eln::sca_r("r_amb_MEM");
		 r_amb_MEM->p(node_MEM_HEATSINK);
		 r_amb_MEM->n(gnd);
		 r_amb_MEM->value = 31.437500;

		 // resistor between node RFTR and ambient!
		 r_amb_RFTR = new sca_eln::sca_r("r_amb_RFTR");
		 r_amb_RFTR->p(node_RFTR_HEATSINK);
		 r_amb_RFTR->n(gnd);
		 r_amb_RFTR->value = 94.308387;

		 // resistor between node UART and ambient!
		 r_amb_UART = new sca_eln::sca_r("r_amb_UART");
		 r_amb_UART->p(node_UART_HEATSINK);
		 r_amb_UART->n(gnd);
		 r_amb_UART->value = 85.506573;

		 // resistor between node SINK_C_W and ambient!
		 r_amb_SINK_C_W = new sca_eln::sca_r("r_amb_SINK_C_W");
		 r_amb_SINK_C_W->p(node_SINK_C_W);
		 r_amb_SINK_C_W->n(gnd);
		 r_amb_SINK_C_W->value = 1.616337;

		 // resistor between node SINK_C_E and ambient!
		 r_amb_SINK_C_E = new sca_eln::sca_r("r_amb_SINK_C_E");
		 r_amb_SINK_C_E->p(node_SINK_C_E);
		 r_amb_SINK_C_E->n(gnd);
		 r_amb_SINK_C_E->value = 1.616337;

		 // resistor between node SINK_C_N and ambient!
		 r_amb_SINK_C_N = new sca_eln::sca_r("r_amb_SINK_C_N");
		 r_amb_SINK_C_N->p(node_SINK_C_N);
		 r_amb_SINK_C_N->n(gnd);
		 r_amb_SINK_C_N->value = 1.832274;

		 // resistor between node SINK_C_S and ambient!
		 r_amb_SINK_C_S = new sca_eln::sca_r("r_amb_SINK_C_S");
		 r_amb_SINK_C_S->p(node_SINK_C_S);
		 r_amb_SINK_C_S->n(gnd);
		 r_amb_SINK_C_S->value = 1.832274;

		 // resistor between node SINK_W and ambient!
		 r_amb_SINK_W = new sca_eln::sca_r("r_amb_SINK_W");
		 r_amb_SINK_W->p(node_SINK_W);
		 r_amb_SINK_W->n(gnd);
		 r_amb_SINK_W->value = 0.558889;

		 // resistor between node SINK_E and ambient!
		 r_amb_SINK_E = new sca_eln::sca_r("r_amb_SINK_E");
		 r_amb_SINK_E->p(node_SINK_E);
		 r_amb_SINK_E->n(gnd);
		 r_amb_SINK_E->value = 0.558889;

		 // resistor between node SINK_N and ambient!
		 r_amb_SINK_N = new sca_eln::sca_r("r_amb_SINK_N");
		 r_amb_SINK_N->p(node_SINK_N);
		 r_amb_SINK_N->n(gnd);
		 r_amb_SINK_N->value = 0.558889;

		 // resistor between node SINK_S and ambient!
		 r_amb_SINK_S = new sca_eln::sca_r("r_amb_SINK_S");
		 r_amb_SINK_S->p(node_SINK_S);
		 r_amb_SINK_S->n(gnd);
		 r_amb_SINK_S->value = 0.558889;



		 // Instatiate all vertical and horzontal resisots!

		 // Resistor between MEM and CORE in silicon layer!
		 r_MEM_CORE_silicon_layer_horizontal = new sca_eln::sca_r("r_MEM_CORE_silicon_layer_horizontal");
		 r_MEM_CORE_silicon_layer_horizontal->p(node_MEM_SILICON);
		 r_MEM_CORE_silicon_layer_horizontal->n(node_CORE_SILICON);
		 r_MEM_CORE_silicon_layer_horizontal->value = 215.48122572;



		 // Resistor between UART and CORE in silicon layer!
		 r_UART_CORE_silicon_layer_horizontal = new sca_eln::sca_r("r_UART_CORE_silicon_layer_horizontal");
		 r_UART_CORE_silicon_layer_horizontal->p(node_UART_SILICON);
		 r_UART_CORE_silicon_layer_horizontal->n(node_CORE_SILICON);
		 r_UART_CORE_silicon_layer_horizontal->value = 89.59696841;

		 // Resistor between UART and MEM in silicon layer!
		 r_UART_MEM_silicon_layer_horizontal = new sca_eln::sca_r("r_UART_MEM_silicon_layer_horizontal");
		 r_UART_MEM_silicon_layer_horizontal->p(node_UART_SILICON);
		 r_UART_MEM_silicon_layer_horizontal->n(node_MEM_SILICON);
		 r_UART_MEM_silicon_layer_horizontal->value = 49.36941385;

		 // Resistor between UART and RFTR in silicon layer!
		 r_UART_RFTR_silicon_layer_horizontal = new sca_eln::sca_r("r_UART_RFTR_silicon_layer_horizontal");
		 r_UART_RFTR_silicon_layer_horizontal->p(node_UART_SILICON);
		 r_UART_RFTR_silicon_layer_horizontal->n(node_RFTR_SILICON);
		 r_UART_RFTR_silicon_layer_horizontal->value = 26.69764363;


		 // Resistor of CORE between silicon layer and interface layer!
 		 r_CORE_CORE_silicon_iface_vertical = new sca_eln::sca_r("r_CORE_CORE_silicon_iface_vertical");
		 r_CORE_CORE_silicon_iface_vertical->p(node_CORE_SILICON);
		 r_CORE_CORE_silicon_iface_vertical->n(node_CORE_INTERFACE);
		 r_CORE_CORE_silicon_iface_vertical->value = 1.50000001;


		 // Resistor of MEM between silicon layer and interface layer!
 		 r_MEM_MEM_silicon_iface_vertical = new sca_eln::sca_r("r_MEM_MEM_silicon_iface_vertical");
		 r_MEM_MEM_silicon_iface_vertical->p(node_MEM_SILICON);
		 r_MEM_MEM_silicon_iface_vertical->n(node_MEM_INTERFACE);
		 r_MEM_MEM_silicon_iface_vertical->value = 0.12500000;

		 //Resistor between MEM and CORE in interface layer!
 		 r_MEM_CORE_iface_layer_horizontal = new sca_eln::sca_r("r_MEM_CORE_iface_layer_horizontal");
		 r_MEM_CORE_iface_layer_horizontal->p(node_MEM_INTERFACE);
		 r_MEM_CORE_iface_layer_horizontal->n(node_CORE_INTERFACE);
		 r_MEM_CORE_iface_layer_horizontal->value = 40402.72982180;


		 // Resistor of RFTR between silicon layer and interface layer!
 		 r_RFTR_RFTR_silicon_iface_vertical = new sca_eln::sca_r("r_RFTR_RFTR_silicon_iface_vertical");
		 r_RFTR_RFTR_silicon_iface_vertical->p(node_RFTR_SILICON);
		 r_RFTR_RFTR_silicon_iface_vertical->n(node_RFTR_INTERFACE);
		 r_RFTR_RFTR_silicon_iface_vertical->value = 0.37498365;


		 // Resistor of UART between silicon layer and interface layer!
 		 r_UART_UART_silicon_iface_vertical = new sca_eln::sca_r("r_UART_UART_silicon_iface_vertical");
		 r_UART_UART_silicon_iface_vertical->p(node_UART_SILICON);
		 r_UART_UART_silicon_iface_vertical->n(node_UART_INTERFACE);
		 r_UART_UART_silicon_iface_vertical->value = 0.33998637;

		 //Resistor between UART and CORE in interface layer!
 		 r_UART_CORE_iface_layer_horizontal = new sca_eln::sca_r("r_UART_CORE_iface_layer_horizontal");
		 r_UART_CORE_iface_layer_horizontal->p(node_UART_INTERFACE);
		 r_UART_CORE_iface_layer_horizontal->n(node_CORE_INTERFACE);
		 r_UART_CORE_iface_layer_horizontal->value = 16799.43157772;

		 //Resistor between UART and MEM in interface layer!
 		 r_UART_MEM_iface_layer_horizontal = new sca_eln::sca_r("r_UART_MEM_iface_layer_horizontal");
		 r_UART_MEM_iface_layer_horizontal->p(node_UART_INTERFACE);
		 r_UART_MEM_iface_layer_horizontal->n(node_MEM_INTERFACE);
		 r_UART_MEM_iface_layer_horizontal->value = 9256.76509633;

		 //Resistor between UART and RFTR in interface layer!
 		 r_UART_RFTR_iface_layer_horizontal = new sca_eln::sca_r("r_UART_RFTR_iface_layer_horizontal");
		 r_UART_RFTR_iface_layer_horizontal->p(node_UART_INTERFACE);
		 r_UART_RFTR_iface_layer_horizontal->n(node_RFTR_INTERFACE);
		 r_UART_RFTR_iface_layer_horizontal->value = 5005.80817997;


		 // Resistor between CORE in interface layer and spreader layer!
		 r_CORE_CORE_iface_hsp_vertical = new sca_eln::sca_r("r_CORE_CORE_iface_hsp_vertical");
		 r_CORE_CORE_iface_hsp_vertical->p(node_CORE_INTERFACE);
		 r_CORE_CORE_iface_hsp_vertical->n(node_CORE_HEATSPREAD);
		 r_CORE_CORE_iface_hsp_vertical->value=5.00000002;


		 // Resistor between MEM in interface layer and spreader layer!
		 r_MEM_MEM_iface_hsp_vertical = new sca_eln::sca_r("r_MEM_MEM_iface_hsp_vertical");
		 r_MEM_MEM_iface_hsp_vertical->p(node_MEM_INTERFACE);
		 r_MEM_MEM_iface_hsp_vertical->n(node_MEM_HEATSPREAD);
		 r_MEM_MEM_iface_hsp_vertical->value=0.41666667;

		 //Resistor between MEM and CORE in spreader layer!
 		 r_MEM_CORE_hsp_layer_horizontal = new sca_eln::sca_r("r_MEM_CORE_hsp_layer_horizontal");
		 r_MEM_CORE_hsp_layer_horizontal->p(node_MEM_HEATSPREAD);
		 r_MEM_CORE_hsp_layer_horizontal->n(node_CORE_HEATSPREAD);
		 r_MEM_CORE_hsp_layer_horizontal->value=8.08054596;


		 // Resistor between RFTR in interface layer and spreader layer!
		 r_RFTR_RFTR_iface_hsp_vertical = new sca_eln::sca_r("r_RFTR_RFTR_iface_hsp_vertical");
		 r_RFTR_RFTR_iface_hsp_vertical->p(node_RFTR_INTERFACE);
		 r_RFTR_RFTR_iface_hsp_vertical->n(node_RFTR_HEATSPREAD);
		 r_RFTR_RFTR_iface_hsp_vertical->value=1.24994549;


		 // Resistor between UART in interface layer and spreader layer!
		 r_UART_UART_iface_hsp_vertical = new sca_eln::sca_r("r_UART_UART_iface_hsp_vertical");
		 r_UART_UART_iface_hsp_vertical->p(node_UART_INTERFACE);
		 r_UART_UART_iface_hsp_vertical->n(node_UART_HEATSPREAD);
		 r_UART_UART_iface_hsp_vertical->value=1.13328792;

		 //Resistor between UART and CORE in spreader layer!
 		 r_UART_CORE_hsp_layer_horizontal = new sca_eln::sca_r("r_UART_CORE_hsp_layer_horizontal");
		 r_UART_CORE_hsp_layer_horizontal->p(node_UART_HEATSPREAD);
		 r_UART_CORE_hsp_layer_horizontal->n(node_CORE_HEATSPREAD);
		 r_UART_CORE_hsp_layer_horizontal->value=3.35988632;

		 //Resistor between UART and MEM in spreader layer!
 		 r_UART_MEM_hsp_layer_horizontal = new sca_eln::sca_r("r_UART_MEM_hsp_layer_horizontal");
		 r_UART_MEM_hsp_layer_horizontal->p(node_UART_HEATSPREAD);
		 r_UART_MEM_hsp_layer_horizontal->n(node_MEM_HEATSPREAD);
		 r_UART_MEM_hsp_layer_horizontal->value=1.85135302;

		 //Resistor between UART and RFTR in spreader layer!
 		 r_UART_RFTR_hsp_layer_horizontal = new sca_eln::sca_r("r_UART_RFTR_hsp_layer_horizontal");
		 r_UART_RFTR_hsp_layer_horizontal->p(node_UART_HEATSPREAD);
		 r_UART_RFTR_hsp_layer_horizontal->n(node_RFTR_HEATSPREAD);
		 r_UART_RFTR_hsp_layer_horizontal->value=1.00116164;


		 // Resistor between CORE in spreader layer and sink layer!
		 r_CORE_CORE_hsp_hsink_vertical = new sca_eln::sca_r("r_CORE_CORE_hsp_hsink_vertical");
		 r_CORE_CORE_hsp_hsink_vertical->p(node_CORE_HEATSPREAD);
		 r_CORE_CORE_hsp_hsink_vertical->n(node_CORE_HEATSINK);
		 r_CORE_CORE_hsp_hsink_vertical->value=2.50000001;


		 // Resistor between MEM in spreader layer and sink layer!
		 r_MEM_MEM_hsp_hsink_vertical = new sca_eln::sca_r("r_MEM_MEM_hsp_hsink_vertical");
		 r_MEM_MEM_hsp_hsink_vertical->p(node_MEM_HEATSPREAD);
		 r_MEM_MEM_hsp_hsink_vertical->n(node_MEM_HEATSINK);
		 r_MEM_MEM_hsp_hsink_vertical->value=0.20833333;

		 //Resistor between MEM and CORE in heat sink layer!
 		 r_MEM_CORE_hsink_layer_horizontal = new sca_eln::sca_r("r_MEM_CORE_hsink_layer_horizontal");
		 r_MEM_CORE_hsink_layer_horizontal->p(node_MEM_HEATSINK);
		 r_MEM_CORE_hsink_layer_horizontal->n(node_CORE_HEATSINK);
		 r_MEM_CORE_hsink_layer_horizontal->value=1.17109362;


		 // Resistor between RFTR in spreader layer and sink layer!
		 r_RFTR_RFTR_hsp_hsink_vertical = new sca_eln::sca_r("r_RFTR_RFTR_hsp_hsink_vertical");
		 r_RFTR_RFTR_hsp_hsink_vertical->p(node_RFTR_HEATSPREAD);
		 r_RFTR_RFTR_hsp_hsink_vertical->n(node_RFTR_HEATSINK);
		 r_RFTR_RFTR_hsp_hsink_vertical->value=0.62497274;


		 // Resistor between UART in spreader layer and sink layer!
		 r_UART_UART_hsp_hsink_vertical = new sca_eln::sca_r("r_UART_UART_hsp_hsink_vertical");
		 r_UART_UART_hsp_hsink_vertical->p(node_UART_HEATSPREAD);
		 r_UART_UART_hsp_hsink_vertical->n(node_UART_HEATSINK);
		 r_UART_UART_hsp_hsink_vertical->value=0.56664396;

		 //Resistor between UART and CORE in heat sink layer!
 		 r_UART_CORE_hsink_layer_horizontal = new sca_eln::sca_r("r_UART_CORE_hsink_layer_horizontal");
		 r_UART_CORE_hsink_layer_horizontal->p(node_UART_HEATSINK);
		 r_UART_CORE_hsink_layer_horizontal->n(node_CORE_HEATSINK);
		 r_UART_CORE_hsink_layer_horizontal->value=0.48694005;

		 //Resistor between UART and MEM in heat sink layer!
 		 r_UART_MEM_hsink_layer_horizontal = new sca_eln::sca_r("r_UART_MEM_hsink_layer_horizontal");
		 r_UART_MEM_hsink_layer_horizontal->p(node_UART_HEATSINK);
		 r_UART_MEM_hsink_layer_horizontal->n(node_MEM_HEATSINK);
		 r_UART_MEM_hsink_layer_horizontal->value=0.26831203;

		 //Resistor between UART and RFTR in heat sink layer!
 		 r_UART_RFTR_hsink_layer_horizontal = new sca_eln::sca_r("r_UART_RFTR_hsink_layer_horizontal");
		 r_UART_RFTR_hsink_layer_horizontal->p(node_UART_HEATSINK);
		 r_UART_RFTR_hsink_layer_horizontal->n(node_RFTR_HEATSINK);
		 r_UART_RFTR_hsink_layer_horizontal->value=0.14509589;


		 // Resistors between unit mapped in spread and spread border four direction->SP_W!
		 r_SP_W_CORE_hsp_border_horizontal = new sca_eln::sca_r("r_SP_W_CORE_hsp_border_horizontal");
		 r_SP_W_CORE_hsp_border_horizontal->p(node_CORE_HEATSPREAD);
		 r_SP_W_CORE_hsp_border_horizontal->n(node_SP_W);
		 r_SP_W_CORE_hsp_border_horizontal->value=2.79446681;

		 // Resistors between unit mapped in spread and spread border four direction->SP_W!
		 r_SP_W_MEM_hsp_border_horizontal = new sca_eln::sca_r("r_SP_W_MEM_hsp_border_horizontal");
		 r_SP_W_MEM_hsp_border_horizontal->p(node_MEM_HEATSPREAD);
		 r_SP_W_MEM_hsp_border_horizontal->n(node_SP_W);
		 r_SP_W_MEM_hsp_border_horizontal->value=6.72987257;

		 // Resistors between unit mapped in spread and spread border four direction->SP_W!
		 r_SP_W_RFTR_hsp_border_horizontal = new sca_eln::sca_r("r_SP_W_RFTR_hsp_border_horizontal");
		 r_SP_W_RFTR_hsp_border_horizontal->p(node_RFTR_HEATSPREAD);
		 r_SP_W_RFTR_hsp_border_horizontal->n(node_SP_W);
		 r_SP_W_RFTR_hsp_border_horizontal->value=20.18873722;


		 // Resistors between unit mapped in spread and spread border four direction->SP_E!
		 r_SP_E_MEM_hsp_border_horizontal = new sca_eln::sca_r("r_SP_E_MEM_hsp_border_horizontal");
		 r_SP_E_MEM_hsp_border_horizontal->p(node_MEM_HEATSPREAD);
		 r_SP_E_MEM_hsp_border_horizontal->n(node_SP_E);
		 r_SP_E_MEM_hsp_border_horizontal->value=4.14187386;

		 // Resistors between unit mapped in spread and spread border four direction->SP_E!
		 r_SP_E_RFTR_hsp_border_horizontal = new sca_eln::sca_r("r_SP_E_RFTR_hsp_border_horizontal");
		 r_SP_E_RFTR_hsp_border_horizontal->p(node_RFTR_HEATSPREAD);
		 r_SP_E_RFTR_hsp_border_horizontal->n(node_SP_E);
		 r_SP_E_RFTR_hsp_border_horizontal->value=12.42507967;

		 // Resistors between unit mapped in spread and spread border four direction->SP_E!
		 r_SP_E_UART_hsp_border_horizontal = new sca_eln::sca_r("r_SP_E_UART_hsp_border_horizontal");
		 r_SP_E_UART_hsp_border_horizontal->p(node_UART_HEATSPREAD);
		 r_SP_E_UART_hsp_border_horizontal->n(node_SP_E);
		 r_SP_E_UART_hsp_border_horizontal->value=7.46410529;


		 // Resistors between unit mapped in spread and spread border four direction->SP_N!
		 r_SP_N_RFTR_hsp_border_horizontal = new sca_eln::sca_r("r_SP_N_RFTR_hsp_border_horizontal");
		 r_SP_N_RFTR_hsp_border_horizontal->p(node_RFTR_HEATSPREAD);
		 r_SP_N_RFTR_hsp_border_horizontal->n(node_SP_N);
		 r_SP_N_RFTR_hsp_border_horizontal->value=1.81722029;


		 // Resistors between unit mapped in spread and spread border four direction->SP_S!
		 r_SP_S_MEM_hsp_border_horizontal = new sca_eln::sca_r("r_SP_S_MEM_hsp_border_horizontal");
		 r_SP_S_MEM_hsp_border_horizontal->p(node_MEM_HEATSPREAD);
		 r_SP_S_MEM_hsp_border_horizontal->n(node_SP_S);
		 r_SP_S_MEM_hsp_border_horizontal->value=2.50926060;


		 // Resistor between CORE in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_W_CORE_sink_horizontal =new sca_eln::sca_r("r_SINK_C_W_CORE_sink_horizontal"); 
		 r_SINK_C_W_CORE_sink_horizontal->p(node_CORE_HEATSINK);
		 r_SINK_C_W_CORE_sink_horizontal->n(node_SINK_C_W);
		 r_SINK_C_W_CORE_sink_horizontal->value=0.40499519;

		 // Resistor between MEM in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_W_MEM_sink_horizontal =new sca_eln::sca_r("r_SINK_C_W_MEM_sink_horizontal"); 
		 r_SINK_C_W_MEM_sink_horizontal->p(node_MEM_HEATSINK);
		 r_SINK_C_W_MEM_sink_horizontal->n(node_SINK_C_W);
		 r_SINK_C_W_MEM_sink_horizontal->value=0.97534385;

		 // Resistor between RFTR in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_W_RFTR_sink_horizontal =new sca_eln::sca_r("r_SINK_C_W_RFTR_sink_horizontal"); 
		 r_SINK_C_W_RFTR_sink_horizontal->p(node_RFTR_HEATSINK);
		 r_SINK_C_W_RFTR_sink_horizontal->n(node_SINK_C_W);
		 r_SINK_C_W_RFTR_sink_horizontal->value=2.92590395;

		 // Resistor between SP_W in preader border and heat sink 1st level border!
 		 r_SINK_C_W_SP_W_vertical= new sca_eln::sca_r("r_SINK_C_W_SP_W_vertical");
		 r_SINK_C_W_SP_W_vertical->p(node_SP_W);
		 r_SINK_C_W_SP_W_vertical->n(node_SINK_C_W);
		 r_SINK_C_W_SP_W_vertical->value=0.01071131;


		 // Resistor between MEM in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_E_MEM_sink_horizontal =new sca_eln::sca_r("r_SINK_C_E_MEM_sink_horizontal"); 
		 r_SINK_C_E_MEM_sink_horizontal->p(node_MEM_HEATSINK);
		 r_SINK_C_E_MEM_sink_horizontal->n(node_SINK_C_E);
		 r_SINK_C_E_MEM_sink_horizontal->value=0.60027157;

		 // Resistor between RFTR in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_E_RFTR_sink_horizontal =new sca_eln::sca_r("r_SINK_C_E_RFTR_sink_horizontal"); 
		 r_SINK_C_E_RFTR_sink_horizontal->p(node_RFTR_HEATSINK);
		 r_SINK_C_E_RFTR_sink_horizontal->n(node_SINK_C_E);
		 r_SINK_C_E_RFTR_sink_horizontal->value=1.80073618;

		 // Resistor between UART in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_E_UART_sink_horizontal =new sca_eln::sca_r("r_SINK_C_E_UART_sink_horizontal"); 
		 r_SINK_C_E_UART_sink_horizontal->p(node_UART_HEATSINK);
		 r_SINK_C_E_UART_sink_horizontal->n(node_SINK_C_E);
		 r_SINK_C_E_UART_sink_horizontal->value=1.08175439;

		 // Resistor between SP_E in preader border and heat sink 1st level border!
 		 r_SINK_C_E_SP_E_vertical= new sca_eln::sca_r("r_SINK_C_E_SP_E_vertical");
		 r_SINK_C_E_SP_E_vertical->p(node_SP_E);
		 r_SINK_C_E_SP_E_vertical->n(node_SINK_C_E);
		 r_SINK_C_E_SP_E_vertical->value=0.01071131;


		 // Resistor between RFTR in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_N_RFTR_sink_horizontal =new sca_eln::sca_r("r_SINK_C_N_RFTR_sink_horizontal"); 
		 r_SINK_C_N_RFTR_sink_horizontal->p(node_RFTR_HEATSINK);
		 r_SINK_C_N_RFTR_sink_horizontal->n(node_SINK_C_N);
		 r_SINK_C_N_RFTR_sink_horizontal->value=0.26336526;

		 // Resistor between SP_N in preader border and heat sink 1st level border!
 		 r_SINK_C_N_SP_N_vertical= new sca_eln::sca_r("r_SINK_C_N_SP_N_vertical");
		 r_SINK_C_N_SP_N_vertical->p(node_SP_N);
		 r_SINK_C_N_SP_N_vertical->n(node_SINK_C_N);
		 r_SINK_C_N_SP_N_vertical->value=0.01214231;


		 // Resistor between MEM in heat sink layer and heat sink 1st level border!
 		 r_SINK_C_S_MEM_sink_horizontal =new sca_eln::sca_r("r_SINK_C_S_MEM_sink_horizontal"); 
		 r_SINK_C_S_MEM_sink_horizontal->p(node_MEM_HEATSINK);
		 r_SINK_C_S_MEM_sink_horizontal->n(node_SINK_C_S);
		 r_SINK_C_S_MEM_sink_horizontal->value=0.36366096;

		 // Resistor between SP_S in preader border and heat sink 1st level border!
 		 r_SINK_C_S_SP_S_vertical= new sca_eln::sca_r("r_SINK_C_S_SP_S_vertical");
		 r_SINK_C_S_SP_S_vertical->p(node_SP_S);
		 r_SINK_C_S_SP_S_vertical->n(node_SINK_C_S);
		 r_SINK_C_S_SP_S_vertical->value=0.01214231;


		 // Resistor between 1st and 2nd level border in sink layer of direction SINK_W!!
		 r_SINK_W_SINK_C_W_horizontal = new sca_eln::sca_r("r_SINK_W_SINK_C_W_horizontal");
		 r_SINK_W_SINK_C_W_horizontal->p(node_SINK_C_W);
		 r_SINK_W_SINK_C_W_horizontal->n(node_SINK_W);
		 r_SINK_W_SINK_C_W_horizontal->value=0.17171913;


		 // Resistor between 1st and 2nd level border in sink layer of direction SINK_E!!
		 r_SINK_E_SINK_C_E_horizontal = new sca_eln::sca_r("r_SINK_E_SINK_C_E_horizontal");
		 r_SINK_E_SINK_C_E_horizontal->p(node_SINK_C_E);
		 r_SINK_E_SINK_C_E_horizontal->n(node_SINK_E);
		 r_SINK_E_SINK_C_E_horizontal->value=0.17171913;


		 // Resistor between 1st and 2nd level border in sink layer of direction SINK_N!!
		 r_SINK_N_SINK_C_N_horizontal = new sca_eln::sca_r("r_SINK_N_SINK_C_N_horizontal");
		 r_SINK_N_SINK_C_N_horizontal->p(node_SINK_C_N);
		 r_SINK_N_SINK_C_N_horizontal->n(node_SINK_N);
		 r_SINK_N_SINK_C_N_horizontal->value=0.16657637;


		 // Resistor between 1st and 2nd level border in sink layer of direction SINK_S!!
		 r_SINK_S_SINK_C_S_horizontal = new sca_eln::sca_r("r_SINK_S_SINK_C_S_horizontal");
		 r_SINK_S_SINK_C_S_horizontal->p(node_SINK_C_S);
		 r_SINK_S_SINK_C_S_horizontal->n(node_SINK_S);
		 r_SINK_S_SINK_C_S_horizontal->value=0.16657637;


		/*

		 // Internal capacitor instantiation!!!
		 // Capacitor of CORE_SILICON in the thermal model!
		 c_CORE_SILICON = new sca_eln::sca_c("c_CORE_SILICON");
		 c_CORE_SILICON->p(node_CORE_SILICON);
		 c_CORE_SILICON->n(gnd);
		 c_CORE_SILICON->value=0.00008741;

		 // Capacitor of MEM_SILICON in the thermal model!
		 c_MEM_SILICON = new sca_eln::sca_c("c_MEM_SILICON");
		 c_MEM_SILICON->p(node_MEM_SILICON);
		 c_MEM_SILICON->n(gnd);
		 c_MEM_SILICON->value=0.00104895;

		 // Capacitor of RFTR_SILICON in the thermal model!
		 c_RFTR_SILICON = new sca_eln::sca_c("c_RFTR_SILICON");
		 c_RFTR_SILICON->p(node_RFTR_SILICON);
		 c_RFTR_SILICON->n(gnd);
		 c_RFTR_SILICON->value=0.00034967;

		 // Capacitor of UART_SILICON in the thermal model!
		 c_UART_SILICON = new sca_eln::sca_c("c_UART_SILICON");
		 c_UART_SILICON->p(node_UART_SILICON);
		 c_UART_SILICON->n(gnd);
		 c_UART_SILICON->value=0.00038566;

		 // Capacitor of CORE_INTERFACE in the thermal model!
		 c_CORE_INTERFACE = new sca_eln::sca_c("c_CORE_INTERFACE");
		 c_CORE_INTERFACE->p(node_CORE_INTERFACE);
		 c_CORE_INTERFACE->n(gnd);
		 c_CORE_INTERFACE->value=0.00002664;

		 // Capacitor of MEM_INTERFACE in the thermal model!
		 c_MEM_INTERFACE = new sca_eln::sca_c("c_MEM_INTERFACE");
		 c_MEM_INTERFACE->p(node_MEM_INTERFACE);
		 c_MEM_INTERFACE->n(gnd);
		 c_MEM_INTERFACE->value=0.00031968;

		 // Capacitor of RFTR_INTERFACE in the thermal model!
		 c_RFTR_INTERFACE = new sca_eln::sca_c("c_RFTR_INTERFACE");
		 c_RFTR_INTERFACE->p(node_RFTR_INTERFACE);
		 c_RFTR_INTERFACE->n(gnd);
		 c_RFTR_INTERFACE->value=0.00010656;

		 // Capacitor of UART_INTERFACE in the thermal model!
		 c_UART_INTERFACE = new sca_eln::sca_c("c_UART_INTERFACE");
		 c_UART_INTERFACE->p(node_UART_INTERFACE);
		 c_UART_INTERFACE->n(gnd);
		 c_UART_INTERFACE->value=0.00011753;

		 // Capacitor of CORE_HEATSPREAD in the thermal model!
		 c_CORE_HEATSPREAD = new sca_eln::sca_c("c_CORE_HEATSPREAD");
		 c_CORE_HEATSPREAD->p(node_CORE_HEATSPREAD);
		 c_CORE_HEATSPREAD->n(gnd);
		 c_CORE_HEATSPREAD->value=0.00118215;

		 // Capacitor of MEM_HEATSPREAD in the thermal model!
		 c_MEM_HEATSPREAD = new sca_eln::sca_c("c_MEM_HEATSPREAD");
		 c_MEM_HEATSPREAD->p(node_MEM_HEATSPREAD);
		 c_MEM_HEATSPREAD->n(gnd);
		 c_MEM_HEATSPREAD->value=0.01418580;

		 // Capacitor of RFTR_HEATSPREAD in the thermal model!
		 c_RFTR_HEATSPREAD = new sca_eln::sca_c("c_RFTR_HEATSPREAD");
		 c_RFTR_HEATSPREAD->p(node_RFTR_HEATSPREAD);
		 c_RFTR_HEATSPREAD->n(gnd);
		 c_RFTR_HEATSPREAD->value=0.00472881;

		 // Capacitor of UART_HEATSPREAD in the thermal model!
		 c_UART_HEATSPREAD = new sca_eln::sca_c("c_UART_HEATSPREAD");
		 c_UART_HEATSPREAD->p(node_UART_HEATSPREAD);
		 c_UART_HEATSPREAD->n(gnd);
		 c_UART_HEATSPREAD->value=0.00521558;

		 // Capacitor of CORE_HEATSINK in the thermal model!
		 c_CORE_HEATSINK = new sca_eln::sca_c("c_CORE_HEATSINK");
		 c_CORE_HEATSINK->p(node_CORE_HEATSINK);
		 c_CORE_HEATSINK->n(gnd);
		 c_CORE_HEATSINK->value=0.02114383;

		 // Capacitor of MEM_HEATSINK in the thermal model!
		 c_MEM_HEATSINK = new sca_eln::sca_c("c_MEM_HEATSINK");
		 c_MEM_HEATSINK->p(node_MEM_HEATSINK);
		 c_MEM_HEATSINK->n(gnd);
		 c_MEM_HEATSINK->value=0.25372602;

		 // Capacitor of RFTR_HEATSINK in the thermal model!
		 c_RFTR_HEATSINK = new sca_eln::sca_c("c_RFTR_HEATSINK");
		 c_RFTR_HEATSINK->p(node_RFTR_HEATSINK);
		 c_RFTR_HEATSINK->n(gnd);
		 c_RFTR_HEATSINK->value=0.08457903;

		 // Capacitor of UART_HEATSINK in the thermal model!
		 c_UART_HEATSINK = new sca_eln::sca_c("c_UART_HEATSINK");
		 c_UART_HEATSINK->p(node_UART_HEATSINK);
		 c_UART_HEATSINK->n(gnd);
		 c_UART_HEATSINK->value=0.09328536;

		 // Capacitor of SP_W in the thermal model!
		 c_SP_W = new sca_eln::sca_c("c_SP_W");
		 c_SP_W->p(node_SP_W);
		 c_SP_W->n(gnd);
		 c_SP_W->value=0.27591164;

		 // Capacitor of SP_E in the thermal model!
		 c_SP_E = new sca_eln::sca_c("c_SP_E");
		 c_SP_E->p(node_SP_E);
		 c_SP_E->n(gnd);
		 c_SP_E->value=0.27591164;

		 // Capacitor of SP_N in the thermal model!
		 c_SP_N = new sca_eln::sca_c("c_SP_N");
		 c_SP_N->p(node_SP_N);
		 c_SP_N->n(gnd);
		 c_SP_N->value=0.24339481;

		 // Capacitor of SP_S in the thermal model!
		 c_SP_S = new sca_eln::sca_c("c_SP_S");
		 c_SP_S->p(node_SP_S);
		 c_SP_S->n(gnd);
		 c_SP_S->value=0.24339481;

		 // Capacitor of SINK_C_W in the thermal model!
		 c_SINK_C_W = new sca_eln::sca_c("c_SINK_C_W");
		 c_SINK_C_W->p(node_SINK_C_W);
		 c_SINK_C_W->n(gnd);
		 c_SINK_C_W->value=4.93493234;

		 // Capacitor of SINK_C_E in the thermal model!
		 c_SINK_C_E = new sca_eln::sca_c("c_SINK_C_E");
		 c_SINK_C_E->p(node_SINK_C_E);
		 c_SINK_C_E->n(gnd);
		 c_SINK_C_E->value=4.93493234;

		 // Capacitor of SINK_C_N in the thermal model!
		 c_SINK_C_N = new sca_eln::sca_c("c_SINK_C_N");
		 c_SINK_C_N->p(node_SINK_C_N);
		 c_SINK_C_N->n(gnd);
		 c_SINK_C_N->value=4.35333901;

		 // Capacitor of SINK_C_S in the thermal model!
		 c_SINK_C_S = new sca_eln::sca_c("c_SINK_C_S");
		 c_SINK_C_S->p(node_SINK_C_S);
		 c_SINK_C_S->n(gnd);
		 c_SINK_C_S->value=4.35333901;

		 // Capacitor of SINK_W in the thermal model!
		 c_SINK_W = new sca_eln::sca_c("c_SINK_W");
		 c_SINK_W->p(node_SINK_W);
		 c_SINK_W->n(gnd);
		 c_SINK_W->value=14.27208863;

		 // Capacitor of SINK_E in the thermal model!
		 c_SINK_E = new sca_eln::sca_c("c_SINK_E");
		 c_SINK_E->p(node_SINK_E);
		 c_SINK_E->n(gnd);
		 c_SINK_E->value=14.27208863;

		 // Capacitor of SINK_N in the thermal model!
		 c_SINK_N = new sca_eln::sca_c("c_SINK_N");
		 c_SINK_N->p(node_SINK_N);
		 c_SINK_N->n(gnd);
		 c_SINK_N->value=14.27208863;

		 // Capacitor of SINK_S in the thermal model!
		 c_SINK_S = new sca_eln::sca_c("c_SINK_S");
		 c_SINK_S->p(node_SINK_S);
		 c_SINK_S->n(gnd);
		 c_SINK_S->value=14.27208863;
		*/


		 }// end CTOR

};// end module

