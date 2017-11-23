/******************************************************/
/***This file is used to simulated in systemc-ams. ****/
/***                  Yukai Chen                   ****/
/***                     EDA                       ****/
/******************************************************/



#include "testbench.h"
#include "systemc-ams.h"
#include "rc_network.h"



int sc_main(int argc, char* argv[]){

 	//Define power signals
	 sca_tdf::sca_signal<double>  powerCORE,  powerMEM,  powerRFTR,  powerUART; 

 	 //Define temperature signals
	 sca_tdf::sca_signal<double>  tempCORE,  tempMEM,  tempRFTR,  tempUART; 

	 //Instantiate the themal model 
	 thermal_network temp_model("temp_model");

	 //Instantiate the testbench 
	 testbench test("test");

	 // Binding!!
	 temp_model.p_CORE(powerCORE); 
	 temp_model.p_MEM(powerMEM); 
	 temp_model.p_RFTR(powerRFTR); 
	 temp_model.p_UART(powerUART); 


	 temp_model.t_CORE(tempCORE); 
	 temp_model.t_MEM(tempMEM); 
	 temp_model.t_RFTR(tempRFTR); 
	 temp_model.t_UART(tempUART); 


	 test.CORE_p(powerCORE); 
	 test.MEM_p(powerMEM); 
	 test.RFTR_p(powerRFTR); 
	 test.UART_p(powerUART); 


	 test.CORE_t(tempCORE); 
	 test.MEM_t(tempMEM); 
	 test.RFTR_t(tempRFTR); 
	 test.UART_t(tempUART); 


	 sc_start(2,SC_MS);
	 cout<<"simulation time is:"<<sc_time_stamp()<<endl;

}


